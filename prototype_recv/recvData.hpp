#ifndef _RECVDATA_H_
#define _RECVDATA_H_

#include <iostream>
#include <string>
#include "ft.hpp"
#include <sys/socket.h>
#include <unistd.h>

#define SUCCESS 1
#define FAILURE 0
#define RECV_BUFFSIZE 1024

class recvData {
 public:
  void clearData();
  void setSocketFd(int fd);
  int recvFromSocket();
  int cutOutRecvDataBySpecifyingBytes(size_t size);
  int cutOutRecvDataToEol();
  std::string getRecvData() const;
  std::string getExtractedData() const;

 public:
  recvData();
  ~recvData();

 private:
  int socketFd_;
  std::string recvData_;
  std::string extractedData_;

};

#endif