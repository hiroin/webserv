#include "Config.hpp"
#include <iostream>

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