#include "getChunked.hpp"

int 

int getChunked::hex2dec(char c)
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
    default:
      return c - '0';
  }
}

void getChunked::setClientBody(std::string *clientBody)
{
  clientBody_ = clientBody;
}

void getChunked::setRecvData(recvData *r)
{
  r_ = r;
}

getChunked::getChunked() : status_(GET_CHUNK_SIZE)
{
}

getChunked::~getChunked()
{
}