#include "Client.hpp"

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
