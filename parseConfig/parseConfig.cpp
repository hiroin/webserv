#include "parseConfig.hpp"
#include "Config.hpp"

void parseConfig::lexer(std::string s)
{
  unsigned int idx = 0;
  unsigned int startIdx = 0;
  parseconfig::tokenType tt;

  while (idx < s.size())
  {
    if(s[idx] <= ' ')
    {
      idx++;
      continue;
    }
    startIdx = idx;
    if (isUsableChar(s[idx]))
    {
      while (isUsableChar(s[idx]))
      {
        idx++;
      }
      tt = parseconfig::CHAR;
    }
    else
    {
      switch (s[idx])
      {
      case ';':
        tt = parseconfig::SEMICOLON;
        break;
      case '{':
        tt = parseconfig::LB;
        break;
      case '}':
        tt = parseconfig::RB;
        break;
      default:
        tt = parseconfig::UNAUTHORIZED;
        throw std::runtime_error("Some characters cannot be used in the config file.");
        break;
      }  
      idx++;
    }
    parseconfig::token t;
    t.token = s.substr(startIdx, idx - startIdx);
    t.type = tt;
    vtoken_.push_back(t);
  }
  return ;
}

bool parseConfig::isUsableChar(char c)
{
  if (std::isalpha(c) || \
      std::isdigit(c) || \
      c == '/' || \
      c == '.' || \
      c == '_' || \
      c == '*' || \
      c == ':' || \
      c == '-')
    return true;
  return false;
}

void parseConfig::printToken()
{
  for(std::vector<parseconfig::token>::iterator itr = vtoken_.begin(); \
   itr != vtoken_.end(); ++itr)
  {
    if(itr->type != parseconfig::INITIAL)
    {
      std::cout << "token TYPE: ";
      switch(itr->type){
      case parseconfig::CHAR:
        std::cout << "CHAR         ";
        break;
      case parseconfig::SEMICOLON:
        std::cout << "SEMICOLON    ";
        break;
      case parseconfig::LB:
        std::cout << "LB           ";
        break;
      case parseconfig::RB:
        std::cout << "RB           ";
        break;
      default:
        std::cout << "UNAUTHORIZED ";
      }
      std::cout << "token: " << itr->token.c_str();
    }
    std::cout << std::endl;
  }
}

unsigned int parseConfig::isContext(std::vector<parseconfig::token>& vtoken, unsigned int idx)
{
  unsigned int i = 0;

  while (vtoken.at(idx).type == parseconfig::CHAR)
  {
    i++;
    idx++;
  }
  if (i <= 1)
    return 0;
  if (vtoken.at(idx).type == parseconfig::SEMICOLON)
  {
    i++;
    return i;
  }
  return 0;
}

bool parseConfig::isModule(std::vector<parseconfig::token> vtoken, unsigned int& idx)
{
  static int brackets = 0;
  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::CHAR && vtoken.at(idx + 1).type == parseconfig::LB)
    {
      brackets++;
      idx += 2;
      if (!isModule(vtoken, idx))
        return false;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::CHAR && vtoken.at(idx + 1).type == parseconfig::CHAR && vtoken.at(idx + 2).type == parseconfig::LB)
    {
      brackets++;
      idx += 3;
      if (!isModule(vtoken, idx))
        return false;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::RB)
    {
      brackets--;
      idx += 1;
      continue;
    }
    return false;
  }
  if (brackets != 0)
    return false;
  return true;
}

bool parseConfig::checkFormat()
{
  unsigned int idx = 0;
  return (parseConfig::isModule(vtoken_, idx));
}

void parseConfig::pushContext(std::vector<parseconfig::token>& vtoken, unsigned int idx, parseconfig::configBase& config)
{
  std::string key = vtoken.at(idx++).token;
  for(std::vector<parseconfig::context>::iterator itr = 
     config.contexts.begin(); itr != config.contexts.end(); ++itr) {
    if (key == itr->key)
    {
      parseconfig::values values;
      while (vtoken.at(idx).type == parseconfig::CHAR)
      {
        values.value.push_back(vtoken.at(idx).token);
        idx++;
      }
      itr->values.push_back(values);
      return ;
    }
  }
  parseconfig::context context;
  context.key = key;
  parseconfig::values values;
  while (vtoken.at(idx).type == parseconfig::CHAR)
  {
    values.value.push_back(vtoken.at(idx).token);
    idx++;
  }
  context.values.push_back(values);
  config.contexts.push_back(context);
}

