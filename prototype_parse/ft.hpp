#ifndef _FT_H_
#define _FT_H_

#include <stdlib.h>
#include <string>

#define SUCCESS 1
#define FAILURE 0

bool ft_isspase_and_htab(char c);
bool ft_isvchar(char c);
bool ft_istchar(char c);
void *ft_memset(void *s, int c, size_t n);
void *ft_memcpy(void *dest, const void *src, size_t n);
std::string ReplaceString
(
    std::string String1   // 置き換え対象
  , std::string String2   // 検索対象
  , std::string String3   // 置き換える内容
);
std::string ft_trim
(
    const std::string& string
  , const char* trimCharacterList
);

#endif