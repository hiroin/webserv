#include "parseConfig.hpp"
#include "Config.hpp"

void parseConfig::lexer(std::string s)
{
  unsigned int idx = 0;
  unsigned int lastIdx = 0;
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
    if (s[idx] == '"')
    {
      idx++;
      startIdx = idx;
      while (s[idx] != '"')
      {
        if (idx == s.size())
          throw std::runtime_error("Double quotation marks are not closed.");
        idx++;
      }
      lastIdx = idx;
      idx++;
      tt = parseconfig::CHAR;
    }
    else if (isUsableChar(s[idx]))
    {
      while (isUsableChar(s[idx]))
      {
        idx++;
      }
      lastIdx = idx;
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
      lastIdx = idx;
    }
    parseconfig::token t;
    t.token = s.substr(startIdx, lastIdx - startIdx);
    t.type = tt;
    vtoken_.push_back(t);
  }
  return ;
}

bool parseConfig::isUsableChar(char c)
{
  if (c <= 32 || \
      c == 127 || \
      c == ';' || \
      c == '{' || \
      c == '}')
    return false;
  return true;
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
    // std::cout << std::endl;
  }
}

unsigned int parseConfig::isContext(token& vtoken, unsigned int idx)
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

bool parseConfig::isModule(token vtoken, unsigned int& idx)
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

