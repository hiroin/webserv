#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "ft.hpp"

#define SUCCESS 1
#define FAILURE 0
namespace client {
  enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER};
}

class Client {
 public:
  bool setData(const char *data, size_t size);
  void getRequestLineFromRecvData();
  bool parseRequestLine();
  std::string getMessageHeader() const;
  enum client::method getMethod() const;
  std::string getHTTPVersion() const;
  void getHeaderFromRecvData();
  bool parseRequestTarget();
  bool parseHeader();
  char *getMessageBody() const;

 public:
  Client();
  ~Client();

 private:
  char *recvData_;
  size_t recvDataSize_;
  std::string messageHeader_;
  size_t messageHeaderEndPosition_;
  std::string requestLine_;

  // requestLine_をparseした結果を格納
  enum client::method method_;
  std::string requestTarget_;
  std::string HTTPVersion_;

  // requestTarget_をparseした結果を格納
  bool isAuthority(std::string requestTarget);
  std::string getHostAndPort(std::string requestTarget);
  std::string absolutePath_; // OPTIONSの*の場合のここにいれる
  std::string query_;
  std::string authority_; // CONNECTの場合の接続先

  char *messageBody_;
};

/*

https://triple-underscore.github.io/RFC7230-ja.html
*/
#endif