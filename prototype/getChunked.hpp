#ifndef _GETCHUNKED_H_
#define _GETCHUNKED_H_

#include <iostream>
#include <string>
#include <errno.h>
#include "ft.hpp"
#include "recvData.hpp"

enum chunkStatus {GET_CHUNK_SIZE, GET_CHUNK_DATA, GET_CHUNK_CRLF};

// Clientクラスのメンバ変数bodyと、recvDataクラスを受け取って、chunkedなデータを結合し、bodyに格納する
// chunkedのデータ書式に従わない場合は501を返す
class getChunked {
 public:
  void setClientBody(std::string *clientBody);
  void setRecvData(recvData *r);

 public:
  getChunked();
  ~getChunked();

 public:
  enum chunkStatus status_;
  std::string *clientBody_;
  recvData *r_;

 public:
  int hexstring2dec(std::string chunksize);
  int getChunked::hex2dec(char c);

};

#endif