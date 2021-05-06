#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../Client.hpp"
#include "../Config.hpp"
#include "../Response.hpp"
#include "../parseConfig.hpp"

// Google TestでのMatcherの活用
// https://goyoki.hatenablog.com/entry/2013/01/16/003803
using ::testing::MatchesRegex;
using ::testing::Ge;

// googletestのインストール手順
// https://rinatz.github.io/cpp-book/test-how-to-gtest/
// g++ -std=c++11 ../HTTPMessageParser.cpp HTTPMessageParser_test.cpp -o test -L/usr/local/lib -pthread -lgtest -lgtest_main

TEST(Response_test, webserv_01)
{
  {
    const char* configfile = "testcase/webserv.conf";
    Config config_;
    parseConfig(configfile, config_);

    // 0001
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      size_t TargetSize = Response.getContentLength();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0002
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0003
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/aaa/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aaa/index.html", Response.targetFilePath);
    }
    // 0004
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/aaa/";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aaa/default.html", Response.targetFilePath);
    }
    // 0005
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/aa/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aa/index.html", Response.targetFilePath);
    }
    // 0006
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/aa/";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aa/index.html", Response.targetFilePath);
    }
    // 0007
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/www/aaa/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/404.html", Response.targetFilePath);
    }
  }
}

TEST(Response_test, webserv_02)
{
  const char* configfile = "testcase/webserv.conf";
  Config config_;
  parseConfig(configfile, config_);  
  {
    // 0008
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/bbb/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/1bbb/index.html", Response.targetFilePath);
    }
    // 0009
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/bb/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/1bb/index.html", Response.targetFilePath);
    }
    // 0010
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/www/bb/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
      EXPECT_EQ("", Response.targetFilePath);
    }
    // 0011
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/1.jpg";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/1.jpg", Response.targetFilePath);
    }
  }
}

TEST(Response_test, webserv_serverNames)
{
  // 012_webserv_serverNames.conf
  {
    const char* configfile = "testcase/012_webserv_serverNames.conf";
    Config config_;
    parseConfig(configfile, config_);
    // 0012
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/serverNames/5000/default/index.html", Response.targetFilePath);
    }
    // 0013
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/index.html";
      client_.hmp.headers_["host"] = "server10";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/serverNames/5000/server1x/index.html", Response.targetFilePath);
    }
    // 0014
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/index.html";
      client_.hmp.headers_["host"] = "server11";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/serverNames/5000/server1x/index.html", Response.targetFilePath);
    }
    // 0015
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/index.html";
      client_.hmp.headers_["host"] = "server20";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/serverNames/5001/server2x/index.html", Response.targetFilePath);
    }
    // 0016
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/index.html";
      client_.hmp.headers_["host"] = "server";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/serverNames/5001/server1x/index.html", Response.targetFilePath);
    }

    // 0017
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/index.html";
      client_.hmp.headers_["host"] = "127.0.0.1:5000";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/serverNames/5000/default/index.html", Response.targetFilePath);
    }
  }  
}

TEST(Response_test, webserv_error)
{
  const char* configfile = "testcase/webserv.conf";
  Config config_;
  parseConfig(configfile, config_);  
  {
    // 0018
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/none";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/404.html", Response.targetFilePath);
    }
    // 0019
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/aaa/none";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/404.html", Response.targetFilePath);
    }
    // 0020
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/aa/none";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/404.html", Response.targetFilePath);
    }
    // 0021
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/root.html";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int TargetFileFd = Response.getTargetFileFd();
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(403, ResponseStatus);
    }
    // 0022
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/none";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
      EXPECT_EQ("", Response.targetFilePath);
    }
    // 0023
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/bbb/none";
      client_.hmp.headers_["host"] = "127.0.0.1";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
      EXPECT_EQ("", Response.targetFilePath);
    }
  }
}

TEST(Response_test, webserv_index)
{
  const char* configfile = "testcase/015_index.conf";
  Config config_;
  parseConfig(configfile, config_);  
  {
    // 0024
    try
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0024" << '\n';
      std::cerr << e.what() << '\n';
    }
    
    // 0025
    try
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/aaap1/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aaa/default.html", Response.targetFilePath);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0025" << '\n';
      std::cerr << e.what() << '\n';
    }
    // 0026
    try
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/aaap2/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aaa/index.html", Response.targetFilePath);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0026" << '\n';
      std::cerr << e.what() << '\n';
    }
    // 0027
    try
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/aaap3/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aaa/default.html", Response.targetFilePath);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0027" << '\n';
      std::cerr << e.what() << '\n';
    }
    // 0028
    try
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/aaap4/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aaa/default.html", Response.targetFilePath);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0028" << '\n';
      std::cerr << e.what() << '\n';
    }
    // 0029
    try
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/aaap5/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0029" << '\n';
      std::cerr << e.what() << '\n';
    }
    // 0030
    try
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/aa/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0030" << '\n';
      std::cerr << e.what() << '\n';
    }
    // 0031
    try
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/1//index.html", Response.targetFilePath);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0031" << '\n';
      std::cerr << e.what() << '\n';
    }
    // 0032
    try
    {
      Client client_;
      client_.port = 5001;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/bbb/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(404, ResponseStatus);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0032" << '\n';
      std::cerr << e.what() << '\n';
    }
    // 00
    {
      // Client client_;
      // client_.port = ;
      // client_.host = "*";
      // client_.hmp.method_ = httpMessageParser::GET;
      // client_.hmp.headers_["host"] = "";
      // client_.hmp.absolutePath_ = "";

      // Response Response(client_, config_);
      // int ResponseStatus = Response.ResponseStatus;

      // EXPECT_EQ(, ResponseStatus);
      // EXPECT_EQ("", Response.targetFilePath);
      // EXPECT_EQ("", Response.errorFilePath);
    }
  }
}

