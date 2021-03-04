/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 08:16:13 by user42            #+#    #+#             */
/*   Updated: 2021/02/23 10:04:54 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*p;
	unsigned char	*s;

	if (!src && !dest)
		return (0);
	p = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (n-- > 0)
	{
		*p++ = *s++;
	}
	return (dest);
}
