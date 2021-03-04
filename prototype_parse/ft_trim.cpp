/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 08:16:13 by user42            #+#    #+#             */
/*   Updated: 2021/02/27 13:33:10 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"

std::string ft_trim(const std::string& string, const char* trimCharacterList = " \t")
{
  std::string result;
  std::string::size_type left = string.find_first_not_of(trimCharacterList);
  if (left != std::string::npos)
  {
    std::string::size_type right = string.find_last_not_of(trimCharacterList);
    result = string.substr(left, right - left + 1);
  }
  return result;
}
