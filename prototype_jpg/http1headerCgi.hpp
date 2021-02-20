#ifndef _HTTP1_HEAD_CGI_H_
#define _HTTP1_HEAD_CGI_H_

#include <vector>
#include <string>
#include <map>
#include <sstream>

class HTTP1_Header_Cgi {
 public:
  static std::vector<std::string>& make_response200();
};

#endif