#include "parseConfig.hpp"
#include "Config.hpp"

/*
clang++ -fsanitize=address -g parseConfig.cpp Config.cpp ft_*.cpp main.cpp -std=c++98
./a.out [ファイル名]
*/

int main(int argc, char* argv[])
{
  if (argc == 1)
    return 1;
  Config c;
  parseConfig(argv[1], c);
  return 0;
}