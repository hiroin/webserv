/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_istoken.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 06:20:50 by user42            #+#    #+#             */
/*   Updated: 2021/04/15 15:51:11 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"

bool ft_istoken(std::string s)
{
  if (s.size() == 0)
    return false;
  for (std::string::iterator itr = s.begin();
    itr != s.end();
    itr++
  )
  {
    if (ft_isctl(*itr) || ft_istspecials(*itr))
      return false;
  }
  return true;
}
