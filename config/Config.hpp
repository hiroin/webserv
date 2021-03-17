#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>
#include <string>
#include "ConfigServer.hpp"

class Config
{
 public:
  std::vector<std::string> cgiScripts;
  std::string phpCgiPath;
  std::vector<ConfigServer> servers;
  Config();
  ~Config();
	Config(const Config&);
	Config& operator=(const Config&);  

 private:
};

#endif