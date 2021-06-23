#include "Response.hpp"

std::string ft_ultos(unsigned long nu);
std::string ft_itos(int nu);
std::vector<std::string> splitByPlus(std::string string);
std::string getMethodasString(int num);


bool Response::execCgiTester()
{
	std::string cgitesterPath = this->config.configGlobal.cgitesterPath;
	if (!isReadable(targetFilePath))
	{
		ResponseStatus = 500;
		return false;
	}
	//ここ以降、pipe して fork する
	int pipes[2];
  int pipeIn[2];
	pid_t pid;
	if (pipe(pipes) < 0)
	{
		ResponseStatus = 500;
		return false;
	}
  if (fcntl(pipes[0], F_SETFL, O_NONBLOCK) == -1
    || fcntl(pipes[1], F_SETFL, O_NONBLOCK) == -1)
  {
    close(pipes[0]);
    close(pipes[1]);
		ResponseStatus = 500;
    return (false);
  }
	if (pipe(pipeIn) < 0)
	{
		ResponseStatus = 500;
		return false;
	}  
	if ((pid = fork()) < 0)
	{
		ResponseStatus = 500;
		return false;
	}
	if (pid == 0)
	{
		int ret[3];
		ret[0] = close(pipes[0]);
		ret[1] = dup2(pipes[1], 1);
		ret[2] = close(pipes[1]);
		for(int i = 0; i < 3; i++)
		{
			if (ret[i] < 0)
			{
				exit(1); // exit
			}
		}
		int retIn[3];
		retIn[0] = close(pipeIn[1]);
		retIn[1] = dup2(pipeIn[0], 0);// fd = 0 でpipeの内容が読み込める
		retIn[2] = close(pipeIn[0]);
		for(int i = 0; i < 3; i++)
		{
			if (retIn[i] < 0)
			{
				exit(1); // exit
			}
		}
		// クエリの確認
		if (client.hmp.query_.find("=") != std::string::npos)
		{
			argv.push_back(targetFilePath);
			argv.push_back(targetFilePath);
		}
		else
		{
			if (client.hmp.query_.find("+") != std::string::npos)
			{
				std::vector<std::string> queries;
				argv.push_back(targetFilePath);
				argv.push_back(targetFilePath);
				queries = splitByPlus(client.hmp.query_);
				for(size_t i = 0; i < queries.size(); i++)
				{
					argv.push_back(queries[i]);
				}
			}
			else
			{
				argv.push_back(targetFilePath);
				argv.push_back(targetFilePath);
			}
		}
		addEnvironmentValue();
		size_t argvSize = argv.size();
		size_t envpSize = envp.size();
		char* Cargv[argvSize + 1];
		char* Cenvp[envpSize + 1];
		for(size_t i = 0; i < argvSize; i++)
		{
			Cargv[i] = const_cast<char*>(argv[i].c_str());
		}
		Cargv[argvSize] = NULL;

		for(size_t i = 0; i < envpSize; i++)
		{
			Cenvp[i] = const_cast<char*>(envp[i].c_str());
		}
		Cenvp[envpSize] = NULL;
		if (execve(cgitesterPath.c_str(), Cargv, Cenvp)  == -1)
		{
			exit(1);
		}
	}
	close(pipes[1]);
	close(pipeIn[0]);
 	close(pipeIn[1]);
	readFd = pipes[0];
	ResponseStatus = 200;
	return true;
}
