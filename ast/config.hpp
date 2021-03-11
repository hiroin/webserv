#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <iostream>
#include <map>
#include <vector>

class configBase
{
 public:
  std::map<std::string, std::vector<std::string> > contexts;
};

class configLocation : public configBase
{
  // std::string allow_method;
};

class configServer : public configBase
{
 public:
  // std::string listen;
  // std::string root;
  std::map<std::string, configLocation> locations;
};

class configHttp : public configBase
{
 public:
  // std::string client_max_body_size;
  // std::string error_page;
  std::vector<configServer> servers;
};

#endif