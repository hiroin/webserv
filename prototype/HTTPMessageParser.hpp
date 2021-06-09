#ifndef _HTTPMESSAGEPARSER_H_
#define _HTTPMESSAGEPARSER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "ft.hpp"

namespace httpMessageParser {
  enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER};
  const int MAX_URI_SIZE = 100;
}

class HTTPMessageParser {
 public:
  int parseRequestLine(const std::string requestLine);
  std::string getMessageHeader() const;
  enum httpMessageParser::method getMethod() const;
  std::string getRequestLine() const;
  std::string getMethodString() const;
  std::string getRequestTarget() const;
  std::string getHTTPVersion() const;
  bool parseRequestTarget(std::string requestTarget);
  std::string getAbsolutePath() const;
  std::string getQuery() const;
  std::string getAuthority() const;
  std::string getPathinfo() const;
  int parseHeader(std::string header);
  std::map<std::string, std::string> getHeaders() const;
  void clearData();
  bool isIllegalValueOfHostHeader(std::string headerField ) const;
  bool correctHostValue(std::string hostValue) const; // private
  bool correctUrihost(std::string uriHost) const; // private

 public:
  HTTPMessageParser();
  ~HTTPMessageParser();

 public:
  std::string requestLine_;
  std::vector<std::string> filenameExtensions_;

  // requestLine_をparseした結果を格納
  enum httpMessageParser::method method_;
  std::string requestTarget_;
  std::string HTTPVersion_;

  // CGIの拡張子を格納
  std::vector<std::string> cgiScripts_;
  void setCgiScripts(std::vector<std::string> cgiScripts);

  // requestTarget_をparseした結果を格納
  bool isAuthority(std::string requestTarget);
  std::string getHostAndPort(std::string requestTarget);
  std::string absolutePath_; // OPTIONSの*の場合のここにいれる
  std::string query_;
  std::string authority_; // CONNECTの場合の接続先
  std::string pathinfo_;

  // headerを格納
  bool validationHeader(std::string header);
  int pushFieldNameAndValue(std::string fieldName, std::string fieldValue);
  std::map<std::string, std::string> headers_;

  // headerの値のチェック
  int isInvalidHeader();
  bool isSetHostHeader();
  int isInvalidHeaderValue();

  // bodyを格納
  std::string body_;
};

/*

https://triple-underscore.github.io/RFC7230-ja.html
*/
#endif