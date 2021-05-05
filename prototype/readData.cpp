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

bool readData::cutOutRecvDataToEol()
{
  std::string::size_type pos;

  for (std::string::iterator itr = readData_.begin(); itr != readData_.end(); itr++)
  {
    if (*itr == '\n')
    {
      pos = readData_.find("\n");
      extractedData_ = readData_.substr(0, pos);
      readData_ = readData_.substr(pos + 1);
      return SUCCESS;
    }
    else if (*itr == '\r')
    {
      pos = readData_.find("\r");
      if (readData_.size() >= pos && readData_[pos + 1] == '\n')
      {
        extractedData_ = readData_.substr(0, pos);
        readData_ = readData_.substr(pos + 2);
        return SUCCESS;
      }
      else
      {
        extractedData_ = readData_.substr(0, pos);
        readData_ = readData_.substr(pos + 1);
        return SUCCESS;
      }
    }
  }
  return FAILURE;
}

std::string readData::getExtractedData() const
{
  return extractedData_;
}

readData::readData() : completeRead_(false)
{

}

readData::~readData()
{

}