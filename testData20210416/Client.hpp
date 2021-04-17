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
  Client();
  ~Client();
  bool operator==(const Client&);
  bool operator!=(const Client&);  

 private:
  Client(const Client&);
  Client& operator=(const Client&);  

  //テスト用
  static int dummyFd;
};


#endif