#include "Config.hpp"
#include <iostream>

s_ConfigCommon::s_ConfigCommon()
{
  clientMaxBodySize = -1;
}

void Config::printConfig()
{
  std::cout << "コンフィグ設定" << std::endl;
  std::cout << "Global" << std::endl;
  std::cout << " phpCgiPath : " << configGlobal.phpCgiPath << std::endl;
  
}

Config::Config()
{

};

Config::~Config()
{

};