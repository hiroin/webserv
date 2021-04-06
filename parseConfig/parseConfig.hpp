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
#include <climits>
#include "Config.hpp"

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
  ~parseConfig();
 
 private:
  bool isUsableChar(char c);
  unsigned int isContext(std::vector<parseconfig::token>& vtoken, unsigned int idx);
  bool isModule(std::vector<parseconfig::token> vtoken, unsigned int& idx); 
  void pushContext(std::vector<parseconfig::token>& vtoken, unsigned int idx, parseconfig::configBase& config);
  bool pushModule(std::vector<parseconfig::token> vtoken, unsigned int& idx, parseconfig::configBase& config);
  bool pushModuleToLocation(std::vector<parseconfig::token> vtoken, unsigned int& idx, parseconfig::configLocation& configLocation);
  bool pushModuleToServer(std::vector<parseconfig::token> vtoken, unsigned int& idx, parseconfig::configServer& configServer);
  bool pushModuleToHttp(std::vector<parseconfig::token> vtoken, unsigned int& idx, parseconfig::configHttp& configHttp);
  int stoi(std::string s);
  bool isCode(std::string s);
  void initCommonConfig(s_ConfigCommon &c);
  void insertAutoindex(std::vector<parseconfig::context>::iterator itr, std::string& autoindex);
  void insertClientMaxBodySize(std::vector<parseconfig::context>::iterator itr, int& clientMaxBodySize);
  void insertErrorPages(std::vector<parseconfig::context>::iterator itr, std::map<std::string, std::string>& errorPages);

  parseConfig(const parseConfig&);
  parseConfig& operator=(const parseConfig&);  
};


#endif