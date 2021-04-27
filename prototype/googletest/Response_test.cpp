#include <gtest/gtest.h>
#include "../Client.hpp"
#include "../Config.hpp"
#include "../Response.hpp"
#include "../parseConfig.hpp"

// googletestのインストール手順
// https://rinatz.github.io/cpp-book/test-how-to-gtest/
// g++ -std=c++11 ../HTTPMessageParser.cpp HTTPMessageParser_test.cpp -o test -L/usr/local/lib -pthread -lgtest -lgtest_main

TEST(HTTPMessageParserEvenTest, getAfterParseRequestLine)
{
  const char* configfile = "testcase/webserv.conf";
	Config config_;
  Client client_;
  parseConfig(configfile, config_);
  client_.port = 5000;
  client_.host = "*";
  client_.hmp.method_ = httpMessageParser::GET;
  client_.hmp.absolutePath_ = "/";
  client_.hmp.headers_["host"] = "127.0.0.1";

	Response Response(client_, config_);
	int ErrorPageFd = Response.getErrorFileFd();
	size_t ErrorPageSize = Response.getErrorContentLength();
	int TargetFileFd = Response.getTargetFileFd();
	size_t TargetSize = Response.getContentLength();
	int ResponseStatus = Response.ResponseStatus;

  EXPECT_EQ(200, ResponseStatus);
  EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
  
	// std::string &ResponseMessage = Response.responseMessege;
	// ResponseMessage.append("Additional Information");
	// std::cout << ResponseMessage << std::endl;
}
