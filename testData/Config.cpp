#include "Config.hpp"
#include <iostream>

s_ConfigCommon::s_ConfigCommon()
{
  clientMaxBodySize = -1;
}

Config::Config(int testPattern)
{
  if (testPattern == 1)
  {
    s_ConfigServer tmpConfigServer;
    s_ConfigLocation tmpConfigLocation;

    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[0].root = "/tmp/webserv0";
    configGlobal.servers[0].configCommon.indexs.push_back("index.html");
    configGlobal.servers[0].configCommon.errorPages["404"] = "/404.html"; 
    configGlobal.servers[0].locations.push_back(tmpConfigLocation);
    configGlobal.servers[0].locations[0].path = "/aaa";
    configGlobal.servers[0].locations[0].alias = "/tmp/webserv0aaa";
    configGlobal.servers[0].locations[0].configCommon.indexs.push_back("default.html");
    configGlobal.servers[0].locations.push_back(tmpConfigLocation);
    configGlobal.servers[0].locations[1].path = "/aa";
    configGlobal.servers[0].locations[1].alias = "/tmp/webserv0aa";
    configGlobal.servers[0].locations.push_back(tmpConfigLocation);
    configGlobal.servers[0].locations[2].path = "/";
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[1].root = "/tmp/webserv1/";
    configGlobal.servers[1].locations.push_back(tmpConfigLocation);
    configGlobal.servers[1].locations[0].path = "/bbb/";
    configGlobal.servers[1].locations[0].alias = "/tmp/webserv0bbb/";
    configGlobal.servers[1].locations.push_back(tmpConfigLocation);
    configGlobal.servers[1].locations[1].path = "/bb/";
    configGlobal.servers[1].locations[1].alias = "/tmp/webserv0bb/";
    configGlobal.servers[1].locations.push_back(tmpConfigLocation);
    configGlobal.servers[1].locations[2].path = "/";
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
    configGlobal.configCommon.allowMethods.push_back("POST");
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
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[1].root = "/tmp/webserv/serverNames/5001/server1x";
    configGlobal.servers[1].host = "*";
    configGlobal.servers[1].port = 5001;
    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[1].root = "/tmp/webserv/serverNames/5001/server2x";
    configGlobal.servers[1].host = "*";
    configGlobal.servers[1].port = 5001;
  }
}

Config::Config()
{

};

Config::~Config()
{

};