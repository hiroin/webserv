#include "Client.hpp"

Client::Client()
{
  HTTPMessageParser hmp(1);
  hmp_ = hmp;

  //テスト用
  socketFd_ = dummyFd;
  dummyFd++;
}

Client::~Client()
{

}

bool Client::operator==(const Client& other)
{
  if (socketFd_ == other.socketFd_)
    return true;
  return false;
}

bool Client::operator!=(const Client& other)
{
  if (*this == other)
    return false;
  return true;
}

int Client::dummyFd = 4;