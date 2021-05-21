/*
テストケース
https://docs.google.com/spreadsheets/d/1LNw2mvutl0X-ehI2lNGltf06Z1zOF04SorS-GIM8q2U/edit#gid=0
*/

#include "../../Response.hpp"
#include "../../parseConfig.hpp"

int main()
{
  const char* configfile = "/home/user42/42/webserv/yiwasa/webserv/prototype/googletest/testcase/022_cgiscripts.conf";
	Config config_;
  Client client_;
  parseConfig(configfile, config_);
  client_.port = 8080;
  client_.host = "*";
  client_.hmp.method_ = httpMessageParser::GET;
  client_.hmp.headers_["host"] = "127.0.0.1";
  client_.hmp.absolutePath_ = "/index.php";
  client_.hmp.query_ = "name=ap2";
  client_.hmp.requestTarget_ = "/index.php?name=ap2";
  client_.ip = "127.0.0.1";  
  Response Response(client_, config_);
     
  int ResponseStatus = Response.ResponseStatus;
  // int TargetFileFd = Response.getTargetFileFd();
  // size_t TargetSize = Response.getContentLength();
  
  std::cout << "response_code  : " << ResponseStatus << std::endl;
  
  // std::cout << "file_path      : " << Response.targetFilePath << std::endl;
  // std::cout << "--------response---------" << std::endl;
  // std::string &ResponseMessage = Response.responseMessege;
  // std::cout << ResponseMessage << std::endl;

  // CGIの試験
  char buf[10000];
  memset(buf, 0, sizeof(buf));
  ssize_t read_size = read(Response.getCgiFd(), buf, sizeof(buf));
  buf[read_size] = 0;
  std::cout << "Response.getCgiFd() : " << Response.getCgiFd() << std::endl;
  std::cout << "CGIの出力" << std::endl << buf << std::endl;

}

