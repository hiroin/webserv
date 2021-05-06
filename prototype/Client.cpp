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

void Client::initClient()
{
  if (socketFd > 0)
  {
    close(socketFd);
    socketFd = -1;
  }
  if (readFd > 0)
  {
    close(readFd);
    readFd = -1;
  }
  if (writeFd > 0)
  {
    close(writeFd);
    writeFd = -1;
  }
  if (pipeReadFd > 0)
  {
    close(pipeReadFd);
    pipeReadFd = -1;
  }
  if (pipeWriteFd > 0)
  {
    close(pipeWriteFd);
    pipeWriteFd = -1;
  }
  port = -1;
  host.clear();
  ip.clear();
  receivedData.clearData();
  readDataFromFd.clearData();
  hmp.clearData();
  gc.clear();
  status = PARSE_STARTLINE;
  bCGI = false;
  bChunked = false;
  responseCode = -1;
  responseFileSize = 0;
  responseMessege.clear();
  body.clear();
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