#include <stdlib.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

class TOKEN {
 public:
	std::string   token;
};

struct test {
	std::string   token;
};

int main ()
{
  TOKEN *t;
  t =  new TOKEN;
  t->token = "string";

  // test *st;
  // st = new test;
  // st->token = "string";

  test st;
  st.token = "string";


}