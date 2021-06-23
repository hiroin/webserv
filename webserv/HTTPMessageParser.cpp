#include "HTTPMessageParser.hpp"

int HTTPMessageParser::parseRequestLine(const std::string requestLine)
{
  // std::cout << "[DEBUG]requestLine = " << requestLine << std::endl;
  requestLine_ = requestLine;
  std::string::size_type firstSpPos = requestLine.find(std::string(" "));
  if (firstSpPos == std::string::npos)
    return 400;
  if (!ft_istchar(requestLine_[0]))
    return 400;
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
    return 501;
  }
  std::string::size_type secondSpPos = requestLine.find(std::string(" "), firstSpPos + 1);
  if (secondSpPos == std::string::npos)
    return 400;
  requestTarget_ = requestLine.substr(firstSpPos + 1, secondSpPos - firstSpPos - 1);
  if (method_ == httpMessageParser::PUT && requestTarget_[requestTarget_.size() - 1] == '/')
    return 400;
  if (requestTarget_.size() > httpMessageParser::MAX_URI_SIZE)
    return 414;    
  // std::cout << "[DEBUG]requestTarget_ = \"" << requestTarget_ << "\"" << std::endl;
  HTTPVersion_ = requestLine.substr(secondSpPos + 1);
  // std::cout << "[DEBUG]HTTPVersion_ = \"" << HTTPVersion_ << "\"" << std::endl;
  if (HTTPVersion_ == "HTTP/1.1")
    return 200;
  if (HTTPVersion_[0] == 'H' &&
    HTTPVersion_[1] == 'T' &&
    HTTPVersion_[2] == 'T' &&
    HTTPVersion_[3] == 'P' &&
    HTTPVersion_[4] == '/' &&
    std::isdigit(HTTPVersion_[5]) &&
    HTTPVersion_[6] == '.' &&
    std::isdigit(HTTPVersion_[7])
    )
  {
    return 505;
  }
  return 400;
}

std::string HTTPMessageParser::getRequestLine() const
{
  return requestLine_;
}

enum httpMessageParser::method HTTPMessageParser::getMethod() const
{
  return method_;
}

std::string HTTPMessageParser::getMethodString() const
{
  if (method_ == httpMessageParser::GET)
    return "GET";
  else if (method_ == httpMessageParser::HEAD)
    return "HEAD";
  else if (method_ == httpMessageParser::POST)
    return "POST";
  else if (method_ == httpMessageParser::PUT)
    return "PUT";
  else if (method_ == httpMessageParser::DELETE)
    return "DELETE";
  else if (method_ == httpMessageParser::CONNECT)
    return "CONNECT";
  else if (method_ == httpMessageParser::OPTIONS)
    return "OPTIONS";
  else if (method_ == httpMessageParser::TRACE)
    return "TRACE";
  return "";
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

int HTTPMessageParser::parseHeader(std::string header)
{
  if (!validationHeader(header))
    return 200;
  std::string::size_type pos = header.find(std::string(":"));
  if (pos == std::string::npos)
    return 200;
  std::string fieldName = header.substr(0, pos);
  std::string fieldValue  = header.substr(pos + 1);
  fieldValue = ft_trim(fieldValue, " \t");
  return (pushFieldNameAndValue(fieldName, fieldValue));
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
  body_.clear();
}

bool HTTPMessageParser::isIllegalValueOfHostHeader(std::string headerField) const
{
  std::string s = headerField;
  std::transform(s.begin(), s.end(), s.begin(), tolower);
  std::string::size_type pos = s.find(std::string("host:"));
  if (pos == std::string::npos)
    return false;
  std::string fieldValue  = headerField.substr(pos + 5);
  fieldValue = ft_trim(fieldValue, " \t");
  if (correctHostValue(fieldValue))
    return false;
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
  while (ft_isspase_and_htab(header[i]))
    i++;
  if (!ft_isvchar(header[i++]))
    return FAILURE;
  while (ft_isvchar(header[i]) || ft_isspase_and_htab(header[i]))
    i++;
  if (i == header.size())
    return SUCCESS;
  return FAILURE;
}

int HTTPMessageParser::pushFieldNameAndValue(std::string fieldName, std::string fieldValue)
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
    || fieldName == "referer" \
    || fieldName == "transfer-encoding" \
    || fieldName == "user-agent"
    )
  {
    std::map<std::string, std::string>::const_iterator itr = headers_.find(fieldName);
    if (itr != headers_.end())
      return 200;
    headers_[fieldName] = fieldValue;
  }
  else if (fieldName == "host")
  {
    std::map<std::string, std::string>::const_iterator itr = headers_.find(fieldName);
    if (itr != headers_.end())
      return 400;
    headers_[fieldName] = fieldValue;
  }  
  else
  {
    std::map<std::string, std::string>::const_iterator itr = headers_.find(fieldName);
    if (itr != headers_.end())
      return 200;
    headers_[fieldName] = fieldValue;
  }
  return 200;
}

int HTTPMessageParser::isInvalidHeader()
{
  if (!isSetHostHeader())
    return 400;
  return (isInvalidHeaderValue());
}

bool HTTPMessageParser::isSetHostHeader()
{
  for (std::map<std::string, std::string>::iterator itr = headers_.begin();
    itr != headers_.end();
    itr++
  )
  {
    if (itr->first == "host")
      return true;
  }
  return false;
}

int HTTPMessageParser::isInvalidHeaderValue()
{
  for (std::map<std::string, std::string>::iterator itr = headers_.begin();
    itr != headers_.end();
    itr++
  )
  {
    if (itr->first == "content-length" && ft_stoi(itr->second) == -1)
      return 400;
    if (itr->first == "transfer-encoding" && itr->second != "chunked")
      return 501;
  }
  return 200;
}

void HTTPMessageParser::setCgiScripts(std::vector<std::string> cgiScripts)
{
  cgiScripts_ = cgiScripts;
  for (std::vector<std::string>::iterator itr = cgiScripts_.begin();
    itr != cgiScripts_.end();
    itr++
  )
  {
    std::string filenameExtension = *itr;
    filenameExtension += '/';
    filenameExtensions_.push_back(filenameExtension);
  }
}

HTTPMessageParser::HTTPMessageParser() :
  method_(httpMessageParser::OTHER)
{
  filenameExtensions_.push_back(".htm/");
  filenameExtensions_.push_back(".html/");
  filenameExtensions_.push_back(".php/");
  filenameExtensions_.push_back(".cgi/");
  filenameExtensions_.push_back(".bla/");
}

HTTPMessageParser::~HTTPMessageParser()
{
}

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