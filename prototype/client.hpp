#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "ft.hpp"

#define SUCCESS 1
#define FAILURE 0
enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER};

class Client {
 public:
  bool setData(const char *data, size_t size);
  void getRequestLineFromRecvData();
  bool parseRequestLine();
  std::string getMessageHeader() const;
  enum method getMethod() const;
  std::string getHTTPVersion() const;
  void getHeaderFromRecvData();
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
  enum method method_;
  std::string requestTarget_;
  std::string HTTPVersion_;

  // requestTarget_をparseした結果を格納
  std::string absolutePath_;
  std::string query_;
  std::string authority_; // CONNECTの場合の接続先
  std::string asteriskForm; // OPTIONSの*の場合

  char *messageBody_;
};

/*

https://triple-underscore.github.io/RFC7230-ja.html
*/
#endif