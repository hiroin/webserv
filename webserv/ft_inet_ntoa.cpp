/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_inet_ntoa.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 06:20:50 by user42            #+#    #+#             */
/*   Updated: 2021/06/09 09:15:38 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string>

static int	ft_digit_count(long i)
{
	int	count;

	count = 0;
	if (i == 0)
		return (1);
	if (i < 0)
	{
		i *= -1;
		count++;
	}
	while (i > 0)
	{
		i /= 10;
		count++;
	}
	return (count);
}

static char		*ft_itoa(int n)
{
	char		*str;
	int			i;
	long		nb;

	nb = n;
	i = ft_digit_count(nb);
	if (!(str = new char[i * sizeof(char) + 1]))
		return (NULL);
	str[i--] = 0;
	if (nb == 0)
	{
		str[0] = '0';
	}
	if (nb < 0)
	{
		str[0] = '-';
		nb = nb * -1;
	}
	while (nb > 0)
	{
		str[i--] = nb % 10 + '0';
		nb = nb / 10;
	}
	return (str);
}

std::string ft_inet_ntos (struct in_addr in)
{
  std::string ipaddress;
  char *tmp;
  unsigned char *bytes = (unsigned char *) &in;
  tmp = ft_itoa(bytes[0]);
  ipaddress.append(tmp);
  delete[] tmp;
  ipaddress.append(".");
  tmp = ft_itoa(bytes[1]);
  ipaddress.append(tmp);
  delete[] tmp;
  ipaddress.append(".");
  tmp = ft_itoa(bytes[2]);
  ipaddress.append(tmp);
  delete[] tmp;
  ipaddress.append(".");
  tmp = ft_itoa(bytes[3]);
  ipaddress.append(tmp);
  delete[] tmp;
  return ipaddress;
}
