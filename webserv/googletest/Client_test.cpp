#include <gtest/gtest.h>
#include "../Client.hpp"

// googletestのインストール手順
// https://rinatz.github.io/cpp-book/test-how-to-gtest/
// g++ -std=c++11 ../HTTPMessageParser.cpp HTTPMessageParser_test.cpp -o test -L/usr/local/lib -pthread -lgtest -lgtest_main

TEST(ClientTest, isNeedBody) {

  Client c;
  std::map<std::string, std::string> headers;
  headers["content-length"] = "3495";
  EXPECT_TRUE(c.isNeedBody(headers));
  headers.clear();

  headers["content-length"] = "3495";
  headers["transfer-encoding"] = "chunked";
  EXPECT_TRUE(c.isNeedBody(headers));
  headers.clear();

  headers["transfer-encoding"] = "chunked";
  EXPECT_TRUE(c.isNeedBody(headers));
  headers.clear();

  EXPECT_FALSE(c.isNeedBody(headers));
}