void parseConfig::pushContext(token& vtoken, unsigned int idx, parseconfig::configBase& config)
{
  std::string key = vtoken.at(idx++).token;
  for(std::vector<parseconfig::context>::iterator itr = 
     config.contexts.begin(); itr != config.contexts.end(); ++itr) {
    if (key == itr->key)
    {
      parseconfig::vvalues values;
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
  parseconfig::vvalues values;
  while (vtoken.at(idx).type == parseconfig::CHAR)
  {
    values.value.push_back(vtoken.at(idx).token);
    idx++;
  }
  context.values.push_back(values);
  config.contexts.push_back(context);
}

bool parseConfig::pushModule(token vtoken, unsigned int& idx, parseconfig::configBase& config)
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

bool parseConfig::pushModuleToLocation(token vtoken, unsigned int& idx, parseconfig::configLocation& configLocation)
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

bool parseConfig::pushModuleToServer(token vtoken, unsigned int& idx, parseconfig::configServer& configServer)
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

bool parseConfig::pushModuleToHttp(token vtoken, unsigned int& idx, parseconfig::configHttp& configHttp)
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
    for(std::vector<parseconfig::vvalues>::iterator itrValues = 
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
      for(std::vector<parseconfig::vvalues>::iterator itrValues = 
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
        for(std::vector<parseconfig::vvalues>::iterator itrValues = 
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
  s_ConfigCommon& tmpConfigCommon = c.configGlobal.configCommon;
  initCommonConfig(tmpConfigCommon);
  for(std::vector<parseconfig::context>::iterator itr = 
      configHttp_.contexts.begin(); itr != configHttp_.contexts.end(); ++itr)
  {
    if (itr->key == "php-cgi_path")
    {
      if (itr->values.at(0).value.size() >= 2)
        throw std::runtime_error("Config Error : invalid php-cgi_path");
      c.configGlobal.phpCgiPath = itr->values.at(0).value.at(0);
      int fp = open(c.configGlobal.phpCgiPath.c_str(), O_RDONLY);
      if (fp == -1)
        throw std::runtime_error("Config Error : cannot open php-cgi");
      close(fp);
      continue;
    }
    if (itr->key == "cgi_tester_path")
    {
      if (itr->values.at(0).value.size() >= 2)
        throw std::runtime_error("Config Error : cgi_tester_path");
      c.configGlobal.cgitesterPath = itr->values.at(0).value.at(0);
      int fp = open(c.configGlobal.cgitesterPath.c_str(), O_RDONLY);
      if (fp == -1)
        throw std::runtime_error("Config Error : cannot open cgi_tester");
      close(fp);
      continue;
    }
    if (insertAutoindex(itr, tmpConfigCommon.autoindex))
      continue;
    if (insertAllowMethods(itr, tmpConfigCommon.allowMethods, tmpConfigCommon.allowMethodsBool))
      continue;
    if (insertAuthBasicInfo(itr, tmpConfigCommon.authBasicUid, tmpConfigCommon.authBasicPassword))
      continue;
    if (insertAuthBasicRealm(itr, tmpConfigCommon.authBasicRealm))
      continue;
    if (insertCgiScript(itr, tmpConfigCommon.cgiScripts))
      continue;
    if (insertClientMaxBodySize(itr, tmpConfigCommon.clientMaxBodySize))
      continue;
    if (insertErrorPages(itr, tmpConfigCommon.errorPages))
      continue;
    if (insertIndexs(itr, tmpConfigCommon.indexs))
      continue;
    std::string errorMessage = "Config Error : invalid directive ";
    errorMessage +=  itr->key ;
    throw std::runtime_error(errorMessage);
  }
  // std::cout << std::endl;
  for(std::vector<parseconfig::configServer>::iterator itrServer = 
      configHttp_.servers.begin(); itrServer != configHttp_.servers.end(); ++itrServer)
  {
    size_t serverNumber = itrServer - configHttp_.servers.begin();
    // std::cout << "[DEBUG]サーバー設定(" << serverNumber << ")" << std::endl;
    s_ConfigServer tmpConfigServer;
    initCommonConfig(tmpConfigServer.configCommon);
    for(std::vector<parseconfig::context>::iterator itr =
        itrServer->contexts.begin(); itr != itrServer->contexts.end(); ++itr)
    {
      if (insertListen(itr, tmpConfigServer.host, tmpConfigServer.port))
        continue;
      if (insertServerNames(itr, tmpConfigServer.serverNames))
        continue;
      if (insertRoot(itr, tmpConfigServer.root))
        continue;
      if (insertAutoindex(itr, tmpConfigServer.configCommon.autoindex))
        continue;
      if (insertAllowMethods(itr, tmpConfigServer.configCommon.allowMethods, tmpConfigServer.configCommon.allowMethodsBool))
        continue;
      if (insertAuthBasicInfo(itr, tmpConfigServer.configCommon.authBasicUid, tmpConfigServer.configCommon.authBasicPassword))
        continue;
      if (insertAuthBasicRealm(itr, tmpConfigServer.configCommon.authBasicRealm))
        continue;
      if (insertCgiScript(itr, tmpConfigServer.configCommon.cgiScripts))
        continue;
      if (insertClientMaxBodySize(itr, tmpConfigServer.configCommon.clientMaxBodySize))
        continue;
      if (insertErrorPages(itr, tmpConfigServer.configCommon.errorPages))
        continue;
      if (insertIndexs(itr, tmpConfigServer.configCommon.indexs))
        continue;
      std::string errorMessage = "Config Error : invalid directive ";
      errorMessage +=  itr->key ;
      throw std::runtime_error(errorMessage);
    }
    // std::cout << std::endl;
    for(std::vector<parseconfig::configLocation>::iterator itrConfig =
        itrServer->locations.begin(); itrConfig != itrServer->locations.end(); ++itrConfig)
    {
      // std::cout << "[DEBUG]  ロケーション設定(" << itrConfig->path << ")" << std::endl;
      s_ConfigLocation tmpConfigLocation;
      initCommonConfig(tmpConfigLocation.configCommon);
      tmpConfigLocation.path = itrConfig->path;
      for(std::vector<parseconfig::context>::iterator itr =
          itrConfig->contexts.begin(); itr != itrConfig->contexts.end(); ++itr)
      {
        if (insertAlias(itr, tmpConfigLocation.alias))
          continue;
        if (insertAutoindex(itr, tmpConfigLocation.configCommon.autoindex))
          continue;
        if (insertAllowMethods(itr, tmpConfigLocation.configCommon.allowMethods, tmpConfigLocation.configCommon.allowMethodsBool))
          continue;
        if (insertAuthBasicInfo(itr, tmpConfigLocation.configCommon.authBasicUid, tmpConfigLocation.configCommon.authBasicPassword))
          continue;
        if (insertAuthBasicRealm(itr, tmpConfigLocation.configCommon.authBasicRealm))
          continue;          
        if (insertCgiScript(itr, tmpConfigLocation.configCommon.cgiScripts))
          continue;
        if (insertClientMaxBodySize(itr, tmpConfigLocation.configCommon.clientMaxBodySize))
          continue;
        if (insertErrorPages(itr, tmpConfigLocation.configCommon.errorPages))
          continue;
        if (insertIndexs(itr, tmpConfigLocation.configCommon.indexs))
          continue;
        std::string errorMessage = "Config Error : invalid directive ";
        errorMessage +=  itr->key ;
        throw std::runtime_error(errorMessage);          
      }
      size_t i = 0;
      while (i < tmpConfigServer.locations.size())
      {
        if (tmpConfigServer.locations.at(i).path.size() < tmpConfigLocation.path.size())
          break;
        i++;
      }
      // std::cout << "[DEBUG]  ロケーション設定挿入位置 : " << i << std::endl;
      tmpConfigServer.locations.insert(tmpConfigServer.locations.begin() + i, tmpConfigLocation);
      // std::cout << std::endl;
    }
    c.configGlobal.servers.push_back(tmpConfigServer);
  }
  inheritedFromHigherlevelDirectives(c);
  return true;
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

bool parseConfig::isMethod(std::string s)
{
  if (s == "GET" ||
    s == "HEAD" ||
    s == "POST" ||
    s == "PUT" ||
    s == "DELETE" ||
    s == "CONNECT" ||
    s == "OPTIONS" ||
    s == "TRACE"
  )
    return true;
  return false;
}

bool parseConfig::isFilenameExtension(std::string s)
{
  if (s[0] != '.')
    return false;
  if (s.find(".", 1) != std::string::npos)
    return false;
  return true;
}

void parseConfig::initCommonConfig(s_ConfigCommon &c)
{
  c.clientMaxBodySize = -1;
}

bool parseConfig::insertAutoindex(contextIterator itr, std::string& autoindex)
{
  if (itr->key == "autoindex")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie autoindex");
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid autoindex");
    std::string autoindexValue = itr->values.at(0).value.at(0);
    if (!(autoindexValue == "off" || autoindexValue == "on"))
      throw std::runtime_error("Config Error : invalid autoindex");
    // std::cout << "[DEBUG]autoindex : " << autoindexValue << std::endl;
    autoindex = autoindexValue;
    return true;
  }
  return false;
}

bool parseConfig::insertAlias(contextIterator itr, std::string& alias)
{
  if (itr->key == "alias")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie alias");
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid alias");
    std::string aliasValue = itr->values.at(0).value.at(0);
    // std::cout << "[DEBUG]alias : " << aliasValue << std::endl;
    alias = aliasValue;
    return true;
  }
  return false;
}

bool parseConfig::insertAllowMethods(contextIterator itr, std::vector<std::string>& allowMethods, bool* allowMethodsBool)
{
  if (itr->key == "allow_methods")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie allow_methods");
    if (itr->values.at(0).value.size() == 0)
      throw std::runtime_error("Config Error : invalid allow_methods");
    for (int i = 0; i < METHOD_NUM; i++)
      allowMethodsBool[i] = false;
    for (std::vector<std::string>::iterator itrValues = 
        itr->values.at(0).value.begin(); itrValues != itr->values.at(0).value.end(); ++itrValues)
    {
      for (std::vector<std::string>::iterator itrAllowMethods = allowMethods.begin();
        itrAllowMethods != allowMethods.end();
        itrAllowMethods++
      )
      {
        if (*itrAllowMethods == *itrValues)
          throw std::runtime_error("Config Error : duplicatie method of allow_methods");
      }
      if (!isMethod(*itrValues))
        throw std::runtime_error("Config Error : Invalid method in allow_methods");
      allowMethods.push_back(*itrValues);
      if (*itrValues == "GET")
        allowMethodsBool[GET] = true;
      if (*itrValues == "HEAD")
        allowMethodsBool[HEAD] = true;
      if (*itrValues == "POST")
        allowMethodsBool[POST] = true;
      if (*itrValues == "PUT")
        allowMethodsBool[PUT] = true;
      if (*itrValues == "DELETE")
        allowMethodsBool[DELETE] = true;
      if (*itrValues == "CONNECT")
        allowMethodsBool[CONNECT] = true;
      if (*itrValues == "OPTIONS")
        allowMethodsBool[OPTIONS] = true;
      if (*itrValues == "TRACE")
        allowMethodsBool[TRACE] = true;
      // std::cout << "[DEBUG]allow_methods : " << *itrValues << std::endl;
    }
    return true;
  }
  return false;
}

bool parseConfig::insertAuthBasicRealm(contextIterator itr, std::string& authBasicRealm)
{
  if (itr->key == "auth_basic")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie auth_basic");
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid auth_basic");
    authBasicRealm = itr->values.at(0).value.at(0);
    // std::cout << "[DEBUG]auth_basic : " << authBasicRealm << std::endl;
    return true;
  }
  return false;
}

bool parseConfig::insertAuthBasicInfo(contextIterator itr, std::string& authBasicUid, std::string& authBasicPassword)
{
  if (itr->key == "auth_basic_info")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie auth_basic");
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid auth_basic");
    size_t pos = itr->values.at(0).value.at(0).find(":");
    if (pos == std::string::npos || pos == 0)
      throw std::runtime_error("Config Error : invalid auth_basic");
    authBasicUid = itr->values.at(0).value.at(0).substr(0, pos);
    authBasicPassword = itr->values.at(0).value.at(0).substr(pos + 1);
    if (!ft_istoken(authBasicUid))
      throw std::runtime_error("Config Error : invalid auth_basic Using characters that cannot be used in the user ID.");
    // std::cout << "[DEBUG]authBasicUid : " << authBasicUid << std::endl;
    // std::cout << "[DEBUG]authBasicPassword : " << authBasicPassword << std::endl;
    return true;
  }
  return false;
}

