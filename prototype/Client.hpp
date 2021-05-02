#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <string>
#include <iostream>
#include "recvData.hpp"
#include "HTTPMessageParser.hpp"
#include "getChunked.hpp"
#include "sendController.hpp"

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
  recvData receivedData;
  recvData readData;
  HTTPMessageParser hmp;
  getChunked gc;
  SendController sc;

  // 処理状態とフラグ
  enum clientStatus status;
  bool bCGI;
  bool bChunked;
  int responseCode;
  size_t responseFileSize;
  std::string responseMessege;

  // メッセージボディ
  std::string body;
  
  // クライアントからの接続タイムアウト用
  struct timeval lastTv;
  struct timeval nowTv;

  // ヘッダの内容を見てボディを受信するかを返す
  // ヘッダに「Transfer-Encoding: chunked」があった場合、bChunkedをtrueに変更する
  bool isNeedBody(std::map<std::string, std::string> headers);

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