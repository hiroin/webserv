#include "HTTPMessageParser.hpp"

bool HTTPMessageParser::parseRequestLine(const std::string requestLine)
{
  // std::cout << "[DEBUG]requestLine = " << requestLine << std::endl;
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
    std::string::size_type pos;
    pos = requestTarget.find(std::string(":///"));
    if (pos != std::string::npos)
      return false;
    pos = requestTarget.find(std::string("://"));
    if (pos == std::string::npos || pos == 0)
      return false;
    pos = requestTarget.find(std::string("/"), pos + 3);
    if (pos == std::string::npos)
      return false;        
    requestTarget = requestTarget.substr(pos);
  }
  // origin-form
  // /insex.htmlのように/ではじまる場合
  if (requestTarget[0] != '/')
    return false;
  // ?があった場合、前後に分割
  std::string::size_type pos = requestTarget.find(std::string("?"));
  if (pos != std::string::npos)
  {
    absolutePath_ = requestTarget.substr(0, pos);
    query_ = requestTarget.substr(pos + 1);
  }
  else
    absolutePath_ = requestTarget;
  // std::cout << "[DEBUG]absolutePath_ : " << absolutePath_ << std::endl;
  // std::cout << "[DEBUG]query_ : " << query_ << std::endl;
  // pathinfoを抽出
  for (std::vector<std::string>::const_iterator itr = filenameExtensions_.begin(); itr != filenameExtensions_.end(); ++itr)
  {
    std::string::size_type pos = absolutePath_.rfind(*itr);
    if (pos != std::string::npos)
    {
      pathinfo_ = absolutePath_.substr(pos + itr->size());
      absolutePath_ = absolutePath_.substr(0, pos + itr->size() - 1);
      // std::cout << "[DEBUG]absolutePath_ : " << absolutePath_ << std::endl;
      // std::cout << "[DEBUG]pathinfo_ : " << pathinfo_ << std::endl;
      break;
    }
  }
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

std::string HTTPMessageParser::getPathinfo() const
{
  return pathinfo_;
}

bool HTTPMessageParser::parseHeader(std::string header)
{
  if (!validationHeader(header))
    return FAILURE;
  std::string::size_type pos = header.find(std::string(":"));
  if (pos == std::string::npos)
    return FAILURE;
  std::string fieldName = header.substr(0, pos);
  std::string fieldValue  = header.substr(pos + 1);
  fieldValue = ft_trim(fieldValue, " \t");
  if (!pushFieldNameAndValue(fieldName, fieldValue))
    return FAILURE;
  return SUCCESS;
}

std::map<std::string, std::string> HTTPMessageParser::getHeaders() const
{
  return headers_;
}

void HTTPMessageParser::clearData()
{
  requestLine_.clear();
  method_ = httpMessageParser::OTHER;
  requestTarget_.clear();
  HTTPVersion_.clear();
  absolutePath_.clear();
  query_.clear();
  authority_.clear();
  headers_.clear();
}

// headersのkeyにhostがあれば、その値を確認し、不正であればfalseを返す。
bool HTTPMessageParser::correctHostValue(std::map<std::string, std::string> headers) const
{
  for (std::map<std::string, std::string>::iterator itr = headers.begin();
    itr != headers.end();
    itr++
  )
  {
    if (itr->first == "host")
    {
      if (!correctHostValue(itr->second))
        return false;
    }
  }
  return true;
}

bool HTTPMessageParser::correctHostValue(std::string hostValue) const
{
  std::string::size_type pos = hostValue.find(std::string(":"));
  if (pos != std::string::npos)
    hostValue = hostValue.substr(0, pos);
  if (correctUrihost(hostValue))
    return true;
  return false;
}