bool parseConfig::insertCgiScript(contextIterator itr, std::vector<std::string>& cgiScripts)
{
  if (itr->key == "cgi_script")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie cgi_script");
    if (itr->values.at(0).value.size() == 0)
      throw std::runtime_error("Config Error : invalid cgi_script");
    for (std::vector<std::string>::iterator itrValues = itr->values.at(0).value.begin();
      itrValues != itr->values.at(0).value.end();
      ++itrValues)
    {
      for (std::vector<std::string>::iterator itrCgiScripts = cgiScripts.begin();
        itrCgiScripts != cgiScripts.end();
        itrCgiScripts++
      )
      {
        if (*itrCgiScripts == *itrValues)
          throw std::runtime_error("Config Error : duplicatie filename extension of cgi_script");
      }
      if (!isFilenameExtension(*itrValues))
        throw std::runtime_error("Config Error : Invalid filename extension in cgi_script");
      cgiScripts.push_back(*itrValues);
      // std::cout << "[DEBUG]cgi_script : " << *itrValues << std::endl;
    }
    return true;
  }
  return false;
}

bool parseConfig::insertClientMaxBodySize(contextIterator itr, int& clientMaxBodySize)
{
  if (itr->key == "client_max_body_size")
  {
    // 重複して設定された場合はエラー
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie client_max_body_size");
    // client_max_body_size 1000 100;のような場合はエラー
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid client_max_body_size");
    clientMaxBodySize = ft_stoi(itr->values.at(0).value.at(0).c_str());
    if (clientMaxBodySize == -1)
      throw std::runtime_error("Config Error : invalid client_max_body_size");
    // std::cout << "[DEBUG]client_max_body_size : " << clientMaxBodySize << std::endl;
    return true;
  }
  return false;
}

