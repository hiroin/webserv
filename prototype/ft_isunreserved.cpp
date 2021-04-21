/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isunreserved.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 06:20:50 by user42            #+#    #+#             */
/*   Updated: 2021/04/21 07:31:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"

bool ft_isunreserved(char c)
{
  if (std::isdigit(c))
    return true;
  if (std::isalpha(c))
    return true;
  switch (c)
  {
  case '-':
  case '.':
  case '_':
  case '~':
    return true;
  }
  return false;
}
