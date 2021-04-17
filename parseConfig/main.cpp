#include "parseConfig.hpp"
#include "Config.hpp"

/*
clang++ -fsanitize=address -g parseConfig.cpp Config.cpp ft_*.cpp main.cpp -std=c++98
./a.out [ファイル名]
*/

// int main(int argc, char* argv[])
// {
//   if (argc == 1)
//     return 1;
//   Config c;
//   parseConfig(argv[1], c);
//   return 0;
// }


#ifdef TEST1
int main()
{
  s_ConfigCommon sCC1;
  s_ConfigCommon sCC2;

  if (sCC1 == sCC2)
    std::cout << "equal" << std::endl;

  s_ConfigLocation sCL1;
  s_ConfigLocation sCL2;

  if (sCL1 == sCL2)
    std::cout << "equal" << std::endl;

  s_ConfigServer sCS1;
  sCS1.port = 5000;
  s_ConfigServer sCS2;
  sCS2.port = 5000;

  if (sCS1 == sCS2)
    std::cout << "equal" << std::endl;
  return 0;
}
#endif

int main()
{
  Config c1;
  const char *configFile = "testcase/001.conf";
  parseConfig((char *)configFile, c1);

  if (c1.configGlobal.configCommon == c1.configGlobal.configCommon)
    std::cout << "configGlobal.configCommon == configGlobal.configCommon" << std::endl;

  if (c1.configGlobal.servers[0].locations[0]
    == c1.configGlobal.servers[0].locations[0])
    std::cout << "servers[0].locations[0] == servers[0].locations[0]" << std::endl;

  if (c1.configGlobal.servers[0].locations[0]
    == c1.configGlobal.servers[0].locations[1])
    std::cout << "servers[0].locations[0] == servers[0].locations[1]" << std::endl;
  else
    std::cout << "servers[0].locations[0] != servers[0].locations[1]" << std::endl;

  if (c1.configGlobal.servers[0] == c1.configGlobal.servers[0])
    std::cout << "servers[0] == servers[0]" << std::endl;

  if (c1.configGlobal.servers[1] == c1.configGlobal.servers[1])
    std::cout << "servers[1] == servers[1]" << std::endl;

  if (c1.configGlobal.servers[0] == c1.configGlobal.servers[1])
    std::cout << "servers[0] == servers[1]" << std::endl;
  else
    std::cout << "servers[0] != servers[1]" << std::endl;
  
  return 0;
}