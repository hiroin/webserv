#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>
#include <string>
#include <iostream>
#include "recvData.hpp"
#include "HTTPMessageParser.hpp"

enum clientStatus {PARSE_STARTLINE, PARSE_HEADER, RESV_BODY, CREATE_RESPONSE, READ, WRITE, SEND, NUM_OF_CLIENTSTATUS};

class Client
{
 public:
  // fd
  int socketFd;
  int readFd;
  int writeFd;
  int pipeReadFd;
  int pipeWriteFd;

  // どのサーバーに接続したかを識別する情報
  int port;
  std::string host;

  // 各種処理をするためのclass
  recvData recvData;
  HTTPMessageParser hmp;

  // 処理状態とフラグ
  enum clientStatus status;
  bool bCGI;
  bool bChunked;

  // クライアントからの接続タイムアウト用
  struct timeval lastTv;
  struct timeval nowTv;

  Client();
  ~Client();
  bool operator==(const Client&);
  bool operator!=(const Client&);

 private:
  Client(const Client&);
  Client& operator=(const Client&);

};

#endif