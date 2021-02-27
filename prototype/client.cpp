#include "client.hpp"

bool Client::setData(const char *data, size_t size)
{
  if (recvData_ == NULL)
  {
    if (!(recvData_ = (char *)malloc(size)))
      throw std::runtime_error("malloc faild.");
    ft_memcpy(recvData_, data, size);
    recvDataSize_ = size;
  }
  else
  {
    char *tmp;
    if (!(tmp = (char *)malloc(size + recvDataSize_)))
      throw std::runtime_error("malloc faild.");
    ft_memcpy(tmp, recvData_, recvDataSize_);
    ft_memcpy(tmp + recvDataSize_, data, size);
    free(recvData_);
    recvData_ = tmp;
    recvDataSize_ = size + recvDataSize_;
  }
  return true;
}

void Client::getRequestLineFromRecvData()
{
  size_t i = 0;
  char *tmp = recvData_;
  while (i < recvDataSize_)
  {
    if (*tmp == '\r' || *tmp == '\n')
    {
      std::string stringRecvData(recvData_);
      requestLine_ = stringRecvData.substr(0, i);
    }
    tmp++;
    i++;
  }
  // std::cout << "[DEBUG]requestLine_ : " << requestLine_ << std::endl;
}

bool Client::parseRequestLine()
{
  // 改行コードが\rでも\nでも\r\nでもOKなように対応
  // RFCは\r\n
  requestLine_ = ReplaceString(requestLine_, "\r\n", "\n");
  requestLine_ = ReplaceString(requestLine_, "\r", "\n");
  std::string::size_type firstSpPos = requestLine_.find(std::string(" "));
  if (firstSpPos == std::string::npos)
    return FAILURE;
  std::string method = requestLine_.substr(0, firstSpPos);
  // std::cout << "[DEBUG]method = " << method << std::endl;
  if (method == "GET")
    method_ = client::GET;
  else if (method == "HEAD")
    method_ = client::HEAD;
  else if (method == "POST")
    method_ = client::POST;
  else if (method == "PUT")
    method_ = client::PUT;
  else if (method == "DELETE")
    method_ = client::DELETE;
  else if (method == "CONNECT")
    method_ = client::CONNECT;
  else if (method == "OPTIONS")
    method_ = client::OPTIONS;
  else if (method == "TRACE")
    method_ = client::TRACE;
  else
  {
    method_ = client::OTHER;
    return FAILURE;
  }
  std::string::size_type secondSpPos = requestLine_.find(std::string(" "), firstSpPos + 1);
  if (secondSpPos == std::string::npos)
    return FAILURE;
  requestTarget_ = requestLine_.substr(firstSpPos + 1, secondSpPos - firstSpPos - 1);
  // std::cout << "[DEBUG]requestTarget_ = \"" << requestTarget_ << "\"" << std::endl;
  std::string::size_type CRLFPos = requestLine_.find(std::string("\n"), secondSpPos + 1);
  if (CRLFPos == std::string::npos)
    return FAILURE;
  HTTPVersion_ = requestLine_.substr(secondSpPos + 1, CRLFPos - secondSpPos - 1);
  // std::cout << "[DEBUG]HTTPVersion_ = \"" << HTTPVersion_ << "\"" << std::endl;
  if (HTTPVersion_ != "HTTP/1.1")
    HTTPVersion_ = "HTTP/1.0";
  return SUCCESS;
}

enum client::method Client::getMethod() const
{
  return method_;
}

std::string Client::getHTTPVersion() const
{
  return HTTPVersion_;
}

void Client::getHeaderFromRecvData()
{
  std::string stringRecvData(recvData_);
  std::string::size_type pos = stringRecvData.find(std::string("\r\n\r\n"));
  if (pos != std::string::npos)
  {
    messageHeader_ = stringRecvData.substr(0, pos);
    messageHeaderEndPosition_ = pos;
  }
}

std::string Client::getMessageHeader() const
{
  return messageHeader_;
}

bool Client::parseRequestTarget()
{
  std::cout << "requestTarget_ : " << requestTarget_ << std::endl;

  if (method_ == client::OPTIONS && requestTarget_ == "*")
  {
    absolutePath_ = "*";
    return SUCCESS;
  }
  if (method_ == client::CONNECT)
  {
    if(!isAuthority(requestTarget_))
      return false;
    authority_ = getHostAndPort(requestTarget_);
  }
  else
  {
    // ここから書く
  }
  std::cout << "authority_ : " << authority_ << std::endl;
  return SUCCESS;
}

bool Client::parseHeader()
{
  return SUCCESS;
}

char *Client::getMessageBody() const
{
  return messageBody_;
}