bool parseConfig::insertErrorPages(contextIterator itr, std::map<std::string, std::string>& errorPages)
{
  if (itr->key == "error_page")
  {
    for (std::vector<parseconfig::vvalues>::iterator itrValues = 
        itr->values.begin(); itrValues != itr->values.end(); ++itrValues)
    {
      if (itrValues->value.size() < 2)
        throw std::runtime_error("Config Error : invalid error_page");
      std::vector<std::string>::iterator itrValue = itrValues->value.end();
      std::string relativePath = *--itrValue;
      for (itrValue = itrValues->value.begin(); itrValue != itrValues->value.end() - 1; ++itrValue)
      {
        if (!isCode(*itrValue))
          throw std::runtime_error("Config Error : invalid error_page");
        errorPages.insert(std::make_pair(*itrValue, relativePath));
        // std::cout << "[DEBUG]error_page : " << *itrValue << " " << relativePath << std::endl;
      }
    }
    return true;
  }
  return false;
}

bool parseConfig::insertIndexs(contextIterator itr, std::vector<std::string>& indexs)
{
  if (itr->key == "index")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie index");
    if (itr->values.at(0).value.size() == 0)
      throw std::runtime_error("Config Error : invalid index");
    for (std::vector<std::string>::iterator itrValues = itr->values.at(0).value.begin();
      itrValues != itr->values.at(0).value.end();
      ++itrValues)
    {
      for (std::vector<std::string>::iterator itrIndexs = indexs.begin();
        itrIndexs != indexs.end();
        itrIndexs++
      )
      {
        if (*itrIndexs == *itrValues)
          throw std::runtime_error("Config Error : duplicatie index");
      }
      indexs.push_back(*itrValues);
      // std::cout << "[DEBUG]index : " << *itrValues << std::endl;
    }
    return true;
  }
  return false;
}

