#include "Response.hpp"
#include <string.h>


int main()
{
	Client client(1, 1);
	Config config(1);
	Response Response(client, config);

	int ErrorPageFd = Response.getErrorFileFd();
	size_t ErrorPageSize = Response.getErrorContentLength();

	int TargetFileFd = Response.getTargetFileFd();
	size_t TargetFileFd = Response.getContentLength();

	int ResponseStatus = Response.ResponseStatus;

	std::string &ResponseMessage = Response.responseMessege;
	ResponseMessage.append("Additional Information");
	std::cout << ResponseMessage << std::endl;
}
