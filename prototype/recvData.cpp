#include "recvData.hpp"

unsigned long recvData::recvBuffsize_ = 1000000;

void recvData::clearData()
{
  recvData_.clear();
  extractedData_.clear();
}

void recvData::setFd(int fd)
{
  fd_ = fd;
}

bool recvData::recvFromSocket()
{
  int read_size;
  char buf[recvBuffsize_];
  std::string tmp;

  memset(buf, 0, sizeof(buf));
  read_size = recv(fd_, buf, sizeof(buf), 0);
  if (read_size == 0) {
    std::cout << "graceful shutdown." << std::endl;
    recvData_.erase();
    extractedData_.erase();
    return FAILURE;
  }
  if (read_size == -1)
  {
    std::cout << "read() failed." << std::endl;
    std::cout << "ERROR: " << errno << std::endl;
    recvData_.erase();
    extractedData_.erase();
    return FAILURE;
  }
  tmp.clear();
  tmp.assign(buf, read_size);
  recvData_ += tmp;
  return SUCCESS;
}

bool recvData::readFromFd()
{
  int read_size;
  char buf[recvBuffsize_];
  std::string tmp;

  memset(buf, 0, sizeof(buf));
  read_size = read(fd_, buf, sizeof(buf));
  if (read_size == -1)
  {
    std::cout << "read() failed." << std::endl;
    std::cout << "ERROR: " << errno << std::endl;
    recvData_.erase();
    extractedData_.erase();
    return FAILURE;
  }
  tmp.clear();
  tmp.assign(buf, read_size);
  recvData_ += tmp;
  return SUCCESS;
}

bool recvData::cutOutRecvDataBySpecifyingBytes(size_t size)
{
  if (recvData_.size() < size)
    return FAILURE;
  extractedData_ = recvData_.substr(0, size);
  recvData_ = recvData_.substr(size);
  return SUCCESS;
}

// bool recvData::cutOutRecvDataToEol()
// {
//   std::string::size_type pos;
  
//   pos = recvData_.find("\r\n");
//   if (pos != std::string::npos)
//   {
//     extractedData_ = recvData_.substr(0, pos);
//     recvData_ = recvData_.substr(pos + 2);
//     return SUCCESS;
//   }
//   pos = recvData_.find("\r");
//   if (pos != std::string::npos)
//   {
//     extractedData_ = recvData_.substr(0, pos);
//     recvData_ = recvData_.substr(pos + 1);
//     return SUCCESS;
//   }
//   pos = recvData_.find("\n");
//   if (pos != std::string::npos)
//   {
//     extractedData_ = recvData_.substr(0, pos);
//     recvData_ = recvData_.substr(pos + 1);
//     return SUCCESS;
//   }
//   return FAILURE;
// }

bool recvData::cutOutRecvDataToEol()
{
  std::string::size_type pos;

  for (std::string::iterator itr = recvData_.begin(); itr != recvData_.end(); itr++)
  {
    if (*itr == '\n')
    {
      pos = recvData_.find("\n");
      extractedData_ = recvData_.substr(0, pos);
      recvData_ = recvData_.substr(pos + 1);
      return SUCCESS;
    }
    else if (*itr == '\r')
    {
      pos = recvData_.find("\r");
      if (recvData_.size() >= pos && recvData_[pos + 1] == '\n')
      {
        extractedData_ = recvData_.substr(0, pos);
        recvData_ = recvData_.substr(pos + 2);
        return SUCCESS;
      }
      else
      {
        extractedData_ = recvData_.substr(0, pos);
        recvData_ = recvData_.substr(pos + 1);
        return SUCCESS;
      }
    }
  }
  return FAILURE;
}

std::string recvData::getRecvData() const
{
  return recvData_;
}

std::string recvData::getExtractedData() const
{
  return extractedData_;
}

void recvData::setRecvData(std::string data)
{
  recvData_ = data;
}

recvData::recvData()
{

}

recvData::~recvData()
{

}