#include "../../Response.hpp"
#include "../../parseConfig.hpp"
#include <string.h>

int main()
{
  const char* configfile = "../../googletest/testcase/webserv.conf";
	Config config_;
  Client client_;
  parseConfig(configfile, config_);
  client_.port = 5000;
  client_.host = "*";
  client_.hmp.method_ = httpMessageParser::GET;
  client_.hmp.absolutePath_ = "/";
  client_.hmp.headers_["host"] = "127.0.0.1";
  Response Response(client_, config_);
     
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