bool parseConfig::pushModule(std::vector<parseconfig::token> vtoken, unsigned int& idx, parseconfig::configBase& config)
{
  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      pushContext(vtoken, idx, config);
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::CHAR && vtoken.at(idx + 1).type == parseconfig::LB)
    {
      idx += 2;
      if (!pushModule(vtoken, idx, config))
        return false;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::CHAR && vtoken.at(idx + 1).type == parseconfig::CHAR && vtoken.at(idx + 2).type == parseconfig::LB)
    {
      idx += 3;
      if (!pushModule(vtoken, idx, config))
        return false;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::RB)
    {
      idx += 1;
      continue;
    }
    return false;
  }
  return true;
}

bool parseConfig::pushModuleToLocation(std::vector<parseconfig::token> vtoken, unsigned int& idx, parseconfig::configLocation& configLocation)
{
  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      pushContext(vtoken, idx, configLocation);
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::RB)
    {
      idx += 1;
      return true;
    }
    return false;
  }
  return true;
}

bool parseConfig::pushModuleToServer(std::vector<parseconfig::token> vtoken, unsigned int& idx, parseconfig::configServer& configServer)
{
  unsigned int locations = 0;

  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      pushContext(vtoken, idx, configServer);
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::CHAR && vtoken.at(idx + 1).type == parseconfig::CHAR && vtoken.at(idx + 2).type == parseconfig::LB)
    {
      std::string path = vtoken.at(idx + 1).token;
      for(std::vector<parseconfig::configLocation>::iterator itr = 
          configServer.locations.begin(); itr != configServer.locations.end(); ++itr) {
        if (itr->path == path)
          return false;
      }
      parseconfig::configLocation tmp;
      tmp.path = vtoken.at(idx + 1).token;
      configServer.locations.push_back(tmp);
      idx += 3;
      if (!pushModuleToLocation(vtoken, idx, configServer.locations[configServer.locations.size() - 1]))
        return false;
      locations++;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::RB)
    {
      idx += 1;
      return true;
    }
    return false;
  }
  return true;
}

bool parseConfig::pushModuleToHttp(std::vector<parseconfig::token> vtoken, unsigned int& idx, parseconfig::configHttp& configHttp)
{
  unsigned int servers = 0;

  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      pushContext(vtoken, idx, configHttp);
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == parseconfig::CHAR && vtoken.at(idx + 1).type == parseconfig::LB)
    {
      idx += 2;
      parseconfig::configServer tmp;
      configHttp.servers.push_back(tmp);
      if (!pushModuleToServer(vtoken, idx, configHttp.servers.at(servers)))
        return false;
      servers++;
      continue;
    }
    return false;
  }
  return true;
}

bool parseConfig::insertConfigToTmpMemory()
{
  unsigned int idx = 0;
  return pushModuleToHttp(vtoken_, idx, configHttp_);  
}