bool parseConfig::insertListen(contextIterator itr, std::string& host, int& port)
{
  if (itr->key == "listen")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie listen");
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid listen");
    size_t pos = itr->values.at(0).value.at(0).find(":");
    if (pos == std::string::npos || pos == 0)
      throw std::runtime_error("Config Error : invalid listen");
    host = itr->values.at(0).value.at(0).substr(0, pos);
    std::string tmpPort = itr->values.at(0).value.at(0).substr(pos + 1);
    if ((port = ft_stoi(tmpPort)) == -1 || port <=0 || port > 65535)
      throw std::runtime_error("Config Error : invalid listen");
    // std::cout << "[DEBUG]host : " << host << std::endl;
    // std::cout << "[DEBUG]port : " << port << std::endl;
    return true;
  }
  return false;
}

bool parseConfig::insertRoot(contextIterator itr, std::string& root)
{
  if (itr->key == "root")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie root");
    if (itr->values.at(0).value.size() >= 2)
      throw std::runtime_error("Config Error : invalid root");
    root = itr->values.at(0).value.at(0);
    // std::cout << "[DEBUG]root : " << root << std::endl;
    return true;
  }
  return false;
}

bool parseConfig::insertServerNames(contextIterator itr, std::vector<std::string>& serverNames)
{
  if (itr->key == "server_name")
  {
    if (itr->values.size() > 1)
      throw std::runtime_error("Config Error : duplicatie server_name");
    for (std::vector<std::string>::iterator itrValues = itr->values.at(0).value.begin();
      itrValues != itr->values.at(0).value.end();
      ++itrValues)
    {
      for (std::vector<std::string>::iterator itrServerNames = serverNames.begin();
        itrServerNames != serverNames.end();
        itrServerNames++
      )
      {
        if (*itrServerNames == *itrValues)
          throw std::runtime_error("Config Error : duplicatie server_name");
      }
      serverNames.push_back(*itrValues);
      // std::cout << "[DEBUG]server_name : " << *itrValues << std::endl;
    }
    return true;
  }
  return false;
}

