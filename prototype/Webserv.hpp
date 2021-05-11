#ifndef _WEBSERV_H_
#define _WEBSERV_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <algorithm>
#include <errno.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>

#include "Socket.hpp"
#include "recvData.hpp"
#include "HTTPMessageParser.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "parseConfig.hpp"
#include "getChunked.hpp"
#include "ft.hpp"
#include "Response.hpp"

struct portAndHost
{
  int port;
  std::string host;
};

class Wevserv
{
 public:
  bool notBeCreatedSocket(int port, std::string host, std::vector<portAndHost>& portAndHostCombination);
  void setupServers();
  void initFD();
  bool isNotKeepConnectionCode(int code);
  void responseNot200(int i, int code);
  std::map<int, std::string> getMonth();
  std::string ft_ito00(int n);
  std::string getTime();
  void coutLog(int i);
  void debugPrintGetRecvData(int i);
  void debugPrintGetReadData(int i);
  void debugPrintExtractedData(int i);
  void debugPrintStartLine(int i);
  void debugPrintHeaders(int i);
  void debugPrintResponceData(int i);
  void debugPrintRequestBody(int i);
  void debugPrintResponseMessege(int i);
  Wevserv(Config& c);
  ~Wevserv();

 private:
  Config& c_;
  std::vector<Socket> servers_;
  Client clients_[MAX_SESSION];
  Response *responses_[MAX_SESSION];
  fd_set readFds_;
  fd_set writeFds_;
  int maxFd_; 
  Wevserv();
};

#endif