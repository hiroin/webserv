#include "recvData.hpp"

void recvData::clearData()
{
  recvData_.clear();
  extractedData_.clear();
}

void recvData::setSocketFd(int fd)
{
  socketFd_ = fd;
}

int recvData::recvFromSocket()
{
  int read_size;
  char buf[RECV_BUFFSIZE];

  ft_memset(buf, 0, sizeof(buf));
  read_size = recv(socketFd_, buf, sizeof(buf), 0);
  if (read_size == 0) {
    std::cout << "graceful shutdown." << std::endl;
    close(socketFd_);
    recvData_.erase();
    extractedData_.erase();
    return FAILURE;
  }
  if (read_size == -1 && errno != EAGAIN) {
    std::cout << "read() failed." << std::endl;
    std::cout << "ERROR: " << errno << std::endl;
    close(socketFd_);
    recvData_.erase();
    extractedData_.erase();
    return FAILURE;
  }
  recvData_.append(buf);
  return SUCCESS;
}

int recvData::cutOutRecvDataBySpecifyingBytes(size_t size)
{
  if (recvData_.size() < size)
    return FAILURE;
  extractedData_ = recvData_.substr(0, size);
  recvData_ = recvData_.substr(size);
  return SUCCESS;
}

int recvData::cutOutRecvDataToEol()
{
  std::string::size_type pos;
  
  pos = recvData_.find("\r\n");
  if (pos != std::string::npos)
  {
    extractedData_ = recvData_.substr(0, pos);
    recvData_ = recvData_.substr(pos + 2);
    return SUCCESS;
  }
  pos = recvData_.find("\r");
  if (pos != std::string::npos)
  {
    extractedData_ = recvData_.substr(0, pos);
    recvData_ = recvData_.substr(pos + 1);
    return SUCCESS;
  }
  pos = recvData_.find("\r");
  if (pos != std::string::npos)
  {
    extractedData_ = recvData_.substr(0, pos);
    recvData_ = recvData_.substr(pos + 1);
    return SUCCESS;
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

recvData::recvData()
{

}

recvData::~recvData()
{

}