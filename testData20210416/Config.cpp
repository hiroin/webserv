#include "Config.hpp"
#include <iostream>

s_ConfigCommon::s_ConfigCommon()
{
  clientMaxBodySize = -1;
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

Config::Config()
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

Config::Config(int testPattern)
{
  if (testPattern == 1)
  {
    s_ConfigServer tmpConfigServer;
    s_ConfigLocation tmpConfigLocation;

    configGlobal.cgitesterPath = "/Users/iwasayoshiki/webserv/testData20210416/cgi_tester";
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[0].root = "/tmp/webserv/0";
    configGlobal.servers[0].configCommon.indexs.push_back("index.html");
    configGlobal.servers[0].configCommon.errorPages["404"] = "/404.html";
    configGlobal.servers[0].locations.push_back(tmpConfigLocation);
    configGlobal.servers[0].locations[0].path = "/aaa";
    configGlobal.servers[0].locations[0].alias = "/tmp/webserv/0aaa";
    configGlobal.servers[0].locations[0].configCommon.indexs.push_back("default.html");
    configGlobal.servers[0].locations[0].configCommon.errorPages["404"] = "/404.html";
    configGlobal.servers[0].locations.push_back(tmpConfigLocation);
    configGlobal.servers[0].locations[1].path = "/aa";
    configGlobal.servers[0].locations[1].alias = "/tmp/webserv/0aa";
    configGlobal.servers[0].locations[1].configCommon.indexs.push_back("index.htm");
    configGlobal.servers[0].locations[1].configCommon.errorPages["404"] = "/404.html";
    configGlobal.servers[0].locations.push_back(tmpConfigLocation);
    configGlobal.servers[0].locations[2].path = "/";
    configGlobal.servers[0].locations[2].configCommon.indexs.push_back("index.htm");
    configGlobal.servers[0].locations[2].configCommon.errorPages["404"] = "/404.html";
    configGlobal.servers[0].locations[2].configCommon.uploadPath = "/tmp/webserv/0";
    configGlobal.servers[0].locations[2].configCommon.cgiScripts.push_back(".bla");
    configGlobal.servers[0].locations[2].configCommon.clientMaxBodySize = 10000;
    configGlobal.servers[0].host = "*";
    configGlobal.servers[0].port = 5000;
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[1].root = "/tmp/webserv/1/";
    configGlobal.servers[1].locations.push_back(tmpConfigLocation);
    configGlobal.servers[1].locations[0].path = "/bbb/";
    configGlobal.servers[1].locations[0].alias = "/tmp/webserv/0/bbb/";
    configGlobal.servers[1].locations.push_back(tmpConfigLocation);
    configGlobal.servers[1].locations[1].path = "/bb/";
    configGlobal.servers[1].locations[1].alias = "/tmp/webserv/0/bb/";
    configGlobal.servers[1].locations.push_back(tmpConfigLocation);
    configGlobal.servers[1].locations[2].path = "/";
    configGlobal.servers[1].host = "*";
    configGlobal.servers[1].port = 5001;
    configGlobal.servers.push_back(tmpConfigServer);
  }

  if (testPattern == 10)
  {
    s_ConfigServer tmpConfigServer;
    s_ConfigLocation tmpConfigLocation;

    configGlobal.phpCgiPath = "/usr/bin/php-cgi";
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[0].root = "/tmp/webserv/base";
    configGlobal.servers[0].host = "*";
    configGlobal.servers[0].port = 5000;
  }

  if (testPattern == 11)
  {
    s_ConfigServer tmpConfigServer;
    s_ConfigLocation tmpConfigLocation;

    configGlobal.phpCgiPath = "/usr/bin/php-cgi";
    configGlobal.configCommon.allowMethods.push_back("GET");
    configGlobal.configCommon.allowMethods.push_back("HEAD");
    configGlobal.configCommon.allowMethods.push_back("POST");
    configGlobal.configCommon.allowMethods.push_back("PUT");
    configGlobal.configCommon.allowMethods.push_back("DELETE");
    configGlobal.configCommon.allowMethods.push_back("CONNECT");
    configGlobal.configCommon.allowMethods.push_back("OPTIONS");
    configGlobal.configCommon.allowMethods.push_back("TRACE");
    configGlobal.configCommon.authBasicUid  = "user";
    configGlobal.configCommon.authBasicPassword  = "pasword";
    configGlobal.configCommon.cgiScripts.push_back(".cgi");
    configGlobal.configCommon.cgiScripts.push_back(".php");
    configGlobal.configCommon.clientMaxBodySize = 1048576;
    configGlobal.configCommon.errorPages["400"] = "/40x.html";
    configGlobal.configCommon.errorPages["403"] = "/40x.html";
    configGlobal.configCommon.errorPages["404"] = "/40x.html";
    configGlobal.configCommon.indexs.push_back("index.html");
    configGlobal.configCommon.indexs.push_back("default.html");
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[0].root = "/tmp/webserv/globalAllitems";
    configGlobal.servers[0].host = "*";
    configGlobal.servers[0].port = 5000;
    configGlobal.servers[0].locations.push_back(tmpConfigLocation);
    configGlobal.servers[0].locations[0].path = "/autoindexon";
    configGlobal.servers[0].locations[0].configCommon.autoindex = "on";
  }

  if (testPattern == 12)
  {
    s_ConfigServer tmpConfigServer;
    s_ConfigLocation tmpConfigLocation;

    configGlobal.phpCgiPath = "/usr/bin/php-cgi";
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[0].root = "/tmp/webserv/serverNames/5000/default";
    configGlobal.servers[0].host = "*";
    configGlobal.servers[0].port = 5000;
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[1].root = "/tmp/webserv/serverNames/5000/server1x";
    configGlobal.servers[1].host = "*";
    configGlobal.servers[1].port = 5000;
    configGlobal.servers[1].serverNames.push_back("server10");
    configGlobal.servers[1].serverNames.push_back("server11");
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[2].root = "/tmp/webserv/serverNames/5001/server1x";
    configGlobal.servers[2].host = "*";
    configGlobal.servers[2].port = 5001;
    configGlobal.servers[2].serverNames.push_back("server10");
    configGlobal.servers[2].serverNames.push_back("server11");
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[3].root = "/tmp/webserv/serverNames/5001/server2x";
    configGlobal.servers[3].host = "*";
    configGlobal.servers[3].port = 5001;
    configGlobal.servers[3].serverNames.push_back("server20");
  }
}
