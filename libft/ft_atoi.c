/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 16:46:48 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/25 04:17:39 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

static int	ft_isspace(int x)
{
	if (x == '\t' || x == '\n' || x == '\v')
		return (1);
	if (x == '\f' || x == '\r' || x == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	s;
	int	z;

	i = 0;
	s = 1;
	z = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			s = -s;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		z = (z * 10) + (str[i] - '0');
		i++;
	}
	return (z * s);
}
