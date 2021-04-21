#ifndef _HTTPMESSAGEPARSER_H_
#define _HTTPMESSAGEPARSER_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>
#include "ft.hpp"

namespace httpMessageParser {
  enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER};
}

class HTTPMessageParser {
 public:
  bool parseRequestLine(const std::string requestLine);
  std::string getMessageHeader() const;
  enum httpMessageParser::method getMethod() const;
  std::string getRequestTarget() const;
  std::string getHTTPVersion() const;
  bool parseRequestTarget(std::string requestTarget);
  std::string getAbsolutePath() const;
  std::string getQuery() const;
  std::string getAuthority() const;
  std::string getPathinfo() const;
  bool parseHeader(std::string header);
  std::map<std::string, std::string> getHeaders() const;
  void clearData();
  bool correctHostValue(std::map<std::string, std::string> headers) const; // private
  bool correctHostValue(std::string hostValue) const; // private
  bool correctUrihost(std::string uriHost) const; // private

 public:
  HTTPMessageParser();
  ~HTTPMessageParser();

 private:
  std::string requestLine_;
  std::vector<std::string> filenameExtensions_;

  // requestLine_をparseした結果を格納
  enum httpMessageParser::method method_;
  std::string requestTarget_;
  std::string HTTPVersion_;

  // requestTarget_をparseした結果を格納
  bool isAuthority(std::string requestTarget);
  std::string getHostAndPort(std::string requestTarget);
  std::string absolutePath_; // OPTIONSの*の場合のここにいれる
  std::string query_;
  std::string authority_; // CONNECTの場合の接続先
  std::string pathinfo_;

  // headerを格納
  bool validationHeader(std::string header);
  bool pushFieldNameAndValue(std::string fieldName, std::string fieldValue);
  std::map<std::string, std::string> headers_;

  //
 public:
  friend class HTTPMessageParserTest;
};

/*

https://triple-underscore.github.io/RFC7230-ja.html
*/
#endif