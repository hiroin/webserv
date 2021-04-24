#include <gtest/gtest.h>
#include "../ft.hpp"

// googletestのインストール手順
// https://rinatz.github.io/cpp-book/test-how-to-gtest/
// g++ -std=c++11 ../HTTPMessageParser.cpp HTTPMessageParser_test.cpp -o test -L/usr/local/lib -pthread -lgtest -lgtest_main

TEST(ft, ft_stoi)
{
  EXPECT_EQ(0, ft_stoi("0"));
  EXPECT_EQ(1, ft_stoi("1"));
  EXPECT_EQ(100, ft_stoi("100"));
  EXPECT_EQ(INT_MAX, ft_stoi("2147483647"));
  EXPECT_EQ(1, ft_stoi("01"));
  EXPECT_EQ(-1, ft_stoi("-1"));
  EXPECT_EQ(-1, ft_stoi("-100"));
  EXPECT_EQ(-1, ft_stoi("2147483648"));
  EXPECT_EQ(-1, ft_stoi("9223372036854775807"));
  EXPECT_EQ(-1, ft_stoi("9223372036854775808"));
  EXPECT_EQ(-1, ft_stoi("10000000000000000000"));
}
