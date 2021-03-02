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
  // 改行コードが\rでも\nでも\r\nでもOKなように対応する場合
  // RFCは\r\n
  // requestLine_ = ReplaceString(requestLine_, "\r\n", "\n");
  // requestLine_ = ReplaceString(requestLine_, "\r", "\n");
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
    messageHeader_ = stringRecvData.substr(0, pos + 2);
    messageBodyStartPosition_ = pos + 4;
  }
}

std::string Client::getMessageHeader() const
{
  return messageHeader_;
}

bool Client::parseRequestTarget()
{
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
    std::cout << "[DEBUG]authority_ : " << authority_ << std::endl;
    return true;
  }
  // absolute-form
  // http://example.com/のような場合
  if (requestTarget_[0] != '/')
  {
    std::string::size_type pos = requestTarget_.find(std::string("//"));
    if (pos == std::string::npos)
      return false;
    pos = requestTarget_.find(std::string("/"), pos + 2);
    if (pos == std::string::npos)
      return false;        
    requestTarget_ = requestTarget_.substr(pos);
  }
  // origin-form
  // /insex.htmlのように/ではじまる場合
  if (requestTarget_[0] != '/')
    return false;
  // ?があった場合、前後に分割
  std::string::size_type pos = requestTarget_.find(std::string("?"));
  if (pos != std::string::npos)
  {
    query_ = requestTarget_.substr(pos + 1);
    requestTarget_ = requestTarget_.substr(0, pos - 1);
  }
  // std::cout << "[DEBUG]requestTarget_ : " << requestTarget_ << std::endl;
  // std::cout << "[DEBUG]query_ : " << query_ << std::endl;
  return SUCCESS;
}

std::string Client::getAbsolutePath() const
{
  return absolutePath_;
}

std::string Client::getQuery() const
{
  return query_;
}

std::string Client::getAuthority() const
{
  return authority_;
}

bool Client::parseHeader()
{
  std::string::size_type pos = messageHeader_.find("\r\n");
  std::string header = messageHeader_.substr(pos + 2);
  std::cout << "header\n" << header << std::endl;
  // 1行取り出す
  while ((pos = header.find("\r\n")) != std::string::npos)
  {
    std::string headerField = header.substr(0, pos);
    header = header.substr(pos + 2);
    // :の前を取り出す
    if ((pos = headerField.find(":")) == std::string::npos)
      return false;
    std::string fieldName = headerField.substr(0, pos);
    // :の後ろを取り出す
    std::string fieldValue = ft_trim(headerField.substr(pos + 1), " \t");
    std::cout << "fieldName : " << fieldName << std::endl;
    std::cout << "fieldValue : " << fieldValue << std::endl;

    // ここからmultimapにいれていく
  }
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
  messageBodyStartPosition_(0),
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
  // {
  //   Client c;
  //   std::string stringData1 = "CONNECT 153.127.88.54 HTTP/1.1\r\n\r\n";
  //   const char *data1 = stringData1.c_str();

  //   c.setData(data1, stringData1.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   c.parseRequestTarget();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  // std::cout << "----------------------------" << std::endl;
  // {
  //   Client c;
  //   std::string stringData1 = "CONNECT 153.127.88.54:80 HTTP/1.1\r\n\r\n";
  //   const char *data1 = stringData1.c_str();

  //   c.setData(data1, stringData1.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   c.parseRequestTarget();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  // std::cout << "----------------------------" << std::endl;
  // {
  //   Client c;
  //   std::string stringData1 = "CONNECT 153.127.88.54:80/index.html HTTP/1.1\r\n\r\n";
  //   const char *data1 = stringData1.c_str();

  //   c.setData(data1, stringData1.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   c.parseRequestTarget();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  // std::cout << "----------------------------" << std::endl;
  // {
  //   Client c;
  //   std::string stringData1 = "CONNECT 153.127.88.54/index.html HTTP/1.1\r\n\r\n";
  //   const char *data1 = stringData1.c_str();

  //   c.setData(data1, stringData1.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   c.parseRequestTarget();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  std::cout << "----------------------------" << std::endl;
  {
    Client c;
    std::string stringData1 = "GET / HTTP/1.1\r\nHost: server\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\nPOSTDATA";
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
    if (!c.parseHeader())
      return 1;
    // std::cout << c.getMessageHeader() << '\n';
  }
  std::cout << "----------------------------" << std::endl;
  // {
  //   Client c;
  //   std::string stringData1 = "GET /index.html HTTP/1.1\r\nHost: server\r\n\r\nPOSTDATA";
  //   const char *data1 = stringData1.c_str();

  //   c.setData(data1, stringData1.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   c.parseRequestTarget();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  // std::cout << "----------------------------" << std::endl;
  // {
  //   Client c;
  //   std::string stringData1 = "GET http://127.0.0.1/index.html HTTP/1.1\r\nHost: server\r\n\r\nPOSTDATA";
  //   const char *data1 = stringData1.c_str();

  //   c.setData(data1, stringData1.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   c.parseRequestTarget();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  // std::cout << "----------------------------" << std::endl;
  // {
  //   Client c;
  //   std::string stringData1 = "GET http://127.0.0.1/index.html?name=hkamiya HTTP/1.1\r\nHost: server\r\n\r\nPOSTDATA";
  //   const char *data1 = stringData1.c_str();

  //   c.setData(data1, stringData1.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   c.parseRequestTarget();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  // std::cout << "----------------------------" << std::endl;
  // {
  //   Client c;
  //   std::string stringData1 = "GET / HTTP/1.0\r\n";
  //   std::string stringData2 = "Host: server\r\n\r\nPOSTDATA";
  //   const char *data1 = stringData1.c_str();
  //   const char *data2 = stringData2.c_str();

  //   c.setData(data1, stringData1.size());
  //   c.setData(data2, stringData2.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  // std::cout << "----------------------------" << std::endl;
  // {
  //   Client c;
  //   std::string stringData1 = "GET / HTTP/0.9\r\nHost: server\r\n\r\nPOSTDATA";
  //   const char *data1 = stringData1.c_str();

  //   c.setData(data1, stringData1.size() + 1);
  //   c.getRequestLineFromRecvData();
  //   if (!c.parseRequestLine())
  //     return 1;
  //   std::cout << "method = " << c.getMethod() << std::endl;
  //   std::cout << "HTTPVersion = " << c.getHTTPVersion() << std::endl;
  //   std::cout << std::endl;
  //   c.getHeaderFromRecvData();
  //   std::cout << c.getMessageHeader() << '\n';
  // }
  return 0;
}