void parseConfig::inheritedCommonConfig(s_ConfigCommon& higher, s_ConfigCommon& lower)
{
  if (lower.autoindex.empty())
    lower.autoindex = higher.autoindex;
  if (lower.allowMethods.empty())
  {
    lower.allowMethods = higher.allowMethods;
    for (int i = 0; i < METHOD_NUM; i++)
    {
      lower.allowMethodsBool[i] = higher.allowMethodsBool[i];
    }
  }
  if (lower.authBasicRealm.empty())
    lower.authBasicRealm = higher.authBasicRealm;
  else if(lower.authBasicRealm == "off")
    lower.authBasicRealm.clear();
  if (lower.authBasicUid.empty())
    lower.authBasicUid = higher.authBasicUid;
  if (lower.authBasicPassword.empty())
    lower.authBasicPassword = higher.authBasicPassword;
  if (lower.cgiScripts.empty())
    lower.cgiScripts = higher.cgiScripts;
  if (lower.clientMaxBodySize == -1)
    lower.clientMaxBodySize = higher.clientMaxBodySize;
  if (lower.indexs.empty())
    lower.indexs = higher.indexs;
  for (std::map<std::string, std::string>::iterator itr = higher.errorPages.begin();
      itr != higher.errorPages.end();
      itr++
  )
  {
    if (lower.errorPages.find(itr->first) == lower.errorPages.end())
      lower.errorPages[itr->first] = itr->second;
  }
}

void parseConfig::inheritedFromHigherlevelDirectives(Config& c)
{
  for(std::vector<s_ConfigServer>::iterator itrServer = c.configGlobal.servers.begin();
      itrServer != c.configGlobal.servers.end();
      ++itrServer)
  {
    inheritedCommonConfig(c.configGlobal.configCommon, itrServer->configCommon);
    for(std::vector<s_ConfigLocation>::iterator itrLocation = itrServer->locations.begin();
        itrLocation != itrServer->locations.end();
        ++itrLocation)
    {
      inheritedCommonConfig(itrServer->configCommon, itrLocation->configCommon);
    }
  }
}

bool parseConfig::notExistRootDirective(Config& c)
{
  for(std::vector<s_ConfigServer>::iterator itrServer = c.configGlobal.servers.begin();
      itrServer != c.configGlobal.servers.end();
      ++itrServer)
  {
    if (itrServer->root == "")
      return true;
  }
  return false;
}

parseConfig::parseConfig(const char *configFile, Config& c)
{
  int fd = open(configFile, O_RDONLY);
  if (fd == -1)
  {
    std::cerr << "Failed to open the config file." << std::endl;
    std::cerr << strerror(errno) << std::endl; 
    exit(1);
  }
  int r;
  std::string cmd;
  char buf[1024];
  int i_for_buf = 0;
  while (i_for_buf < 1024)
    buf[i_for_buf++] = 0;
  while ((r = read(fd, buf, 1023)) > 0)
  {
    cmd.append(buf);
    i_for_buf = 0;
    while (i_for_buf < 1024)
      buf[i_for_buf++] = 0;
  }
  if (r == -1)
  {
    std::cerr << "Failed to load the config file." << std::endl;
    std::cerr << strerror(errno) << std::endl; 
    exit(1);
  }
  r = close(fd);
  if (r == -1)
  {
    std::cerr << "Failed to close the config file." << std::endl;
    std::cerr << strerror(errno) << std::endl; 
    exit(1);
  }

  try
  {
    lexer(cmd);
    {
      // デバッグ用出力
      // printToken();
      // std::cout << std::endl;
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
  {
    std::cout << "設定ファイルの文法チェック : ";
    if (checkFormat() == 0)
    {
      std::cout << "NG" << std::endl;
      exit(1);
    }
    else
      std::cout << "OK" << std::endl;
  }
  {
    std::cout << "設定ファイルの内容を一時作業領域に格納 : ";
    if (r == insertConfigToTmpMemory())
    {
      std::cerr << "NG" << std::endl;
      exit(1);
    }
    else
      std::cerr << "OK" << std::endl;
    {
      // デバッグ用出力
      // printConfigHttp();
      // std::cout << std::endl;
    }
  }
  try
  {
    insertToConfigClass(c);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
  // serverディレクティブにrootの設定があるかを確認し、なかったら終了する
  if (notExistRootDirective(c))
  {
    std::cerr << "There is a server directive for which the root directive does not exist." << std::endl;
    exit(1);
  }
  {
    // デバッグ用出力
    // c.printConfig();
  }
}

parseConfig::parseConfig()
{

}

parseConfig::~parseConfig()
{

}