bool Client::isAuthority(std::string requestTarget)
{
  // 正しいケース
  // 153.127.88.54
  // 153.127.88.54:80
  size_t i = 0;
  while (std::isdigit(requestTarget[i]))
    i++;
  if (requestTarget[i] != '.')
    return false;
  i++;
  while (std::isdigit(requestTarget[i]))
    i++;
  if (requestTarget[i] != '.')
    return false;
  i++;
  while (std::isdigit(requestTarget[i]))
    i++;
  if (requestTarget[i] != '.')
    return false;
  i++;
  while (std::isdigit(requestTarget[i]))
    i++;
  if (i == requestTarget.size() || requestTarget[i] == '/')
    return true;
  if (requestTarget[i] != ':')
    return false;
  i++;
  while (std::isdigit(requestTarget[i]))
    i++;
  if (i == requestTarget.size() || requestTarget[i] == '/')
    return true;
  return false;
}

std::string Client::getHostAndPort(std::string requestTarget)
{
  size_t i = 0;
  while (std::isdigit(requestTarget[i]))
    i++;
  i++;
  while (std::isdigit(requestTarget[i]))
    i++;
  i++;
  while (std::isdigit(requestTarget[i]))
    i++;
  i++;
  while (std::isdigit(requestTarget[i]))
    i++;
  if (i == requestTarget.size() || requestTarget[i] == '/')
    return requestTarget.substr(0, i);
  i++;
  while (std::isdigit(requestTarget[i]))
    i++;
  return requestTarget.substr(0, i);
}

Client::Client() :
  recvData_(NULL),
  recvDataSize_(0),
  messageHeaderEndPosition_(0),
  method_(client::OTHER),
  messageBody_(NULL)
{
  (void)method_;
}

Client::~Client()
{
  free(recvData_);
  free(messageBody_);
}

// clang++ -g -Wall -Wextra -Werror -fsanitize=address,leak client.cpp ft_memcpy.cpp ft_replaceString.cpp
int main()
{
  {
    Client c;
    std::string stringData1 = "CONNECT 153.127.88.54 HTTP/1.1\r\n\r\n";
    const char *data1 = stringData1.c_str();

    c.setData(data1, stringData1.size() + 1);
    c.getRequestLineFromRecvData();
    if (!c.parseRequestLine())
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    c.getHeaderFromRecvData();
    c.parseRequestTarget();
    std::cout << c.getMessageHeader() << '\n';
  }
  std::cout << "----------------------------" << std::endl;
  {
    Client c;
    std::string stringData1 = "CONNECT 153.127.88.54:80 HTTP/1.1\r\n\r\n";
    const char *data1 = stringData1.c_str();

    c.setData(data1, stringData1.size() + 1);
    c.getRequestLineFromRecvData();
    if (!c.parseRequestLine())
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    c.getHeaderFromRecvData();
    c.parseRequestTarget();
    std::cout << c.getMessageHeader() << '\n';
  }
  std::cout << "----------------------------" << std::endl;
  {
    Client c;
    std::string stringData1 = "CONNECT 153.127.88.54:80/index.html HTTP/1.1\r\n\r\n";
    const char *data1 = stringData1.c_str();

    c.setData(data1, stringData1.size() + 1);
    c.getRequestLineFromRecvData();
    if (!c.parseRequestLine())
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    c.getHeaderFromRecvData();
    c.parseRequestTarget();
    std::cout << c.getMessageHeader() << '\n';
  }
  std::cout << "----------------------------" << std::endl;
  {
    Client c;
    std::string stringData1 = "CONNECT 153.127.88.54/index.html HTTP/1.1\r\n\r\n";
    const char *data1 = stringData1.c_str();

    c.setData(data1, stringData1.size() + 1);
    c.getRequestLineFromRecvData();
    if (!c.parseRequestLine())
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    c.getHeaderFromRecvData();
    c.parseRequestTarget();
    std::cout << c.getMessageHeader() << '\n';
  }
  std::cout << "----------------------------" << std::endl;
  {
    Client c;
    std::string stringData1 = "GET / HTTP/1.1\r\nHost: server\r\n\r\nPOSTDATA";
    const char *data1 = stringData1.c_str();

    c.setData(data1, stringData1.size() + 1);
    c.getRequestLineFromRecvData();
    if (!c.parseRequestLine())
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    c.getHeaderFromRecvData();
    c.parseRequestTarget();
    std::cout << c.getMessageHeader() << '\n';
  }
  std::cout << "----------------------------" << std::endl;
  {
    Client c;
    std::string stringData1 = "GET / HTTP/1.0\r\n";
    std::string stringData2 = "Host: server\r\n\r\nPOSTDATA";
    const char *data1 = stringData1.c_str();
    const char *data2 = stringData2.c_str();

    c.setData(data1, stringData1.size());
    c.setData(data2, stringData2.size() + 1);
    c.getRequestLineFromRecvData();
    if (!c.parseRequestLine())
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    c.getHeaderFromRecvData();
    std::cout << c.getMessageHeader() << '\n';
  }
  std::cout << "----------------------------" << std::endl;
  {
    Client c;
    std::string stringData1 = "GET / HTTP/0.9\r\nHost: server\r\n\r\nPOSTDATA";
    const char *data1 = stringData1.c_str();

    c.setData(data1, stringData1.size() + 1);
    c.getRequestLineFromRecvData();
    if (!c.parseRequestLine())
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    c.getHeaderFromRecvData();
    std::cout << c.getMessageHeader() << '\n';
  }
  return 0;
}