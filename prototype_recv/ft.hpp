#ifndef _FT_H_
#define _FT_H_

#include <stdlib.h>
#include <string>

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