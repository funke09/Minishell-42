/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 11:11:48 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/22 03:15:14 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int	i;
	int	j;

	if (s && f)
	{
		i = ft_strlen(s);
		j = 0;
		while (j < i)
		{
			(*f)(j, s);
			j++;
			s++;
		}
	}
}
