#include <gtest/gtest.h>
#include "../Client.hpp"
#include "../Config.hpp"
#include "../Response.hpp"
#include "../parseConfig.hpp"

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
      int ErrorPageFd = Response.getErrorFileFd();
      size_t ErrorPageSize = Response.getErrorContentLength();
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
      EXPECT_EQ("/tmp/webserv/0/www/aaa/index.html", Response.targetFilePath);
      EXPECT_EQ("/tmp/webserv/0/404.html", Response.errorFilePath);
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
      // Client client_;
      // client_.port = 5001;
      // client_.host = "*";
      // client_.hmp.method_ = httpMessageParser::GET;
      // client_.hmp.absolutePath_ = "/www/bb/index.html";
      // client_.hmp.headers_["host"] = "127.0.0.1";

      // Response Response(client_, config_);
      // int TargetFileFd = Response.getTargetFileFd();
      // int ResponseStatus = Response.ResponseStatus;

      // EXPECT_EQ(404, ResponseStatus);
      // EXPECT_EQ("/tmp/webserv/1/www/bb/index.html", Response.targetFilePath);
      // EXPECT_EQ("/tmp/webserv/0/404.html", Response.errorFilePath);
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
      EXPECT_EQ("/tmp/webserv/0/none", Response.targetFilePath);
      EXPECT_EQ("/tmp/webserv/0/404.html", Response.errorFilePath);
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
      EXPECT_EQ("/tmp/webserv/0aaa/none", Response.targetFilePath);
      EXPECT_EQ("/tmp/webserv/0/404.html", Response.errorFilePath);
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
      EXPECT_EQ("/tmp/webserv/0aa/none", Response.targetFilePath);
      EXPECT_EQ("/tmp/webserv/0/404.html", Response.errorFilePath);
    }
    // 0021
    {
      // Client client_;
      // client_.port = 5000;
      // client_.host = "*";
      // client_.hmp.method_ = httpMessageParser::GET;
      // client_.hmp.absolutePath_ = "/root.html";
      // client_.hmp.headers_["host"] = "127.0.0.1";

      // Response Response(client_, config_);
      // int TargetFileFd = Response.getTargetFileFd();
      // int ResponseStatus = Response.ResponseStatus;

      // EXPECT_EQ(403, ResponseStatus);
      // EXPECT_EQ("/tmp/webserv/0/root.html", Response.targetFilePath);
      // EXPECT_EQ("", Response.errorFilePath);
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
      EXPECT_EQ("/tmp/webserv/1/none", Response.targetFilePath);
      EXPECT_EQ("", Response.errorFilePath);
    }
    // 0023
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
      EXPECT_EQ("/tmp/webserv/1/bbb/none", Response.targetFilePath);
      EXPECT_EQ("", Response.errorFilePath);
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
      EXPECT_EQ("/tmp/webserv/0/default.html", Response.targetFilePath);
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
      EXPECT_EQ("/tmp/webserv/0aaa/002.html", Response.targetFilePath);
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

      EXPECT_EQ(200, ResponseStatus);
      EXPECT_EQ("/tmp/webserv/0aa/default.html", Response.targetFilePath);
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
      EXPECT_EQ("/tmp/webserv/1/bbb/index.html", Response.targetFilePath);
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