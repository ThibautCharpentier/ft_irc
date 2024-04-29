/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 16:04:49 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/28 16:04:49 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

static unsigned int	ft_size_result(unsigned int nbr)
{
	unsigned int	size;

	if (nbr == 0)
		return (1);
	size = 0;
	while (nbr != 0)
	{
		nbr = nbr / 10;
		size++;
	}
	return (size);
}

static void	ft_put_result(char *res, unsigned int nbr)
{
	while (nbr != 0)
	{
		*res = nbr % 10 + 48;
		nbr = nbr / 10;
		res--;
	}
}

std::string	ft_itoa(int n)
{
	char			*res;
	unsigned int	nbr;
	unsigned int	size;
	std::string		result;

	if (n < 0)
	{
		nbr = -n;
		size = ft_size_result(nbr) + 1;
	}
	else
	{
		nbr = n;
		size = ft_size_result(nbr);
	}
	res = (char *) malloc(sizeof(char) * size + 1);
	if (!res)
		return (0);
	if (n < 0)
		res[0] = '-';
	res[size] = 0;
	if (n == 0)
		res[0] = 48;
	ft_put_result(res + size - 1, nbr);
	result = res;
	free(res);
	return (result);
}
