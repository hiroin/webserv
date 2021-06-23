#include <gtest/gtest.h>
#include "../ft.hpp"

// googletestのインストール手順
// https://rinatz.github.io/cpp-book/test-how-to-gtest/
// g++ -std=c++11 ../HTTPMessageParser.cpp HTTPMessageParser_test.cpp -o test -L/usr/local/lib -pthread -lgtest -lgtest_main

TEST(ft, ft_split_use_find_first_of)
{
  std::vector<std::string> resultSplit = {"a","b","c"};
  EXPECT_EQ(resultSplit, ft_split_use_find_first_of("a b c", " "));
  EXPECT_EQ(resultSplit, ft_split_use_find_first_of(" a b c ", " "));
  EXPECT_EQ(resultSplit, ft_split_use_find_first_of(" a b c ", " \t"));
  EXPECT_EQ(resultSplit, ft_split_use_find_first_of(" a \tb c ", " \t"));
}
