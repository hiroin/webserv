#ifndef _FT_H_
#define _FT_H_

#include <stdlib.h>
#include <string>
#include <climits>

bool ft_isctl(char c);
bool ft_istspecials(char c);
bool ft_istoken(std::string s);
int ft_stoi(std::string s);

#endif