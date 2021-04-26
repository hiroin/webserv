#include "Response.hpp"
#include <string.h>
#include <errno.h>

int main()
{
	Client client(1, 1);
	Config config(1);
	Response Response(client, config);
	// Response.DecideConfigServer();
	std::cout << Response.configServer.host << std::endl;
	std::cout << Response.configServer.port << std::endl;
	std::cout << Response.configServer.root << std::endl;

	// Response.DecideConfigLocation();
	std::cout << Response.configLocation.path << std::endl;
	std::cout << Response.configLocation.alias << std::endl;
	std::cout << Response.configLocation.configCommon.autoindex << std::endl;

	std::cout << Response.GetSerachAbsolutePath() << std::endl;
	Response.setTargetFileAndStatus();
	std::cout << Response.getContentLength() << std::endl;
	std::cout << Response.ResponseStatus << std::endl;

}
