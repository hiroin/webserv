#include "parseConfig.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "HTTPMessageParser.hpp"

/*
clang++ -fsanitize=address -g parseConfig.cpp Config.cpp HTTPMessageParser.cpp Client.cpp ft_*.cpp main.cpp -std=c++98
clang++ -fsanitize=address -g parseConfig.cpp Config.cpp HTTPMessageParser.cpp Client.cpp ft_*.cpp main.cpp -std=c++98 -D DEBUG
./a.out [configファイル名] [clientのport/hostのテストケース番号] [HTTPMessageParserのテストケース番号]
./a.out [configファイル名] [clientのport/hostのテストケース番号] [HTTPMessageParserのテスト項目] [HTTPMessageParserのテストケース番号]

./a.out testcase/webserv.conf 1 1
  testcase/webserv_result.txt

./a.out testcase/012_webserv_serverNames.conf serverNames 1 1
  testcase/012_webserv_serverNames_result.txt
*/

int main(int argc, char* argv[])
{
  if (argc < 4)
    return 1;
  Config c;
  // c.printConfig(); // Config内容の出力
  parseConfig(argv[1], c);
  Client client;
  if (argc == 4)
    client = Client(atoi(argv[2]), atoi(argv[3]));
  if (argc == 5)
    client = Client(atoi(argv[2]), argv[3], atoi(argv[4]));

#ifdef DEBUG
  std::cout << "[DEBUG]config       : " << argv[1] << std::endl;
  std::cout << "[DEBUG]port         : " << client.port_ << std::endl;
  std::cout << "[DEBUG]host         : " << client.host_ << std::endl;
  std::cout << "[DEBUG]method       : " << client.hmp_.getMethod() << std::endl;
  std::cout << "[DEBUG]absolutePath : " << client.hmp_.getAbsolutePath() << std::endl;
  if (client.hmp_.getHeaders().size() > 0)
  {
    std::map<std::string, std::string> headers = client.hmp_.getHeaders();
    std::cout << "[DEBUG]headers" << std::endl;
    for (std::map<std::string, std::string>::iterator itr = headers.begin();
      itr != headers.end();
      itr++
    )
    {
      std::cout << "[DEBUG]  " << itr->first << " : " << itr->second << std::endl;
    }
  }
#endif
  return 0;
}

#ifdef TEST_CLIENT_EQUAL
int main()
{
  Client client1;
  Client client2;

  if (client1 == client1)
    std::cout << "client1 == client1" << std::endl;
  if (client1 == client2)
    std::cout << "client1 == client2" << std::endl;
  else
    std::cout << "client1 != client2" << std::endl;
  if (client1 != client2)
    std::cout << "client1 != client2" << std::endl;
  else
    std::cout << "client1 == client2" << std::endl;
  
  return 0;
}
#endif