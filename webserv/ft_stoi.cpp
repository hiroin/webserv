/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stoi.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 08:16:13 by user42            #+#    #+#             */
/*   Updated: 2021/04/22 08:54:26 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"

// 0～2147483647をintに変換する
// 012のように0から始まる数は0を無視する
// 負数、12aのわうに文字を含む場合は-1を返す
// https://triple-underscore.github.io/RFC7230-ja.html#header.content-length
// Content-Length = 1*DIGIT
// 【 構文としては，先頭の 0 も許容されているが、例えば "011" を数として解釈するときは， 11 と見なすと見受けられる（先頭の 0 を 8 進数に解釈するなどとは述べられていない）。 】
int ft_stoi(std::string s)
{
  size_t i;
  long r;

  for (std::string::iterator itr = s.begin();
    itr != s.end();
    itr++)
  {
    if (!std::isdigit(*itr))
      return -1;
  }
  i = 0;
  r = s[i++] - '0';
  while (i < s.size())
  {
    r = r * 10 + s[i++] - '0';
    if (r > INT_MAX)
      return -1;
  }
	return (r);
}
