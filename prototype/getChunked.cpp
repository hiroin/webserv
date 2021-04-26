#include "getChunked.hpp"

// chunkedなデータをclientBody_に格納する
// chunkedの形式にあっていないデータな場合400を返す
// 
int getChunked::parseChunkedData()
{
  if (clientBody_ == NULL || r_ == NULL)
    return -1;
  bool bNotDone = true;
  std::string chunksize_chunkext;
  std::string chunksize;  
  while (bNotDone == true)
  {
    r_->cutOutRecvDataToEol();
    chunksize_chunkext = r_->getExtractedData();
    std::cout << "chunksize_chunkext : [" << chunksize_chunkext << "]" << std::endl;
    std::string::size_type pos = chunksize_chunkext.find(";");
    if (pos != std::string::npos)
      chunksize = chunksize_chunkext.substr(0, pos);
    else
      chunksize = chunksize_chunkext;
    if (chunksize == "0")
    {
      bNotDone = false;
      continue;
    }
    std::cout << "chunksize(hex)     : [" << chunksize << "]" << std::endl;
    std::cout << "chunksize(dec)     : [" << hexstring2int(chunksize) << "]" << std::endl;
    if (hexstring2int(chunksize) == -1)
      return 400;
    r_->cutOutRecvDataBySpecifyingBytes(hexstring2int(chunksize));
    std::cout << "chunk-data         : [" << r_->getExtractedData() << "]" << std::endl;
    *clientBody_ += r_->getExtractedData();
    r_->cutOutRecvDataToEol();
    if (r_->getExtractedData() != "")
      return 400;
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

getChunked::getChunked() : status_(GET_CHUNK_SIZE), clientBody_(NULL), r_(NULL)
{
}

getChunked::~getChunked()
{
}