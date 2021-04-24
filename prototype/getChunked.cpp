#include "getChunked.hpp"

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