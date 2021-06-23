#include "recvData.hpp"

ssize_t recvData::recvBuffsize_ = 1000000;

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
  ssize_t read_size;
  char buf[recvBuffsize_];
  std::string tmp;

  memset(buf, 0, sizeof(buf));
  read_size = recv(fd_, buf, sizeof(buf), 0);
  if (read_size == 0) {
    recvData_.erase();
    extractedData_.erase();
    return FAILURE;
  }
  if (read_size == -1)
  {
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
      // std::cout << "extractedData_[n ] : " << extractedData_<< std::endl;
      return SUCCESS;
    }
    else if (*itr == '\r')
    {
      pos = recvData_.find("\r");
      if (recvData_.size() >= pos && recvData_[pos + 1] == '\n')
      {
        extractedData_ = recvData_.substr(0, pos);
        recvData_ = recvData_.substr(pos + 2);
        // std::cout << "extractedData_[rn] : " << extractedData_<< std::endl;
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
  // std::cout << "--extractedData_-----------------------" << std::endl;
  // std::cout << extractedData_ << std::endl;
  // std::cout << "---------------------------------------" << std::endl;
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