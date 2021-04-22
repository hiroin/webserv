/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stoi.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 08:16:13 by user42            #+#    #+#             */
/*   Updated: 2021/04/22 07:53:57 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"

int ft_stoi(std::string s)
{
  size_t i;
  long r;

  i = 0;
  if (!std::isdigit(s[i]))
    return -1;
  if (s[i] == '0')
    return -1;
  i++;
  while (std::isdigit(s[i]))
    i++;
  if (i != s.size())
    return -1;
  i = 0;
  r = s[i++] - '0';
  while (i < s.size())
    r = r * 10 + s[i++] - '0';
  if (r > INT_MAX || r <= 0)
    return -1;
	return (r);
}
