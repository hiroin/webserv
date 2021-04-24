#include "Client.hpp"

bool Client::isNeedBody(std::map<std::string, std::string> headers)
{
  std::map<std::string, std::string>::iterator itr;
  itr = headers.find("transfer-encoding");
  if (itr != headers.end() && itr->second == "chunked")
  {
    this->bChunked = true;
    return true;
  }
  itr = headers.find("content-length");
  if (itr != headers.end())
    return true;
  return false;
}

Client::Client() : \
  socketFd(-1), readFd(-1), writeFd(-1), pipeReadFd(-1), \
  pipeWriteFd(-1), port(-1), status(PARSE_STARTLINE), \
  bCGI(false), bChunked(false)
{
}

Client::~Client()
{

}

bool Client::operator==(const Client& other)
{
  if (socketFd == other.socketFd)
    return true;
  return false;
}

bool Client::operator!=(const Client& other)
{
  if (*this == other)
    return false;
  return true;
}

// テスト用
int Client::dummyFd = 4;