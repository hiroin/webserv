#include "parseConfig.hpp"
#include "Config.hpp"

int main(int argc, char* argv[])
{
  if (argc == 1)
    return 1;
  Config c;
  parseConfig(argv[1], c);
	return 0;
}