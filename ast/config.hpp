#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <iostream>
#include <map>
#include <vector>

namespace config {
  enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER, NUM};
}

struct values
{
  std::vector<std::string> value;
};

struct context
{
  std::string key;
  std::vector<values> values;
};

class configBase
{
 public:
  std::vector<context> contexts;
};

class configLocation : public configBase
{
 public:
  std::string path;
};

class configServer : public configBase
{
 public:
  std::vector<configLocation> locations;
};

class configHttp : public configBase
{
 public:
  std::vector<configServer> servers;
};

#endif