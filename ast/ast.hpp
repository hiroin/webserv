#ifndef _AST_H_
#define _AST_H_

#include <stdlib.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

enum TOKEN_TYPE {
	INITIAL,
	CHAR,      /* 文字 */
	SEMICOLON, /* ; */
	LB,        /* { */
	RB,        /* } */
  UNAUTHORIZED,
	END
};

class TOKEN {
 public:
	TOKEN_TYPE    type;
	std::string   token;
} ;

namespace config {
  enum method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, OTHER, NUM};
}

struct values
{
  std::vector<std::string> value;
};

struct context
{
  std::string key;
  std::vector<values> values;
};

class configBase
{
 public:
  std::vector<context> contexts;
};

class configLocation : public configBase
{
 public:
  std::string path;
};

class configServer : public configBase
{
 public:
  std::vector<configLocation> locations;
};

class configHttp : public configBase
{
 public:
  std::vector<configServer> servers;
};

#endif