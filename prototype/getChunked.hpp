#ifndef _GETCHUNKED_H_
#define _GETCHUNKED_H_


#ifdef _GLIBCXX_DEBUG
# include <debug/vector>
#endif
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
  int parseChunkedData();
  void clear();

 public:
  getChunked();
  ~getChunked();

 public:
  enum chunkStatus status_;
  std::string *clientBody_;
  recvData *r_;

 public:
  int hexstring2int(std::string chunksize);
  int hex2int(char c);

 private:
   std::string chunksize_;

};

#endif