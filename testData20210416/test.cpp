#include "Response.hpp"
#include <string.h>

int main()
{
    Client client(1, 1);
    Config config(1);
    Response Response(client, config);
    
    
    int ResponseStatus = Response.ResponseStatus;
    int ErrorPageFd = Response.getErrorFileFd();
    size_t ErrorPageSize = Response.getErrorContentLength();
    int TargetFileFd = Response.getTargetFileFd();
    size_t TargetSize = Response.getContentLength();

    std::cout << "response_code  : " << ResponseStatus << std::endl;
    std::cout << "file_path      : " << Response.targetFilePath << std::endl;
    std::cout << "errorfile_path : " << Response.errorFilePath << std::endl;

    std::cout << "--------response---------" << Response.errorFilePath << std::endl;
    std::string &ResponseMessage = Response.responseMessege;
    ResponseMessage.append("Additional Information");
    std::cout << ResponseMessage << std::endl;
}