TEST(Response_test, webserv_index_2)
{
  const char* configfile = "testcase/015_index2.conf";
  Config config_;
  parseConfig(configfile, config_);  
  {
    // 0056
    try
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(403, ResponseStatus);
    }
    catch(const std::exception& e)
    {
      std::cerr << "0024" << '\n';
      std::cerr << e.what() << '\n';
    }
  }
}

TEST(Response_test, allow)
{
  const char* configfile = "testcase/016_allow.conf";
  Config config_;
  parseConfig(configfile, config_);  
  {
    // 0033
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0034
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::HEAD;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0035
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::POST;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0036
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::PUT;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0037
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::DELETE;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0038
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::CONNECT;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0039
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::OPTIONS;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0040
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::TRACE;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0041
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0/index.html", Response.targetFilePath);
    }
    // 0042
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::POST;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: GET\r\n.*"));
    }
    // 0042
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::POST;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: GET\r\n.*"));
    }
    // 0043
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::HEAD;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: GET\r\n.*"));
    }
    // 0044
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::PUT;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: GET\r\n.*"));
    }
    // 0045
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::DELETE;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: GET\r\n.*"));
    }
    // 0046
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::CONNECT;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: GET\r\n.*"));
    }
    // 0047
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::OPTIONS;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: GET\r\n.*"));
    }
    // 0048
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::TRACE;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/get/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: GET\r\n.*"));
    }
    // 0049
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/head/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: HEAD\r\n.*"));
    }
    // 0050
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/put/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: PUT\r\n.*"));
    }
    // 0051
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/delete/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: DELETE\r\n.*"));
    }
    // 0052
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/connect/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: CONNECT\r\n.*"));
    }
    // 0053
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/options/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: OPTIONS\r\n.*"));
    }
    // 0054
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/trace/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: TRACE\r\n.*"));
    }
    // 0055
    {
      Client client_;
      client_.port = 5000;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::PUT;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/getheadpost/";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(405, ResponseStatus);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nAllow: POST,HEAD,GET\r\n.*"));
    }
  }
}

TEST(Response_test, AcceptLanguage)
{
  const char* configfile = "testcase/017_acceptlanguage.conf";
  Config config_;
  parseConfig(configfile, config_);  
  {
    // 0057
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "en;q=0.8, ja;q=0.9, *";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0058
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/index.html";
      client_.hmp.headers_["accept-language"] = "ja";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nContent-Language: ja\r\n.*"));
    }
  }
  {
    // 0059
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nContent-Language: ja\r\n.*"));
    }
  }
  {
    // 0060
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "*";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0061
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "en";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.en", Response.targetFilePath);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nContent-Language: en\r\n.*"));
    }
  }
  {
    // 0062
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja, en;q=0.9";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nContent-Language: ja\r\n.*"));
    }
  }
  {
    // 0063
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "en;q=0.9";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.en", Response.targetFilePath);
      std::string &ResponseMessage = Response.responseMessege;
      EXPECT_THAT(ResponseMessage, MatchesRegex(".*\r\nContent-Language: en\r\n.*"));
    }
  }
  {
    // 0064
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "fr";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(406, ResponseStatus);
      EXPECT_EQ("", Response.targetFilePath);
    }
  }
  {
    // 0065
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "en;q=0.9, ja";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0066
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "fr, ja";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0067
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "fr, *";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0068
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "fr, ja, en";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0069
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=1.000";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0070
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=1.00";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0071
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=1.0";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0072
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=1.";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0073
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=1";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0074
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=1.0000";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0075
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=1.2";
      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0076
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=2";
      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0077
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja,";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0078
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=0.999";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0079
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=0.99";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0080
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=0.9";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0081
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=0.";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0082
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=0";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0083
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "zz";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(406, ResponseStatus);
    }
  }
  {
    // 0084
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=2, en";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0085
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=2, en,";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0086
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=2, en;,";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html", Response.targetFilePath);
    }
  }
  {
    // 0087
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=0.11, en;q=0.11, *;q=0.11";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0088
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "ja;q=0.111, en;q=0.11, *;q=0.1";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/acceptlanguage/index.html.ja", Response.targetFilePath);
    }
  }
  {
    // 0089
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["accept-language"] = "fr,de;q=0.5,el;q=0.3";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(406, ResponseStatus);
    }
  }
}

TEST(Response_test, Authorization)
{
  const char* configfile = "testcase/018_basicauth.conf";
  Config config_;
  parseConfig(configfile, config_);
  {
    // 0090
    {
      Client client_;
      client_.port = 8080;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.headers_["authorization"] = "Basic dXNlcjpwYXNzd29yZA==";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/base/index.html", Response.targetFilePath);
    }
  }
  {
    // 0091
    {
      Client client_;
      client_.port = 8081;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.headers_["authorization"] = "Basic dXNlcjpwYXNzd29yZA==";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/base/index.html", Response.targetFilePath);
    }
  }
  {
    // 0092
    {
      Client client_;
      client_.port = 8082;
      client_.host = "*";
      client_.hmp.method_ = httpMessageParser::GET;
      client_.hmp.absolutePath_ = "/";
      client_.hmp.headers_["host"] = "127.0.0.1";
      client_.hmp.headers_["authorization"] = "Basic dXNlcjrjgYI=";

      Response Response(client_, config_);
      int ResponseStatus = Response.ResponseStatus;

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/base/index.html", Response.targetFilePath);
    }
  }  
}