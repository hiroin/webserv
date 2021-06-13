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
    client.hmp.method_ = httpMessageParser::POST;
    client.hmp.absolutePath_ = "/index.bla";
    client.hmp.headers_["host"] = "127.0.0.1";
    client.hmp.headers_["transfer-encoding"] = std::string("chunked");
    client.hmp.pathinfo_ = "/Users/iwasayoshiki/webserv/testData20210416/cgi_tester";
    client.body = "abcd";
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
    sleep(1);
    int writeFd = Response.getCgiFdForWrite();
    std::cout << "writeFd = " << writeFd << std::endl;
    int ret = write(writeFd, "abcd", 4);
    // std::cout << ret << std::endl;
    sleep(1);
    char buf[1000];
    int readFd = Response.getCgiFd();
    std::cout << "readFd = " << readFd << std::endl;
    ret = read(readFd, buf, 1000);
    std::cout << ret << std::endl;
    std::cout << buf << std::endl;
    std::cout << Response.responseMessege << std::endl;
}
