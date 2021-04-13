#include "Config.hpp"
#include <iostream>

s_ConfigCommon::s_ConfigCommon()
{
  clientMaxBodySize = -1;
}

void Config::printCommonConfig(s_ConfigCommon& c) const
{
  std::cout << " autoindex         : " << c.autoindex << std::endl;
  std::cout << " allowMethods      : " ;
  for(std::vector<std::string>::iterator itr = 
      c.allowMethods.begin();
      itr != c.allowMethods.end(); ++itr)
  {
    std::cout << *itr << " " ;
  }
  std::cout << std::endl;
  std::cout << " authBasicUid      : " << c.authBasicUid << std::endl;
  std::cout << " authBasicPassword : " << c.authBasicPassword << std::endl;
  std::cout << " cgiScripts        : " ;
  for(std::vector<std::string>::iterator itr = c.cgiScripts.begin();
      itr != c.cgiScripts.end();
      ++itr)
  {
    std::cout << *itr << " " ;
  }
  std::cout << std::endl;
  std::cout << " clientMaxBodySize : " << c.clientMaxBodySize << std::endl;
  for(std::map<std::string, std::string>::iterator itr = c.errorPages.begin();
      itr != c.errorPages.end();
      ++itr)
  {
    std::cout << " errorPages[" << itr->first << "]   : " << itr->second << std::endl;
  }
  std::cout << " indexs            : " ;
  for(std::vector<std::string>::iterator itr = c.indexs.begin();
      itr != c.indexs.end();
      ++itr)
  {
    std::cout << *itr << " " ;
  }
  std::cout << std::endl;
}

void Config::printLocationConfig(s_ConfigLocation& c) const
{
  std::cout << " alias             : " << c.alias << std::endl;
  printCommonConfig(c.configCommon);
}

void Config::printServerConfig(s_ConfigServer& c) const
{
  std::cout << " host              : " << c.host << std::endl;
  std::cout << " port              : " << c.port << std::endl;
  std::cout << " root              : " << c.root << std::endl;
  std::cout << " serverNames       : " ;
  for(std::vector<std::string>::iterator itr = c.serverNames.begin();
      itr != c.serverNames.end();
      ++itr)
  {
    std::cout << *itr << " " ;
  }
  std::cout << std::endl;
  printCommonConfig(c.configCommon);
}

void Config::printConfig()
{
  std::cout << "コンフィグ設定" << std::endl;
  std::cout << "Global" << std::endl;
  std::cout << " phpCgiPath        : " << configGlobal.phpCgiPath << std::endl;
  printCommonConfig(configGlobal.configCommon);
  std::cout << std::endl;

  unsigned int i = 0;
  for(std::vector<s_ConfigServer>::iterator itrServer = configGlobal.servers.begin();
      itrServer != configGlobal.servers.end();
      ++itrServer)
  {
    std::cout << "サーバー設定(" << i++ << ")" << std::endl;
    printServerConfig(*itrServer);
    std::cout << std::endl;
    for(std::vector<s_ConfigLocation>::iterator itrLocation = itrServer->locations.begin();
        itrLocation != itrServer->locations.end();
        ++itrLocation)
    {
      std::cout << "サーバー設定(" << i << ") ロケーション設定(" << itrLocation->path << ")" << std::endl;
      printLocationConfig(*itrLocation);
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

Config::Config()
{

};

Config::~Config()
{

};