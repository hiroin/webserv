/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stoi.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 08:16:13 by user42            #+#    #+#             */
/*   Updated: 2021/04/22 08:18:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"

// 0～2147483647をintに変換する
// 負数、012のように0から始まる数、12aのわうに文字を含む場合は-1を返す
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
  if (s[0] == '0' && s.size() >= 2)
    return -1;
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
