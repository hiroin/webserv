#ifndef _CONFIGSERVER_H_
#define _CONFIGSERVER_H_

#include <vector>
#include <string>
//#include "ConfigLocation.hpp"

class ConfigServer
{
 public:
  std::string host;
  int port;
  std::string root;
  std::vector<std::string> serverNames;
  // std::vector<ConfigLocation> locations;
  ConfigServer();
  ~ConfigServer();
	ConfigServer(const ConfigServer&);
	ConfigServer& operator=(const ConfigServer&);  

 private:
};

#endif