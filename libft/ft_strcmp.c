/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: funke <funke@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 01:51:02 by funke             #+#    #+#             */
/*   Updated: 2022/10/21 01:51:11 by funke            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(char *str, char *in)
{
	int	len_str;
	int	len_in;
	int	i;

	i = 0;
	len_str = 0;
	len_in = 0;
	len_str = ft_strlen(str);
	len_in = ft_strlen(in);
	if (len_str != len_in)
		return (0);
	while (i < len_in)
	{
		if (str[i] != in[i])
			return (0);
		i++;
	}
	return (1);
}
