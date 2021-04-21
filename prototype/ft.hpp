#ifndef _FT_H_
#define _FT_H_

#include <stdlib.h>
#include <string>
#include <vector>

#define SUCCESS 1
#define FAILURE 0

bool ft_isctl(char c);
bool ft_isspase_and_htab(char c);
bool ft_issubdelims(char c);
bool ft_istchar(char c);
bool ft_istoken(std::string s);
bool ft_istspecials(char c);
bool ft_isunreserved(char c);
bool ft_isvchar(char c);
bool ft_istchar(char c);
void *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_memset(void *s, int c, size_t n);
void ft_print_vector_string(std::vector<std::string> vs);
std::string ReplaceString
(
    std::string String1   // 置き換え対象
  , std::string String2   // 検索対象
  , std::string String3   // 置き換える内容
);
std::vector<std::string> ft_split_use_find_first_of(std::string s, std::string d);
std::string ft_trim
(
    const std::string& string
  , const char* trimCharacterList
);

#endif