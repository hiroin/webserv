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

typedef enum _token_type {
	INITIAL,
	CHAR,      /* 文字 */
	SEMICOLON, /* ; */
	LB,        /* { */
	RB,        /* } */
  UNAUTHORIZED,
	END
} TOKEN_TYPE;

class TOKEN {
 public:
	TOKEN_TYPE    type;
	std::string   token;
} ;

#endif