#include "ResponseMaker.hpp"

int main()
{
	Client client(1, 1);
	Config config(1);
	ResponseMaker Response(client, config);
	Response.DecideConfigServer();
	std::cout << Response.configServer.host << std::endl;
	std::cout << Response.configServer.port << std::endl;
	std::cout << Response.configServer.root << std::endl;

	Response.DecideConfigLocation();
	std::cout << Response.configLocation.path << std::endl;
	std::cout << Response.configLocation.alias << std::endl;
	std::cout << Response.configLocation.configCommon.autoindex << std::endl;

	std::cout << Response.GetSerachAbsolutePath() << std::endl;

}