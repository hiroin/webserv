#include "Config.hpp"
#include <iostream>

Config::Config(int testPattern)
{
  if (testPattern == 1)
  {
    s_ConfigServer tmpConfigServer;
    s_ConfigLocation tmpConfigLocation;

    configGlobal.servers.push_back(tmpConfigServer);
    configGlobal.servers[0].root = "/tmp/webserv0";
    configGlobal.servers[0].configCommon.index = "index.html";
    configGlobal.servers[0].locations.push_back(tmpConfigLocation);
    configGlobal.servers[0].locations[0].path = "/aaa";
    configGlobal.servers[0].locations[0].alias = "/tmp/webserv0aaa";
    configGlobal.servers[0].locations[0].configCommon.index = "default.html";
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
}

Config::Config()
{

};

Config::~Config()
{

};