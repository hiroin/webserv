#include "getChunked.hpp"

// chunkedなデータをclientBody_に格納する
// chunkedの形式にあっていないデータな場合400を返す
// chunkedなデータの受信が完了しない場合は-1を返す
int getChunked::parseChunkedData()
{
  if (clientBody_ == NULL || r_ == NULL)
    return 500;
  bool bNotDone = true;
  std::string chunksize_chunkext;
  while (bNotDone == true)
  {
    if (status_ == GET_CHUNK_SIZE)
    {
      if (r_->cutOutRecvDataToEol() == 0)
        return -1;
      chunksize_chunkext = r_->getExtractedData();
      if (debugLevel_ >= 1)
        std::cout << "[DEBUG]chunksize_chunkext : [" << chunksize_chunkext << "]" << std::endl;
      std::string::size_type pos = chunksize_chunkext.find(";");
      if (pos != std::string::npos)
        chunksize_ = chunksize_chunkext.substr(0, pos);
      else
        chunksize_ = chunksize_chunkext;
      if (chunksize_ == "0")
      {
        bNotDone = false;
        break;
      }
      if (debugLevel_ >= 1)
      {
        std::cout << "[DEBUG]chunksize(hex)     : [" << chunksize_ << "]" << std::endl;
        std::cout << "[DEBUG]chunksize(dec)     : [" << hexstring2int(chunksize_) << "]" << std::endl;
      }
      if (hexstring2int(chunksize_) == -1)
        return 400;
      status_ = GET_CHUNK_DATA;
    }
    if (status_ == GET_CHUNK_DATA)
    {
      if (r_->cutOutRecvDataBySpecifyingBytes(hexstring2int(chunksize_)) == 0)
        return -1;
      if (debugLevel_ >= 2)
        std::cout << "[DEBUG]chunk-data         : [" << r_->getExtractedData() << "]" << std::endl;
      *clientBody_ += r_->getExtractedData();
      status_ = GET_CHUNK_CRLF;
    }
    if (status_ == GET_CHUNK_CRLF)
    {
      if (r_->cutOutRecvDataToEol() == 0)
        return -1;
      if (r_->getExtractedData() != "")
        return 400;
      status_ = GET_CHUNK_SIZE;
    }
  }
  return 200;
}

// 16進数をintの10進数に変換する
// INT_MAXを超えた場合やHEXの文字以外が入力された場合は-1を返す
int getChunked::hexstring2int(std::string chunksize)
{
  long iBytes = 0;
  long iFactor = 1;

  if (chunksize.size() >= 9)
    return -1;
  for (std::string::reverse_iterator itr = chunksize.rbegin();
    itr != chunksize.rend();
    itr++
  )
  {
    int value = hex2int(*itr);
    if (value == -1)
      return -1;
    iBytes += iFactor * value;
    if (iBytes > INT_MAX)
      return -1;
    iFactor *= 16;
  }
  return iBytes;
}

// 16進文字を検索intに変換する
// 16進文字以外の場合は-1を検索返す
int getChunked::hex2int(char c)
{
  switch (c)
  {
    case 'A':
    case 'a':
      return 10;
    case 'B':
    case 'b':
      return 11;
    case 'C':
    case 'c':
      return 12;
    case 'D':
    case 'd':
      return 13;
    case 'E':
    case 'e':
      return 14;
    case 'F':
    case 'f':
      return 15;
  }
  if (std::isdigit(c))
    return c - '0';
  return -1;
}

void getChunked::setClientBody(std::string *clientBody)
{
  if (clientBody == NULL)
    return;
  clientBody_ = clientBody;
}

void getChunked::setRecvData(recvData *r)
{
  if (r == NULL)
    return;
  r_ = r;
}

void getChunked::clear()
{
  status_ = GET_CHUNK_SIZE;
  chunksize_.clear();
}

void getChunked::setDebugLevel(int level)
{
  debugLevel_ = level;
}

getChunked::getChunked() : status_(GET_CHUNK_SIZE), clientBody_(NULL), r_(NULL), debugLevel_(0)
{
}

getChunked::~getChunked()
{
}