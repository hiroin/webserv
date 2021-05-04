#include "readData.hpp"

unsigned long readData::readBuffsize_ = 1000000;

void readData::clearData()
{
  fd_ = -1;
  readData_.clear();
  completeRead_ = false;
}

void readData::setFd(int fd)
{
  fd_ = fd;
}

bool readData::readFromFd()
{
  int read_size;
  char buf[readBuffsize_];
  std::string tmp;

  memset(buf, 0, sizeof(buf));
  read_size = read(fd_, buf, sizeof(buf));
  if (read_size == -1)
  {
    std::cout << "read() failed." << std::endl;
    std::cout << "ERROR: " << errno << std::endl;
    readData_.erase();
    return FAILURE;
  }
  if (read_size == 0)
  {
    completeRead_ = true;
  }
  tmp.clear();
  tmp.assign(buf, read_size);
  readData_ += tmp;
  return SUCCESS;
}

bool readData::isCompleteRead() const
{
  return completeRead_;
}

std::string readData::getReadData() const
{
  return readData_;
}

void readData::setReadData(std::string data)
{
  readData_ = data;
}

readData::readData() : completeRead_(false)
{

}

readData::~readData()
{

}