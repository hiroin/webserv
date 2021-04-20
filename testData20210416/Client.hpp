#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <string>
#include "HTTPMessageParser.hpp"

class Client
{
 public:
  int socketFd_;
  int port_;
  std::string host_;
  HTTPMessageParser hmp_;
  Client(int testPatternOfClient, int testPatternOfHTTPMessageParser);
  Client(int testPatternOfClient, std::string testPatternOfHTTPMessageParserConf, int testPatternOfHTTPMessageParser); 
  Client();
  ~Client();
  bool operator==(const Client&);
  bool operator!=(const Client&);  

 private:
  Client(const Client&);

  //テスト用
  void generateTestValue(int testPatternOfClient);
  static int dummyFd;
};


#endif