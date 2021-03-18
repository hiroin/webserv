#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>
#include <string>
#include <map>

enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER, METHOD_NUM};

struct s_ConfigCommon
{
  int clientMaxBodySize;
  std::map<std::string, std::string> errorPages;
  bool allowMethods[METHOD_NUM];
  bool autoindex;
  std::string index;
  std::string authBasicUid;
  std::string authBasicPassword;
  std::vector<std::string> cgiScripts;
};

struct s_ConfigLocation
{
  std::string path;
  s_ConfigCommon configCommon;
};

struct s_ConfigServer
{
  std::string host;
  int port;
  std::string root;
  std::vector<std::string> serverNames;
  std::vector<s_ConfigLocation> locations;
  s_ConfigCommon configCommon;
};

struct s_ConfigGlobal
{
  std::string phpCgiPath;
  std::vector<s_ConfigServer> servers;
  s_ConfigCommon configCommon;
};

class Config
{
 public:
  s_ConfigGlobal configGlobal;
  Config();
  ~Config();

 private:
  Config(const Config&);
  Config& operator=(const Config&);  

 private:
};

#endif