#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <string>
#include "HTTPMessageParser.hpp"

enum clientStatus {PARSE_STARTLINE, PARSE_HEADER, RESV_BODY, CREATE_RESPONSE, READ, WRITE, SEND, NUM_OF_CLIENTSTATUS};
class Client
{
 public:
  int socketFd;
  int port;
  std::string host;
  HTTPMessageParser hmp;
  enum clientStatus status;
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