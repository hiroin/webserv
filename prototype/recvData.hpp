#ifndef _RECVDATA_H_
#define _RECVDATA_H_

#include <iostream>
#include <string>
#include "ft.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

class recvData {
 private:
  static unsigned long recvBuffsize_;

 public:
  void clearData();
  void setSocketFd(int fd);
  bool recvFromSocket();
  bool cutOutRecvDataBySpecifyingBytes(size_t size);
  bool cutOutRecvDataToEol();
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