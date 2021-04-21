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
	std::cout << Response.configServer.serverNames[0] << std::endl;


}