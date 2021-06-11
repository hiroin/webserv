#include "Config.hpp"
#include <iostream>

s_ConfigCommon::s_ConfigCommon()
{
  clientMaxBodySize = CLIENTMAXBODYSIZE;
  for (int i = 0; i < METHOD_NUM; i++)
  {
    allowMethodsBool[i] = true;
  }
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
  std::cout << " allowMethodsBool  : " ;
  for(int i = 0; i < METHOD_NUM; i++)
  {
    std::cout << c.allowMethodsBool[i] << " " ;
  }
  std::cout << std::endl;
  std::cout << " authBasicRealm    : " << c.authBasicRealm << std::endl;
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
  std::cout << " rewrite           : " << std::endl;
  for(std::map<std::string, std::string>::iterator itr = c.rewrite.begin();
      itr != c.rewrite.end();
      ++itr)
  {
    std::cout << "                     ";
    std::cout << itr->first << " -> " << itr->second << std::endl;
  }
  std::cout << " uploadPath        : " << c.uploadPath << std::endl;
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
  std::cout << " cgitesterPath     : " << configGlobal.cgitesterPath << std::endl;
  printCommonConfig(configGlobal.configCommon);
  std::cout << std::endl;

  unsigned int i = 0;
  for(std::vector<s_ConfigServer>::iterator itrServer = configGlobal.servers.begin();
      itrServer != configGlobal.servers.end();
      ++itrServer)
  {
    std::cout << "サーバー設定(" << i << ")" << std::endl;
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
    i++;
    std::cout << std::endl;
  }
}

void Config::setDebugLevel(int level)
{
  debugLevel_ = level;
}

int Config::getDebugLevel() const
{
  return debugLevel_;
}

Config::Config() : debugLevel_(0)
{

};

Config::~Config()
{

};

bool s_ConfigCommon::operator==(const s_ConfigCommon& other) const
{
  if (autoindex == other.autoindex
    && allowMethods == other.allowMethods
    && authBasicRealm == other.authBasicRealm
    && authBasicUid == other.authBasicUid
    && authBasicPassword == other.authBasicPassword
    && cgiScripts == other.cgiScripts
    && clientMaxBodySize == other.clientMaxBodySize
    && errorPages == other.errorPages
    && indexs == other.indexs
  )
  {
    return true;
  }
  return false;
}

bool s_ConfigCommon::operator!=(const s_ConfigCommon& other) const
{
  if (*this == other)
    return false;
  return true;
}

bool s_ConfigLocation::operator==(const s_ConfigLocation& other) const
{
  if (path == other.path
    && alias == other.alias
    && configCommon == other.configCommon
  )
  {
    return true;
  }
  return false;
}

bool s_ConfigLocation::operator!=(const s_ConfigLocation& other) const
{
  if (*this == other)
    return false;
  return true;
}

bool s_ConfigServer::operator==(const s_ConfigServer& other) const
{
  if (host == other.host
    && port == other.port
    && root == other.root
    && serverNames == other.serverNames
    && configCommon == other.configCommon
    && locations == other.locations
  )
  {
    return true;
  }
  return false;
}

bool s_ConfigServer::operator!=(const s_ConfigServer& other) const
{
  if (*this == other)
    return false;
  return true;
}