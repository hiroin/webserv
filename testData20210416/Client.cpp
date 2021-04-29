#include "Client.hpp"

void Client::generateTestValue(int testPatternOfClient)
{
  switch (testPatternOfClient)
  {
  case 1:
    port = 5000;
    host = "*";
    break;

  case 2:
    port = 5001;
    host = "*";
    break;

  case 3:
    port = 5000;
    host = "127.0.0.1";
    break;

  case 4:
    port = 5001;
    host = "127.0.0.1";
    break;

  default:
    break;
  }
}

Client::Client(int testPatternOfClient, int testPatternOfHTTPMessageParser)
{
  this->generateTestValue(testPatternOfClient);
  this->hmp = HTTPMessageParser(testPatternOfHTTPMessageParser);
}

Client::Client(int testPatternOfClient, std::string testPatternOfHTTPMessageParserConf, int testPatternOfHTTPMessageParser)
{
  this->generateTestValue(testPatternOfClient);
  this->hmp = HTTPMessageParser("serverNames", testPatternOfHTTPMessageParser);
}

Client::Client()
{
  HTTPMessageParser hmp(1);
  hmp = hmp;

  //テスト用
  socketFd = dummyFd;
  dummyFd++;
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

int Client::dummyFd = 4;