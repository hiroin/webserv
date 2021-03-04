#include "HTTPMessageParser.hpp"

bool HTTPMessageParser::parseRequestLine(std::string requestLine)
{
  requestLine_ = requestLine;
  std::string::size_type firstSpPos = requestLine.find(std::string(" "));
  if (firstSpPos == std::string::npos)
    return FAILURE;
  std::string method = requestLine.substr(0, firstSpPos);
  // std::cout << "[DEBUG]method = " << method << std::endl;
  if (method == "GET")
    method_ = httpMessageParser::GET;
  else if (method == "HEAD")
    method_ = httpMessageParser::HEAD;
  else if (method == "POST")
    method_ = httpMessageParser::POST;
  else if (method == "PUT")
    method_ = httpMessageParser::PUT;
  else if (method == "DELETE")
    method_ = httpMessageParser::DELETE;
  else if (method == "CONNECT")
    method_ = httpMessageParser::CONNECT;
  else if (method == "OPTIONS")
    method_ = httpMessageParser::OPTIONS;
  else if (method == "TRACE")
    method_ = httpMessageParser::TRACE;
  else
  {
    method_ = httpMessageParser::OTHER;
    return FAILURE;
  }
  std::string::size_type secondSpPos = requestLine.find(std::string(" "), firstSpPos + 1);
  if (secondSpPos == std::string::npos)
    return FAILURE;
  requestTarget_ = requestLine.substr(firstSpPos + 1, secondSpPos - firstSpPos - 1);
  // std::cout << "[DEBUG]requestTarget_ = \"" << requestTarget_ << "\"" << std::endl;
  HTTPVersion_ = requestLine.substr(secondSpPos + 1);
  // std::cout << "[DEBUG]HTTPVersion_ = \"" << HTTPVersion_ << "\"" << std::endl;
  if (HTTPVersion_ == "HTTP/1.1" || HTTPVersion_ == "HTTP/1.0")
    return SUCCESS;
  return FAILURE;
}

enum httpMessageParser::method HTTPMessageParser::getMethod() const
{
  return method_;
}

std::string HTTPMessageParser::getRequestTarget() const
{
  return requestTarget_;
}

std::string HTTPMessageParser::getHTTPVersion() const
{
  return HTTPVersion_;
}

bool HTTPMessageParser::parseRequestTarget(std::string requestTarget)
{
  if (method_ == httpMessageParser::OPTIONS && requestTarget == "*")
  {
    absolutePath_ = "*";
    return true;
  }
  if (method_ == httpMessageParser::CONNECT)
  {
    if(!isAuthority(requestTarget))
      return false;
    authority_ = getHostAndPort(requestTarget);
    // std::cout << "[DEBUG]authority_ : " << authority_ << std::endl;
    return true;
  }
  // absolute-form
  // http://example.com/のような場合
  if (requestTarget[0] != '/')
  {
    std::string::size_type pos = requestTarget.find(std::string("//"));
    if (pos == std::string::npos)
      return false;
    pos = requestTarget.find(std::string("/"), pos + 2);
    if (pos == std::string::npos)
      return false;        
    requestTarget = requestTarget_.substr(pos);
  }
  // origin-form
  // /insex.htmlのように/ではじまる場合
  if (requestTarget[0] != '/')
    return false;
  // ?があった場合、前後に分割
  std::string::size_type pos = requestTarget.find(std::string("?"));
  if (pos != std::string::npos)
  {
    absolutePath_ = requestTarget.substr(0, pos - 1);
    query_ = requestTarget.substr(pos + 1);
  }
  else
    absolutePath_ = requestTarget;
  // std::cout << "[DEBUG]absolutePath_ : " << absolutePath_ << std::endl;
  // std::cout << "[DEBUG]query_ : " << query_ << std::endl;
  return SUCCESS;
}

std::string HTTPMessageParser::getAbsolutePath() const
{
  return absolutePath_;
}

std::string HTTPMessageParser::getQuery() const
{
  return query_;
}

std::string HTTPMessageParser::getAuthority() const
{
  return authority_;
}

bool HTTPMessageParser::parseHeader(std::string header)
{
  (void)header;
  return SUCCESS;
}

bool HTTPMessageParser::isAuthority(std::string requestTarget)
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

std::string HTTPMessageParser::getHostAndPort(std::string requestTarget)
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

HTTPMessageParser::HTTPMessageParser() :
  method_(httpMessageParser::OTHER)
{
//  (void)method_;
}

HTTPMessageParser::~HTTPMessageParser()
{
}

// clang++ -g -Wall -Wextra -Werror -std=c++98 -fsanitize=address,leak HTTPMessageParser.cpp ft_memcpy.cpp ft_replaceString.cpp -D MAIN
// "GET / HTTP/1.1\r\nHost: server\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\nPOSTDATA"
#ifdef MAIN
int main()
{
  std::cout << "--start--------------------------" << std::endl;
  {
    HTTPMessageParser c;
    if (!c.parseRequestLine("GET / HTTP/1.1"))
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "request-target = " << c.getRequestTarget() << std::endl;
    std::cout << "HTTP-version = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    if (!c.parseRequestTarget(c.getRequestTarget()))
      return 1;
    std::cout << "absolute-path = " << c.getAbsolutePath() << std::endl;
    std::cout << "query = " << c.getQuery() << std::endl;
  }
  std::cout << "--end----------------------------" << std::endl;
  std::cout << "--start--------------------------" << std::endl;
  {
    HTTPMessageParser c;
    if (!c.parseRequestLine("GET /index.html HTTP/1.1"))
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "request-target = " << c.getRequestTarget() << std::endl;
    std::cout << "HTTP-version = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    if (!c.parseRequestTarget(c.getRequestTarget()))
      return 1;
    std::cout << "absolute-path = " << c.getAbsolutePath() << std::endl;
    std::cout << "query = " << c.getQuery() << std::endl;
  }
  std::cout << "--end----------------------------" << std::endl;
  std::cout << "--start--------------------------" << std::endl;
  {
    HTTPMessageParser c;
    if (!c.parseRequestLine("GET http://127.0.0.1/index.html HTTP/1.1"))
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "request-target = " << c.getRequestTarget() << std::endl;
    std::cout << "HTTP-version = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    if (!c.parseRequestTarget(c.getRequestTarget()))
      return 1;
    std::cout << "absolute-path = " << c.getAbsolutePath() << std::endl;
    std::cout << "query = " << c.getQuery() << std::endl;
  }
  std::cout << "--end----------------------------" << std::endl;
  std::cout << "--start--------------------------" << std::endl;
  {
    HTTPMessageParser c;
    if (!c.parseRequestLine("GET http://127.0.0.1/www/index.html HTTP/1.1"))
      return 1;
    std::cout << "method = " << c.getMethod() << std::endl;
    std::cout << "request-target = " << c.getRequestTarget() << std::endl;
    std::cout << "HTTP-version = " << c.getHTTPVersion() << std::endl;
    std::cout << std::endl;
    if (!c.parseRequestTarget(c.getRequestTarget()))
      return 1;
    std::cout << "absolute-path = " << c.getAbsolutePath() << std::endl;
    std::cout << "query = " << c.getQuery() << std::endl;
  }
  std::cout << "--end----------------------------" << std::endl;
  return 0;
}
#endif