#include "Client.hpp"

void Client::generateTestValue(int testPatternOfClient)
{
  switch (testPatternOfClient)
  {
  case 1:
    port_ = 5000;
    host_ = "*";
    break;

  case 2:
    port_ = 5001;
    host_ = "*";
    break;

  case 3:
    port_ = 5000;
    host_ = "127.0.0.1";
    break;

  case 4:
    port_ = 5001;
    host_ = "127.0.0.1";
    break;

  default:
    break;
  }
}

Client::Client(int testPatternOfClient, int testPatternOfHTTPMessageParser)
{
  this->generateTestValue(testPatternOfClient);
  this->hmp_ = HTTPMessageParser(testPatternOfHTTPMessageParser);
}

Client::Client(int testPatternOfClient, std::string testPatternOfHTTPMessageParserConf, int testPatternOfHTTPMessageParser)
{
  this->generateTestValue(testPatternOfClient); 
  this->hmp_ = HTTPMessageParser("serverNames", testPatternOfHTTPMessageParser);
}

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