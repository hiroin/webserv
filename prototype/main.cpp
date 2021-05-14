#include <iostream>
#include <signal.h>
#include "Config.hpp"
#include "parseConfig.hpp"
#include "Webserv.hpp"

void stop(int sig)
{
  (void)sig;
  exit(0);
}

int main(int argc, char *argv[])
{
  if (argc != 2 && argc != 3)
  {
    std::cout << "Usage: ./server [configfile]" << std::endl;
    return 1;
  }
  signal(SIGABRT, stop);
  signal(SIGINT, stop);
  signal(SIGTERM, stop);
  Config c;
  if (argc == 3 && (std::string(argv[2]) == "1" || std::string(argv[2]) == "2"))
  {
    switch (argv[2][0])
    {
    case '1':
      c.setDebugLevel(1);
      break;
    case '2':
      c.setDebugLevel(2);
      break;
    default:
      break;
    }
  }
  try
  {
    parseConfig(argv[1], c);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (c.getDebugLevel() >= 1)
    c.printConfig();
  Wevserv myserver(c);
  return 0;
}