void parseConfig::printConfigHttp()
{
  std::cout << "一時作業領域内容" << std::endl;
  std::cout << "グローバル設定" << std::endl;
  for(std::vector<parseconfig::context>::iterator itr = 
      configHttp_.contexts.begin(); itr != configHttp_.contexts.end(); ++itr) {
    std::cout << "  key = " << itr->key << std::endl;
    for(std::vector<parseconfig::values>::iterator itrValues = 
        itr->values.begin(); itrValues != itr->values.end(); ++itrValues) {
      std::cout << "    value = ";
      for(std::vector<std::string>::iterator itrValue = 
          itrValues->value.begin(); itrValue != itrValues->value.end(); ++itrValue) {
        std::cout << *itrValue << " ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
  unsigned int i = 1;
  for(std::vector<parseconfig::configServer>::iterator itrServer = 
      configHttp_.servers.begin(); itrServer != configHttp_.servers.end(); ++itrServer) {
    std::cout << "サーバー設定(" << i++ << ")" << std::endl;
    for(std::vector<parseconfig::context>::iterator itr =
        itrServer->contexts.begin(); itr != itrServer->contexts.end(); ++itr) {
      std::cout << "  key = " << itr->key << std::endl;
      for(std::vector<parseconfig::values>::iterator itrValues = 
          itr->values.begin(); itrValues != itr->values.end(); ++itrValues) {
        std::cout << "    value = ";
        for(std::vector<std::string>::iterator itrValue = 
            itrValues->value.begin(); itrValue != itrValues->value.end(); ++itrValue) {
          std::cout << *itrValue << " ";
        }
        std::cout << std::endl;
      }
    }
    std::cout << std::endl;
    for(std::vector<parseconfig::configLocation>::iterator itrConfig =
        itrServer->locations.begin(); itrConfig != itrServer->locations.end(); ++itrConfig) {
      std::cout << "  ロケーション設定(" << itrConfig->path << ")" << std::endl;
      for(std::vector<parseconfig::context>::iterator itr = 
          itrConfig->contexts.begin(); itr != itrConfig->contexts.end(); ++itr) {
        std::cout << "    key = " << itr->key << std::endl;
        for(std::vector<parseconfig::values>::iterator itrValues = 
            itr->values.begin(); itrValues != itr->values.end(); ++itrValues) {
          std::cout << "      value = ";
          for(std::vector<std::string>::iterator itrValue = 
              itrValues->value.begin(); itrValue != itrValues->value.end(); ++itrValue) {
            std::cout << *itrValue << " ";
          }
          std::cout << std::endl;
        }
      }
      std::cout << std::endl;
    }
  }  
}

bool parseConfig::insertToConfigClass(Config& c)
{ 
  initCommonConfig(c.configGlobal.configCommon);
  for(std::vector<parseconfig::context>::iterator itr = 
      configHttp_.contexts.begin(); itr != configHttp_.contexts.end(); ++itr)
  {
    if (itr->key == "php-cgi-path")
    {
      if (itr->values.at(0).value.size() >= 2)
        throw std::runtime_error("Config Error : invalid php-cgi-path");
      c.configGlobal.phpCgiPath = itr->values.at(0).value.at(0);
      std::cout << "php-cgi-path : " << c.configGlobal.phpCgiPath << std::endl;
    }
    insertAutoindex(itr, c.configGlobal.configCommon.autoindex);
    insertClientMaxBodySize(itr, c.configGlobal.configCommon.clientMaxBodySize);
    insertErrorPages(itr, c.configGlobal.configCommon.errorPages);
  }
  std::cout << std::endl;
  for(std::vector<parseconfig::configServer>::iterator itrServer = 
      configHttp_.servers.begin(); itrServer != configHttp_.servers.end(); ++itrServer)
  {
    size_t serverNumber = itrServer - configHttp_.servers.begin();
    std::cout << "サーバー設定(" << serverNumber << ")" << std::endl;
    s_ConfigServer tmpConfigServer;
    initCommonConfig(tmpConfigServer.configCommon);
    c.configGlobal.servers.push_back(tmpConfigServer);
    for(std::vector<parseconfig::context>::iterator itr =
        itrServer->contexts.begin(); itr != itrServer->contexts.end(); ++itr)
    {
      if (itr->key == "listen")
      {

      }
      if (itr->key == "server_name")
      {

      }
      if (itr->key == "root")
      {
        if (itr->values.at(0).value.size() >= 2)
          throw std::runtime_error("Config Error : server { root }");
        c.configGlobal.servers.at(serverNumber).root = itr->values.at(0).value.at(0);
        std::cout << "  root : " << c.configGlobal.servers.at(serverNumber).root << std::endl;
      }
      insertClientMaxBodySize(itr, c.configGlobal.servers.at(serverNumber).configCommon.clientMaxBodySize);
      insertErrorPages(itr, c.configGlobal.servers.at(serverNumber).configCommon.errorPages);
    }
    std::cout << std::endl;
    for(std::vector<parseconfig::configLocation>::iterator itrConfig =
        itrServer->locations.begin(); itrConfig != itrServer->locations.end(); ++itrConfig)
    {
      std::cout << "  ロケーション設定(" << itrConfig->path << ")" << std::endl;
      s_ConfigLocation tmpConfigLocation;
      initCommonConfig(tmpConfigLocation.configCommon);
      tmpConfigLocation.path = itrConfig->path;
      size_t i = 0;
      while (i < c.configGlobal.servers.at(serverNumber).locations.size())
      {
        if (c.configGlobal.servers.at(serverNumber).locations.at(i).path.size() < tmpConfigLocation.path.size())
          break;
        i++;
      }
      std::cout << "  ロケーション設定挿入位置 : " << i << std::endl;
      c.configGlobal.servers.at(serverNumber).locations.insert(c.configGlobal.servers.at(serverNumber).locations.begin() + i, tmpConfigLocation);
      for(std::vector<parseconfig::context>::iterator itr =
          itrConfig->contexts.begin(); itr != itrConfig->contexts.end(); ++itr)
      {
        insertErrorPages(itr, c.configGlobal.servers.at(serverNumber).locations.at(i).configCommon.errorPages);
      }
      std::cout << std::endl;
    }
  }






  return true;
}

int parseConfig::stoi(std::string s)
{
  size_t i;
  long r;

  i = 0;
  if (!std::isdigit(s[i]))
    return -1;
  if (s[i] == '0')
    return -1;
  i++;
  while (std::isdigit(s[i]))
    i++;
  if (i != s.size())
    return -1;
  i = 0;
  r = s[i++] - '0';
  while (i < s.size())
    r = r * 10 + s[i++] - '0';
  if (r > INT_MAX || r <= 0)
    return -1;
	return (r);
}

bool parseConfig::isCode(std::string s)
{
  if (s == "400" ||
    s == "401" ||
    s == "403" ||
    s == "404" ||
    s == "405" ||
    s == "413" ||
    s == "429" ||
    s == "500" ||
    s == "501" ||
    s == "503"
  )
    return true;
  return false;
}

void parseConfig::initCommonConfig(s_ConfigCommon &c)
{
  c.autoindex = false;
  for (int i = 0; i < METHOD_NUM; ++i)
    c.allowMethods[i] = true;
  c.clientMaxBodySize = CLIENTMAXBODYSIZE;
}

void parseConfig::insertAutoindex(std::vector<parseconfig::context>::iterator itr, bool& autoindex)
{
  if (itr->key == "autoindex")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplication autoindex");
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid autoindex");
    std::string autoindexValue = itr->values.at(0).value.at(0);
    if (!(autoindexValue == "off" || autoindexValue == "on"))
      throw std::runtime_error("Config Error : invalid autoindex");
    std::cout << "[DEBUG]autoindex : " << autoindexValue << std::endl;
  } 
}

void parseConfig::insertClientMaxBodySize(std::vector<parseconfig::context>::iterator itr, int& clientMaxBodySize)
{
  if (itr->key == "client_max_body_size")
  {
    // 重複して設定された場合はエラー
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplication client_max_body_size");
    // client_max_body_size 1000 100;のような場合はエラー
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid client_max_body_size");
    clientMaxBodySize = parseConfig::stoi(itr->values.at(0).value.at(0).c_str());
    if (clientMaxBodySize == -1)
      throw std::runtime_error("Config Error : invalid client_max_body_size");
    std::cout << "[DEBUG]client_max_body_size : " << clientMaxBodySize << std::endl;
  } 
}

void parseConfig::insertErrorPages(std::vector<parseconfig::context>::iterator itr, std::map<std::string, std::string>& errorPages)
{
  if (itr->key == "error_page")
  {
    for (std::vector<parseconfig::values>::iterator itrValues = 
        itr->values.begin(); itrValues != itr->values.end(); ++itrValues)
    {
      std::vector<std::string>::iterator itrValue = itrValues->value.end();
      std::string relativePath = *--itrValue;
      for (itrValue = itrValues->value.begin(); itrValue != itrValues->value.end() - 1; ++itrValue)
      {
        if (!isCode(*itrValue))
          throw std::runtime_error("Config Error : invalid error_page");
        errorPages.insert(std::make_pair(*itrValue, relativePath));
        std::cout << "[DEBUG]error_page : " << *itrValue << " " << relativePath << std::endl;
      }
    }
  }
}

parseConfig::parseConfig()
{

}

parseConfig::~parseConfig()
{

}
