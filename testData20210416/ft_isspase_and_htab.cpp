/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspase.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 06:20:50 by user42            #+#    #+#             */
/*   Updated: 2021/03/05 01:01:34 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

bool ft_isspase_and_htab(char c)
{
	return ((c == ' ' || c == '\t') ? true : false);
}