#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>
#include <string>
#include <map>

#define CLIENTMAXBODYSIZE 1048576;

struct s_ConfigCommon
{
  std::string autoindex;
  std::vector<std::string> allowMethods;
  std::string authBasicUid;
  std::string authBasicPassword;
  std::vector<std::string> cgiScripts;
  int clientMaxBodySize;
  std::map<std::string, std::string> errorPages;
  std::vector<std::string> indexs;
  s_ConfigCommon();
};

struct s_ConfigLocation
{
  std::string path;
  std::string alias;
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
  void printConfig(); // Output for debugging
  Config();
  Config(int testPattern);
  ~Config();

 private:
  Config(const Config&);
  Config& operator=(const Config&);
  void printCommonConfig(s_ConfigCommon& c) const;
  void printLocationConfig(s_ConfigLocation& c) const;
  void printServerConfig(s_ConfigServer& c) const;

 private:
};

#endif