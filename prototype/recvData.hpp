#ifndef _RECVDATA_H_
#define _RECVDATA_H_

#include <iostream>
#include <string>
#include "ft.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

class recvData {
 private:
  static ssize_t recvBuffsize_;

 public:
  void clearData();
  void setFd(int fd);
  bool recvFromSocket();
  bool cutOutRecvDataBySpecifyingBytes(size_t size);
  bool cutOutRecvDataToEol();
  bool recvDataToEof();
  std::string getRecvData() const;
  std::string getExtractedData() const;

  // デバッグ用
  void setRecvData(std::string data);

 public:
  recvData();
  ~recvData();

 private:
  int fd_;
  std::string recvData_;
  std::string extractedData_;

};

#endif