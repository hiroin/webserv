#ifndef _HTTP1_PARSER_H_
#define _HTTP1_PARSER_H_

#include <string>
#include <cstring>
#include <unistd.h>
#include "configure.hpp"

class HTTP1_Parser {
public:
  static char *get_filename(const char *excutive_file);
  static std::string argv_path_analyzer(std::string request_path, const char *path, const char *executive_file);
  static std::string get_requestline_path(const char _pbuf[BUF_SIZE]);
};

#endif