#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>
#include <string>
#include <map>

#define CLIENTMAXBODYSIZE 1048576
#define MAX_SESSION 2
#define SELECT_TIMEOUT 60
#define SESSION_TIMEOUT 10

enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER, METHOD_NUM};

struct s_ConfigCommon
{
  std::string autoindex;
  std::vector<std::string> allowMethods;
  bool allowMethodsBool[METHOD_NUM];
  std::string authBasicFile;
  std::string authBasicRealm;
  std::string authBasicUid;
  std::string authBasicPassword;
  std::vector<std::string> cgiScripts;
  int clientMaxBodySize;
  std::map<std::string, std::string> errorPages;
  std::vector<std::string> indexs;
  std::string uploadPath;
  s_ConfigCommon();
  bool operator==(const s_ConfigCommon&) const;
  bool operator!=(const s_ConfigCommon&) const;
};

struct s_ConfigLocation
{
  std::string path;
  std::string alias;
  s_ConfigCommon configCommon;
  bool operator==(const s_ConfigLocation&) const;
  bool operator!=(const s_ConfigLocation&) const;
};

struct s_ConfigServer
{
  std::string host;
  int port;
  std::string root;
  std::vector<std::string> serverNames;
  std::vector<s_ConfigLocation> locations;
  s_ConfigCommon configCommon;
  bool operator==(const s_ConfigServer&) const;
  bool operator!=(const s_ConfigServer&) const;
};

struct s_ConfigGlobal
{
  std::string phpCgiPath;
  std::string cgitesterPath;
  std::vector<s_ConfigServer> servers;
  s_ConfigCommon configCommon;
};

class Config
{
 public:
  s_ConfigGlobal configGlobal;
  void setDebugLevel(int level);
  int getDebugLevel() const;
  void printConfig(); // Output for debugging
  Config();
  Config(int testPattern);
  Config(char *configFile);
  ~Config();

 private:
  char *configFile_;
  int debugLevel_;
  Config(const Config&);
  Config& operator=(const Config&);
  void printCommonConfig(s_ConfigCommon& c) const;
  void printLocationConfig(s_ConfigLocation& c) const;
  void printServerConfig(s_ConfigServer& c) const;

 private:
};

#endif