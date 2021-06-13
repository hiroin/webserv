#ifndef A2879922_2A4B_4315_BE9F_5B8029A21A98
#define A2879922_2A4B_4315_BE9F_5B8029A21A98

#include "Response.hpp"
std::string ft_ultos(unsigned long nu);
std::string ft_itos(int nu);

std::vector<std::string> splitByPlus(std::string string)
{
	std::string separator = std::string("+");         // 区切り文字
	size_t separator_length = separator.length(); // 区切り文字の長さ
	std::vector<std::string> ret;
	if (separator_length == 0) {
  	ret.push_back(string);
	} else {
  	size_t offset = 0;
  	while (1) {
    	size_t pos = string.find(separator, offset);
    	if (pos == std::string::npos) {
      	ret.push_back(string.substr(offset));
      	break;
    	}
    	ret.push_back(string.substr(offset, pos - offset));
    	offset = pos + separator_length;
  		}
	}
	return ret;
}

std::string getMethodasString(int num)
{
	std::map<int, std::string> methods;
	methods[0] = "GET";
	methods[1] = "HEAD";
	methods[2] = "POST";
	methods[3] = "PUT";
	methods[4] = "DELETE";
	methods[5] = "CONNECT";
	methods[6] = "OPTION";
	methods[7] = "TRASE";
	methods[8] = "OTHER";
	return methods[num];
}

void Response::addEnvironmentValue()
{
	s_ConfigCommon configCommon = getConfigCommon();
	if (isNecesarryAuth())
	{
		envp.push_back("AUTH_TYPE=Basic");
		envp.push_back(std::string("REMOTE_USER=") + configCommon.authBasicUid);
	}
	if (client.hmp.method_ == httpMessageParser::POST)
	{
		size_t bodySize = PutPostBody.size();
		envp.push_back(std::string("CONTENT_LENGTH=") + ft_ultos(bodySize));
		envp.push_back(std::string("CONTENT_TYPE=") + client.hmp.headers_["content-type"]);
	}
	envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
  if (client.hmp.pathinfo_ != "")
  {
    envp.push_back(std::string("PATH_INFO=") + "/" + client.hmp.pathinfo_);
    if (configServer.root[configServer.root.size() - 1] != '/')
      configServer.root.append("/");
    envp.push_back(std::string("PATH_TRANSLATED=") + configServer.root + client.hmp.pathinfo_);
  }
	envp.push_back(std::string("QUERY_STRING=") + client.hmp.query_);
	envp.push_back(std::string("REMOTE_ADDR=") + client.ip);
	envp.push_back(std::string("REQUEST_METHOD=") + getMethodasString(client.hmp.method_));
	envp.push_back(std::string("REQUEST_URI=") + client.hmp.requestTarget_);
	envp.push_back(std::string("SCRIPT_NAME=") + client.hmp.absolutePath_);
	envp.push_back(std::string("SCRIPT_FILENAME=") + targetFilePath);
	envp.push_back(std::string("SERVER_NAME=") + client.hmp.headers_["host"]);
	envp.push_back(std::string("SERVER_PORT=") + ft_itos(client.port));
	envp.push_back(std::string("SERVER_PROTOCOL=") + "HTTP/1.1");
	envp.push_back(std::string("SERVER_SOFTWARE=") + "webserv");
	envp.push_back(std::string("REDIRECT_STATUS=") + "200");

}

bool Response::execCgi()
{
	if (!isExecutable(targetFilePath))
	{
		ResponseStatus = 500;
		return false;
	}
	//ここ以降、pipe して fork する
	int pipes[2];
	pid_t pid;
	if (pipe(pipes) < 0)
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
		// クエリの確認
		if (client.hmp.query_.find("=") != std::string::npos)
		{
			argv.push_back(targetFilePath);
		}
		else
		{
			if (client.hmp.query_.find("+") != std::string::npos)
			{
				std::vector<std::string> queries;
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
			}
		}
		addEnvironmentValue();
		//環境変数設定
		//環境変数をchar** に変換する関数
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
		if (execve(Cargv[0], Cargv, Cenvp)  == -1) //環境変数は他のものは必要なのかな？？？
		{
			exit(1);
		}
	}
	close(pipes[1]);
	readFd = pipes[0];
	ResponseStatus = 200;
	return true;
}

#endif /* A2879922_2A4B_4315_BE9F_5B8029A21A98 */
