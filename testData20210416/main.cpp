#include "parseConfig.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "HTTPMessageParser.hpp"

/*
clang++ -fsanitize=address -g parseConfig.cpp Config.cpp HTTPMessageParser.cpp Client.cpp ft_*.cpp main.cpp -std=c++98
./a.out [ファイル名]

./a.out testcase/webserv.conf
  HTTPMessageParser hmp(1); ～ HTTPMessageParser hmp(12);
  testcase/webserv_result.txt

./a.out testcase/012_webserv_serverNames.conf
  HTTPMessageParser hmp("serverNames", 1); ～ HTTPMessageParser hmp("serverNames", 5);
  testcase/012_webserv_serverNames_result.txt


*/

int main(int argc, char* argv[])
{
  if (argc == 1)
    return 1;
  Client client;
  Config c;
  parseConfig(argv[1], c);
  HTTPMessageParser hmp(1);
  // HTTPMessageParser hmp("serverNames", 1);
  return 0;
}