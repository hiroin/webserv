/*
テストケース
https://docs.google.com/spreadsheets/d/1LNw2mvutl0X-ehI2lNGltf06Z1zOF04SorS-GIM8q2U/edit#gid=0
*/
#include "Response.hpp"
#include <string.h>
#include <string>



int main()
{
    Client client;
    client.port = 5000;
    client.host = "*";
    client.hmp.method_ = httpMessageParser::GET;
    client.hmp.absolutePath_ = "/";
    client.hmp.headers_["host"] = "127.0.0.1";
    client.hmp.headers_["authorization"] = "1234:tyuio";
    Config config(1);

    Response Response(client, config);

    // int ResponseStatus = Response.ResponseStatus;
    // int ErrorPageFd = Response.getErrorFileFd();
    // size_t ErrorPageSize = Response.getErrorContentLength();
    // int TargetFileFd = Response.getTargetFileFd();
    // size_t TargetSize = Response.getContentLength();

    // std::cout << "response_code  : " << ResponseStatus << std::endl;
    // std::cout << "file_path      : " << Response.targetFilePath << std::endl;
    // std::cout << "errorfile_path : " << Response.errorFilePath << std::endl;

    // std::cout << "--------response---------" << Response.errorFilePath << std::endl;
    // std::string &ResponseMessage = Response.responseMessege;
    // ResponseMessage.append("Additional Information");
    std::cout << Response.responseMessege << std::endl;
}
