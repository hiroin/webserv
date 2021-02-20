#ifndef _HTTP1_HEAD_JPG_H_
#define _HTTP1_HEAD_JPG_H_

#include <vector>
#include <string>
#include <map>
#include <sstream>

class HTTP1_Header_Jpg {
 public:
  static std::vector<std::string>& make_response404();
  static std::vector<std::string>& make_response200(int body_length);
};

#endif