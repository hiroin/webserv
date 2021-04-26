#include "Response.hpp"
#include <string.h>




int main()
{
	Client client(1, 1);
	Config config(1);
	Response Response(404, client, config);

	std::cout << Response.responseMessege << std::endl;

}
