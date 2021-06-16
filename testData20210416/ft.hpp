#ifndef _FT_H_
#define _FT_H_

#include <stdlib.h>
#include <string>
#include <climits>

#define SUCCESS 1
#define FAILURE 0

bool ft_isctl(char c);
bool ft_istspecials(char c);
bool ft_istoken(std::string s);
int ft_stoi(std::string s);
bool ft_istchar(char c);
bool ft_isspase_and_htab(char c);
bool ft_isvchar(char c);

#endif