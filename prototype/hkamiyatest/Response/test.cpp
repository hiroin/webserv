/*
テストケース
https://docs.google.com/spreadsheets/d/1LNw2mvutl0X-ehI2lNGltf06Z1zOF04SorS-GIM8q2U/edit#gid=0
*/

#include "../../Response.hpp"
#include "../../parseConfig.hpp"

int main()
{
  const char* configfile = "/home/user42/42/webserv/yiwasa/webserv/prototype/googletest/testcase/018_basicauth.conf";
	Config config_;
  Client client_;
  parseConfig(configfile, config_);
  client_.port = 8080;
  client_.host = "*";
  client_.hmp.method_ = httpMessageParser::GET;
  client_.hmp.absolutePath_ = "/index.html";
  client_.hmp.headers_["host"] = "127.0.0.1";
  client_.hmp.headers_["authorization"] = "Basic dXNlcjpwYXNzd29yZA==";
  Response Response(client_, config_);
     
  int ResponseStatus = Response.ResponseStatus;
  int TargetFileFd = Response.getTargetFileFd();
  size_t TargetSize = Response.getContentLength();

  std::cout << "response_code  : " << ResponseStatus << std::endl;
  std::cout << "file_path      : " << Response.targetFilePath << std::endl;

  std::cout << "--------response---------" << std::endl;
  std::string &ResponseMessage = Response.responseMessege;
  std::cout << ResponseMessage << std::endl;
}

