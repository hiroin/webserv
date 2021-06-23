#include <gtest/gtest.h>
#include "../HTTPMessageParser.hpp"

// googletestのインストール手順
// https://rinatz.github.io/cpp-book/test-how-to-gtest/
// g++ -std=c++11 ../HTTPMessageParser.cpp HTTPMessageParser_test.cpp -o test -L/usr/local/lib -pthread -lgtest -lgtest_main

TEST(HTTPMessageParserEvenTest, parseRequestLine) {

  HTTPMessageParser c;
  EXPECT_EQ(200, c.parseRequestLine("GET / HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("HEAD / HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("POST / HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("PUT /putdata.html HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("DELETE / HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("CONNECT / HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("OPTIONS / HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("TRACE / HTTP/1.1"));

  EXPECT_EQ(505, c.parseRequestLine("GET / HTTP/1.0"));
  EXPECT_EQ(505, c.parseRequestLine("GET test HTTP/1.0"));

  EXPECT_EQ(501, c.parseRequestLine("OTHER / HTTP/1.1"));
  EXPECT_EQ(505, c.parseRequestLine("GET / HTTP/0.9"));
  EXPECT_EQ(400, c.parseRequestLine("GET /  "));
  EXPECT_EQ(400, c.parseRequestLine("GET / "));
  EXPECT_EQ(400, c.parseRequestLine("GET /"));
  EXPECT_EQ(400, c.parseRequestLine("GET "));
  EXPECT_EQ(400, c.parseRequestLine("GET"));
  EXPECT_EQ(400, c.parseRequestLine(" GET / HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("GET /00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.html HTTP/1.1"));

  EXPECT_EQ(400, c.parseRequestLine("PUT / HTTP/1.1"));
  EXPECT_EQ(200, c.parseRequestLine("PUT /i HTTP/1.1"));
}

TEST(HTTPMessageParserEvenTest, getAfterParseRequestLine) {

  HTTPMessageParser c;
  c.parseRequestLine("GET / HTTP/1.1");
  EXPECT_EQ(0, c.getMethod());
  EXPECT_EQ("/", c.getRequestTarget());
  EXPECT_EQ("HTTP/1.1", c.getHTTPVersion());
}

TEST(HTTPMessageParserEvenTest, parseRequestTarget) {

  HTTPMessageParser c;
  EXPECT_TRUE(c.parseRequestTarget("/"));
  EXPECT_TRUE(c.parseRequestTarget("/?"));
  EXPECT_TRUE(c.parseRequestTarget("/index.html"));
  EXPECT_TRUE(c.parseRequestTarget("/index.php"));
  EXPECT_TRUE(c.parseRequestTarget("/index.php?"));
  EXPECT_TRUE(c.parseRequestTarget("/index.php?name=hkamiya"));
  EXPECT_TRUE(c.parseRequestTarget("/index.cgi"));
  EXPECT_TRUE(c.parseRequestTarget("/www/index.html"));
  EXPECT_TRUE(c.parseRequestTarget("/www/index.php?"));
  EXPECT_TRUE(c.parseRequestTarget("/www/index.php?name=hkamiya"));
  EXPECT_TRUE(c.parseRequestTarget("http://127.0.0.1/"));
  EXPECT_TRUE(c.parseRequestTarget("http://127.0.0.1/index.html"));
  EXPECT_TRUE(c.parseRequestTarget("http://127.0.0.1/index.php?"));
  EXPECT_TRUE(c.parseRequestTarget("http://127.0.0.1/index.php?name=hkamiya"));

  EXPECT_FALSE(c.parseRequestTarget(""));
  EXPECT_FALSE(c.parseRequestTarget("index.html"));
  EXPECT_FALSE(c.parseRequestTarget("://127.0.0.1/"));
  EXPECT_FALSE(c.parseRequestTarget("http:///"));
}

TEST(HTTPMessageParserEvenTest, getAfterParseRequestTarget) {
  {
    HTTPMessageParser c;
    c.parseRequestLine("GET / HTTP/1.1");
    c.parseRequestTarget(c.getRequestTarget());
    EXPECT_EQ("/", c.getAbsolutePath());
    EXPECT_EQ("", c.getQuery());
  }
  {
    HTTPMessageParser c;
    c.parseRequestLine("GET /? HTTP/1.1");
    c.parseRequestTarget(c.getRequestTarget());
    EXPECT_EQ("/", c.getAbsolutePath());
    EXPECT_EQ("", c.getQuery());
  }
  {
    HTTPMessageParser c;
    c.parseRequestLine("GET /?name=hkamiya HTTP/1.1");
    c.parseRequestTarget(c.getRequestTarget());
    EXPECT_EQ("/", c.getAbsolutePath());
    EXPECT_EQ("name=hkamiya", c.getQuery());
  }
  {
    HTTPMessageParser c;
    c.parseRequestLine("GET /index.php?name=hkamiya HTTP/1.1");
    c.parseRequestTarget(c.getRequestTarget());
    EXPECT_EQ("/index.php", c.getAbsolutePath());
    EXPECT_EQ("name=hkamiya", c.getQuery());
  }
  {
    HTTPMessageParser c;
    c.parseRequestLine("GET /www/index.php?name=hkamiya HTTP/1.1");
    c.parseRequestTarget(c.getRequestTarget());
    EXPECT_EQ("/www/index.php", c.getAbsolutePath());
    EXPECT_EQ("name=hkamiya", c.getQuery());
    EXPECT_EQ("", c.getPathinfo());
  }
  {
    HTTPMessageParser c;
    c.parseRequestLine("GET /www/index.php/id/ap2c9w?name=hkamiya HTTP/1.1");
    c.parseRequestTarget(c.getRequestTarget());
    EXPECT_EQ("/www/index.php", c.getAbsolutePath());
    EXPECT_EQ("name=hkamiya", c.getQuery());
    EXPECT_EQ("id/ap2c9w", c.getPathinfo());
  }
}

TEST(HTTPMessageParserEvenTest, parseHeader) {
  {
    HTTPMessageParser c;
    EXPECT_TRUE(c.parseHeader("Host: 127.0.0.1"));
    EXPECT_TRUE(c.parseHeader("User-Agent: curl/7.58.0"));
    EXPECT_TRUE(c.parseHeader("Accept: */*"));

    std::map<std::string, std::string> headers = c.getHeaders();
    EXPECT_EQ("127.0.0.1", headers["host"]);
    EXPECT_EQ("curl/7.58.0", headers["user-agent"]);
    EXPECT_EQ("*/*", headers["accept"]);
  }
  {
    HTTPMessageParser c;
    EXPECT_TRUE(c.parseHeader("Host:\t 127.0.0.1\t "));

    std::map<std::string, std::string> headers = c.getHeaders();
    EXPECT_EQ("127.0.0.1", headers["host"]);
  }
  {
    HTTPMessageParser c;
    EXPECT_TRUE(c.parseHeader("User-Agent: Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1)"));

    std::map<std::string, std::string> headers = c.getHeaders();
    EXPECT_EQ("Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1)", headers["user-agent"]);
  }
  {
    HTTPMessageParser c;
    EXPECT_TRUE(c.parseHeader("User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.99 Safari/537.36"));
  }

  {
    HTTPMessageParser c;
    
    EXPECT_FALSE(c.validationHeader("Host : 127.0.0.1"));
    EXPECT_EQ(200, c.parseHeader("Host : 127.0.0.1"));
  }
  {
    HTTPMessageParser c;
    EXPECT_FALSE(c.validationHeader(" Host: 127.0.0.1"));
    EXPECT_EQ(200, c.parseHeader(" Host: 127.0.0.1"));
  }
  {
    HTTPMessageParser c;
    EXPECT_EQ(200, c.parseHeader("Host: 127.0.0.1"));
    EXPECT_EQ(400, c.parseHeader("Host: 127.0.0.1"));
  }
  {
    HTTPMessageParser c;
    EXPECT_TRUE(c.parseHeader("Accept: */*"));
    EXPECT_TRUE(c.parseHeader("Accept: */*"));
    std::map<std::string, std::string> headers = c.getHeaders();
    EXPECT_EQ("*/*,*/*", headers["accept"]);
  }
}

TEST(HTTPMessageParserEvenTest, isIllegalValueOfHostHeader) {
  {
    HTTPMessageParser c;
    EXPECT_TRUE(c.isIllegalValueOfHostHeader("Host::"));
    EXPECT_TRUE(c.isIllegalValueOfHostHeader("host::"));
    EXPECT_TRUE(c.isIllegalValueOfHostHeader("Host:"));
    EXPECT_TRUE(c.isIllegalValueOfHostHeader("Host: "));
    EXPECT_TRUE(c.isIllegalValueOfHostHeader("Host:   "));
    EXPECT_TRUE(c.isIllegalValueOfHostHeader("Host: /"));
    EXPECT_FALSE(c.isIllegalValueOfHostHeader("Host: 127.0.0.1:5000"));
    EXPECT_FALSE(c.isIllegalValueOfHostHeader("host: 127.0.0.1:5000"));
    EXPECT_FALSE(c.isIllegalValueOfHostHeader("host: 127.0.0.1:5000:5000"));
  }
}

TEST(HTTPMessageParserEvenTest, correctHostValue) {
  {
    HTTPMessageParser c;
    EXPECT_TRUE(c.correctHostValue("127.0.0.1:5000"));
    EXPECT_TRUE(c.correctHostValue("127.0.0.1::"));
    EXPECT_TRUE(c.correctHostValue("www.yahoo.co.jp:5000"));
    EXPECT_TRUE(c.correctHostValue("www.yahoo.co.jp::"));
    EXPECT_FALSE(c.correctHostValue(":5000"));
    EXPECT_FALSE(c.correctHostValue(":"));
  }
}

TEST(HTTPMessageParserEvenTest, correctUrihost) {
  {
    HTTPMessageParser c;
    EXPECT_TRUE(c.correctUrihost("127.0.0.1"));
    EXPECT_TRUE(c.correctUrihost("www.yahoo.co.jp"));
    EXPECT_TRUE(c.correctUrihost("-._~!$&'()*+,;="));
    EXPECT_FALSE(c.correctUrihost("\""));
    EXPECT_FALSE(c.correctUrihost("#"));
    EXPECT_FALSE(c.correctUrihost("/"));
    EXPECT_FALSE(c.correctUrihost(":"));
    EXPECT_FALSE(c.correctUrihost(":"));
    EXPECT_FALSE(c.correctUrihost("<"));
    EXPECT_FALSE(c.correctUrihost(">"));
    EXPECT_FALSE(c.correctUrihost("?"));
    EXPECT_FALSE(c.correctUrihost("@"));
    EXPECT_FALSE(c.correctUrihost("["));
    EXPECT_FALSE(c.correctUrihost("]"));
    EXPECT_FALSE(c.correctUrihost("^"));
    EXPECT_FALSE(c.correctUrihost("`"));
    EXPECT_FALSE(c.correctUrihost("{"));
    EXPECT_FALSE(c.correctUrihost("|"));
    EXPECT_FALSE(c.correctUrihost("}"));
  }
}

TEST(HTTPMessageParserEvenTest, isInvalidHeaderValue) {

  HTTPMessageParser c;
  c.headers_["content-length"] = "3495";
  EXPECT_EQ(200, c.isInvalidHeaderValue());
  c.headers_.clear();
  c.headers_["content-length"] = "a";
  EXPECT_EQ(400, c.isInvalidHeaderValue());
  c.headers_.clear();
  c.headers_["transfer-encoding"] = "chunked";
  EXPECT_EQ(200, c.isInvalidHeaderValue());
  c.headers_.clear();
  c.headers_["transfer-encoding"] = "a";
  EXPECT_EQ(501, c.isInvalidHeaderValue());
  c.headers_.clear();
}

TEST(HTTPMessageParserEvenTest, isInvalidHeader) {

  HTTPMessageParser c;
  c.headers_["host"] = "127.0.0.1";
  c.headers_["content-length"] = "3495";
  EXPECT_EQ(200, c.isInvalidHeader());
  c.headers_.clear();
  c.headers_["host"] = "127.0.0.1";
  c.headers_["content-length"] = "a";
  EXPECT_EQ(400, c.isInvalidHeader());
  c.headers_.clear();
  c.headers_["host"] = "127.0.0.1";
  c.headers_["transfer-encoding"] = "chunked";
  EXPECT_EQ(200, c.isInvalidHeader());
  c.headers_.clear();
  c.headers_["host"] = "127.0.0.1";
  c.headers_["transfer-encoding"] = "a";
  EXPECT_EQ(501, c.isInvalidHeader());
  c.headers_.clear();
  c.headers_["content-length"] = "3495";
  EXPECT_EQ(400, c.isInvalidHeader());
  c.headers_.clear();

}