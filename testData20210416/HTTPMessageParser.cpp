#include "HTTPMessageParser.hpp"

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

std::string HTTPMessageParser::getPathinfo() const
{
  return pathinfo_;
}

std::map<std::string, std::string> HTTPMessageParser::getHeaders() const
{
  return headers_;
}

HTTPMessageParser::HTTPMessageParser()
{

}

HTTPMessageParser::HTTPMessageParser(int testPattern)
{
  switch (testPattern)
  {
  case 1:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/";
    headers_["host"] = "127.0.0.1";
    headers_["accept-language"] = "127.0.0.1";
    break;

  case 2:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/index.html";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/index.html";
    headers_["host"] = "127.0.0.1";
    break;

  case 3:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/aaa/index.html";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/aaa/index.html";
    headers_["host"] = "127.0.0.1";
    break;

  case 4:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/aaa/";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/aaa/";
    headers_["host"] = "127.0.0.1";
    break;

  case 5:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/aa/";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/aa/";
    headers_["host"] = "127.0.0.1";
    break;

  case 6:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/aa/index.html";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/aa/index.html";
    headers_["host"] = "127.0.0.1";
    break;

  case 7:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/www/aaa/index.html";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/www/aaa/index.html";
    headers_["host"] = "127.0.0.1";
    break;

  case 8:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/bbb/index.html";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/bbb/index.html";
    headers_["host"] = "127.0.0.1";
    break;

  case 9:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/bb/index.html";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/bb/index.html";
    headers_["host"] = "127.0.0.1";
    break;

  case 10:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/www/bb/index.html";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/www/bb/index.html";
    headers_["host"] = "127.0.0.1";
    break;

  case 11:
    method_ = httpMessageParser::GET;
    requestTarget_ = "/1.jpg";
    HTTPVersion_ = "HTTP/1.1";
    absolutePath_ = "/1.jpg";
    headers_["host"] = "127.0.0.1";
    break;

  default:
    break;
  }
}

HTTPMessageParser::HTTPMessageParser(std::string conf, int testPattern)
{
  if (conf == "serverNames")
  {
    switch (testPattern)
    {
    case 1:
      method_ = httpMessageParser::GET;
      requestTarget_ = "/index.html";
      HTTPVersion_ = "HTTP/1.1";
      absolutePath_ = "/index.html";
      headers_["host"] = "127.0.0.1";
      break;

    case 2:
      method_ = httpMessageParser::GET;
      requestTarget_ = "/index.html";
      HTTPVersion_ = "HTTP/1.1";
      absolutePath_ = "/index.html";
      headers_["host"] = "server10";
      break;

    case 3:
      method_ = httpMessageParser::GET;
      requestTarget_ = "/index.html";
      HTTPVersion_ = "HTTP/1.1";
      absolutePath_ = "/index.html";
      headers_["host"] = "server11";
      break;

    case 4:
      method_ = httpMessageParser::GET;
      requestTarget_ = "/index.html";
      HTTPVersion_ = "HTTP/1.1";
      absolutePath_ = "/index.html";
      headers_["host"] = "server20";
      break;

    case 5:
      method_ = httpMessageParser::GET;
      requestTarget_ = "/index.html";
      HTTPVersion_ = "HTTP/1.1";
      absolutePath_ = "/index.html";
      headers_["host"] = "server";
      break;

    case 6:
      method_ = httpMessageParser::GET;
      requestTarget_ = "/index.html";
      HTTPVersion_ = "HTTP/1.1";
      absolutePath_ = "/index.html";
      headers_["host"] = "127.0.0.1:5000";
      break;

    default:
      break;
    }
  }
}

HTTPMessageParser::  ~HTTPMessageParser()
{

}