bool HTTPMessageParser::correctUrihost(std::string uriHost) const
{
  if (uriHost.size() == 0)
    return false;
  for (std::string::iterator itr = uriHost.begin();
    itr != uriHost.end();
    itr++
  )
  {
    if (!(ft_isunreserved(*itr) || *itr == '%' || ft_issubdelims(*itr)))
      return false;
  }
  return true;
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

bool HTTPMessageParser::validationHeader(std::string header)
{
  size_t i = 0;
  if (!ft_istchar(header[i++]))
    return FAILURE;
  while (ft_istchar(header[i]))
    i++;
  if (header[i++] != ':')
    return FAILURE;
  while (ft_isspase_and_htab(header[i++]))
    ;
  if (!ft_isvchar(header[i++]))
    return FAILURE;
  while (ft_isvchar(header[i]) || ft_isspase_and_htab(header[i]))
    i++;
  if (i == header.size())
    return SUCCESS;
  return FAILURE;
}

bool HTTPMessageParser::pushFieldNameAndValue(std::string fieldName, std::string fieldValue)
{
  std::transform(fieldName.begin(), fieldName.end(), fieldName.begin(), tolower);
  if (fieldName == "accept" \
    || fieldName == "accept-charsets" \
    || fieldName == "accept-language"
    )
  {
    std::map<std::string, std::string>::const_iterator itr = headers_.find(fieldName);
    if (itr != headers_.end())
    {
      headers_[fieldName] += ',';
      headers_[fieldName] += fieldValue;
    }
    else
      headers_[fieldName] = fieldValue;
  }
  else if (fieldName == "authorization" \
    || fieldName == "content-length" \
    || fieldName == "content-type" \
    || fieldName == "date" \
    || fieldName == "host" \
    || fieldName == "referer" \
    || fieldName == "transfer-encoding" \
    || fieldName == "user-agent"
    )
  {
    std::map<std::string, std::string>::const_iterator itr = headers_.find(fieldName);
    if (itr != headers_.end())
      return FAILURE;
    headers_[fieldName] = fieldValue;
  }
  else
  {
    std::map<std::string, std::string>::const_iterator itr = headers_.find(fieldName);
    if (itr != headers_.end())
      return FAILURE;
    headers_[fieldName] = fieldValue;
  }
  return SUCCESS;
}

HTTPMessageParser::HTTPMessageParser() :
  method_(httpMessageParser::OTHER)
{
  filenameExtensions_.push_back(".htm/");  
  filenameExtensions_.push_back(".html/");  
  filenameExtensions_.push_back(".php/");  
  filenameExtensions_.push_back(".cgi/");  
}

HTTPMessageParser::~HTTPMessageParser()
{
}

// clang++ -g -Wall -Wextra -Werror -std=c++98 -fsanitize=address,leak HTTPMessageParser.cpp ft_memcpy.cpp ft_replaceString.cpp ft_istchar.cpp ft_isspase_and_htab.cpp ft_isvchar.cpp ft_trim.cpp -D MAIN
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
  std::cout << "--start--------------------------" << std::endl;
  {
    HTTPMessageParser c;
    if (!c.parseRequestLine("GET http:/// HTTP/1.1"))
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

// int main()
// {
//   std::cout << "--start--------------------------" << std::endl;
//   {
//     HTTPMessageParser c;
//     if (!c.parseHeader("Host: 127.0.0.1"))
//       return 1;
//     // if (!c.parseHeader("Host: 127.0.0.1"))
//     //   return 1;
//     if (!c.parseHeader("Accept: */*"))
//       return 1;
//     // if (!c.parseHeader("Accept: */*"))
//     //   return 1;
//     if (!c.parseHeader("User-Agent: Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1)"))
//       return 1;
//     std::map<std::string, std::string> headers = c.getHeaders();
//     for(std::map<std::string, std::string>::const_iterator itr = headers.begin(); itr != headers.end(); ++itr)
//     {
//       std::cout << "\"" << itr->first << "\" = \"" << itr->second << "\"\n";
//     }
//   }
//   std::cout << "--end----------------------------" << std::endl;
// }