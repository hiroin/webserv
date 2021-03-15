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
  public:
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
  // std::string allow_method[config::NUM];
  // bool autoindex;
  // std::string index;
  // std::vector<std::string> cgi-script;
  // std::string php-cgi-path;
  // std::string alias;
  // std::auth_basic_user;
  // std::auth_basic_password;
  std::string path;
};


class configServer : public configBase
{
 public:
  // std::string listen;
  // std::string port;
  // std::string root;
  // std::map<std::string, std::string> error_page;
  // std::vector<std::string> server_name;
  std::vector<configLocation> locations;
};

class configHttp : public configBase
{
 public:
  // unsigned int client_max_body_size;
  // std::map<std::string, std::string> error_page;
  std::vector<configServer> servers;
};

#endif