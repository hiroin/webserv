#ifndef _HTTPMESSAGEPARSER_H_
#define _HTTPMESSAGEPARSER_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>

namespace httpMessageParser {
  enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER};
}

class HTTPMessageParser {
 public:
  enum httpMessageParser::method getMethod() const;
  std::string getRequestTarget() const;
  std::string getHTTPVersion() const;
  std::string getAbsolutePath() const;
  std::string getQuery() const;
  std::string getAuthority() const;
  std::string getPathinfo() const;
  std::map<std::string, std::string> getHeaders() const;
  HTTPMessageParser();
  HTTPMessageParser(int testPattern);
  HTTPMessageParser(std::string conf, int testPattern);
  ~HTTPMessageParser();

 private:

 private:
  std::string requestLine_;
  std::vector<std::string> filenameExtensions_;

  // requestLine_をparseした結果を格納
  enum httpMessageParser::method method_;
  std::string requestTarget_;
  std::string HTTPVersion_;

  // requestTarget_をparseした結果を格納
  std::string absolutePath_; // OPTIONSの*の場合のここにいれる
  std::string query_;
  std::string authority_; // CONNECTの場合の接続先
  std::string pathinfo_;

  // headerを格納
  std::map<std::string, std::string> headers_;

};

/*

https://triple-underscore.github.io/RFC7230-ja.html
*/
#endif