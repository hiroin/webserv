#ifndef _PARSECONFIG_H_
#define _PARSECONFIG_H_

#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "Config.hpp"
#include "ft.hpp"

namespace parseconfig
{
  enum tokenType
  {
    INITIAL,
    CHAR,
    SEMICOLON,
    LB,        /* { */
    RB,        /* } */
    UNAUTHORIZED,
    END
  };

  struct token
  {
    tokenType    type;
    std::string   token;
  } ;

  struct vvalues
  {
    std::vector<std::string> value;
  };

  struct context
  {
    std::string key;
    std::vector<vvalues> values;
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
}

class parseConfig
{
 public:
  void lexer(std::string s);
  void printToken(); // Output for debugging
  bool checkFormat();
  bool insertConfigToTmpMemory();
  void printConfigHttp(); // Output for debugging
  bool insertToConfigClass(Config& c);

  std::vector<parseconfig::token> vtoken_;
  parseconfig::configHttp configHttp_;

  parseConfig();
  parseConfig(const char *configFile, Config& c);
  ~parseConfig();
 
 private:
  bool isUsableChar(char c);
  typedef std::vector<parseconfig::token> token;
  unsigned int isContext(token& vtoken, unsigned int idx);
  bool isModule(token vtoken, unsigned int& idx);
  void pushContext(token& vtoken, unsigned int idx, parseconfig::configBase& config);
  bool pushModule(token vtoken, unsigned int& idx, parseconfig::configBase& config);
  bool pushModuleToLocation(token vtoken, unsigned int& idx, parseconfig::configLocation& configLocation);
  bool pushModuleToServer(token vtoken, unsigned int& idx, parseconfig::configServer& configServer);
  bool pushModuleToHttp(token vtoken, unsigned int& idx, parseconfig::configHttp& configHttp);

  bool isCode(std::string s);
  bool isMethod(std::string s);
  bool isFilenameExtension(std::string s);
  void initCommonConfig(s_ConfigCommon &c);
  void inheritedFromHigherlevelDirectives(Config& c);
  void inheritedCommonConfig(s_ConfigCommon& higher, s_ConfigCommon& lower);
  bool notExistRootDirective(Config& c);

  typedef std::vector<parseconfig::context>::iterator contextIterator;
  bool insertAutoindex(contextIterator itr, std::string& autoindex);
  bool insertAlias(contextIterator itr, std::string& alias);
  bool insertAllowMethods(contextIterator itr, std::vector<std::string>& allowMethods, bool* allowMethodsBool);
  bool insertAuthBasicFile( \
    contextIterator itr, \
    std::string& authBasicFile, \
    std::string& authBasicRealm, \
    std::string& authBasicUid, \
    std::string& authBasicPassword \
    );
  bool insertAuthBasicInfo(contextIterator itr, std::string& authBasicUid, std::string& authBasicPassword);
  bool insertAuthBasicRealm(contextIterator itr, std::string& authBasicRealm);
  bool insertCgiScript(contextIterator itr, std::vector<std::string>& cgiScripts);
  bool insertClientMaxBodySize(contextIterator itr, int& clientMaxBodySize);
  bool insertErrorPages(contextIterator itr, std::map<std::string, std::string>& errorPages);
  bool insertIndexs(contextIterator itr, std::vector<std::string>& indexs);
  bool insertListen(contextIterator itr, std::string& host, int& port);
  bool insertRoot(contextIterator itr, std::string& root);
  bool insertServerNames(contextIterator itr, std::vector<std::string>& serverNames);

  parseConfig(const parseConfig&);
  parseConfig& operator=(const parseConfig&);  
};


#endif