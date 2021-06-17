#ifndef _READDATA_H_
#define _READDATA_H_

#include <iostream>
#include <string>
#include "ft.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

class readData {
 private:
  static ssize_t readBuffsize_;

 public:
  void clearData();
  void setFd(int fd);
  bool readFromFd();
  bool isCompleteRead() const;
  std::string getReadData() const;
  bool cutOutRecvDataToEol();
  std::string getExtractedData() const;

  // デバッグ用
  void setReadData(std::string data);

 public:
  readData();
  ~readData();

 private:
  int fd_;
  bool completeRead_;
  std::string readData_;
  std::string extractedData_;

};

#endif