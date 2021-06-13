#include "Response.hpp"

std::string ft_ultos(unsigned long nu);
std::string ft_itos(int nu);
std::vector<std::string> splitByPlus(std::string string);
std::string getMethodasString(int num);

bool Response::execPhpCgi_POST()
{
	std::string phpCgiPath = this->config.configGlobal.phpCgiPath;
	if (!isReadable(targetFilePath))
	{
		ResponseStatus = 500;
		return false;
	}
	int pipeIn[2];
	int pipeOut[2];
	pid_t pid;
	if (pipe(pipeIn) < 0)
	{
		ResponseStatus = 500;
		return false;
	}
	if (pipe(pipeOut) < 0)
	{
		ResponseStatus = 500;
		return false;
	}
	if ((pid = fork()) < 0)
	{
		ResponseStatus = 500;
		return false;
	}
	if (pid == 0)// 子プロセス
	{
		int retIn[3];
		retIn[0] = close(pipeIn[1]);
		retIn[1] = dup2(pipeIn[0], 0);// fd = 0 でpipeの内容が読み込める
		retIn[2] = close(pipeIn[0]); //こいつで

		int retOut[3];
		retOut[0] = close(pipeOut[0]);
		retOut[1] = dup2(pipeOut[1], 1);
		retOut[2] = close(pipeOut[1]);
		for(int i = 0; i < 3; i++)
		{
			if (retOut[i] < 0 || retIn[i] < 0)
			{
				exit(1); // exit
			}
		}
		argv.push_back(phpCgiPath);
		argv.push_back(targetFilePath);
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
		if (execve(Cargv[0], Cargv, Cenvp)  == -1)
		{
			exit(1);
		}
	}
	close(pipeOut[1]);
	close(pipeIn[0]);
	readFd = pipeOut[0];
	writeFd = pipeIn[1];
	ResponseStatus = 200;
	return true;
}
