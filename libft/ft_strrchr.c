/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:23:26 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/22 03:25:03 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*last;

	last = NULL;
	while (*s != '\0')
	{
		if ((unsigned char)*s == (unsigned char)c)
			last = (char *)s;
		s++;
	}
	if (c == 0)
		return ((char *)s);
	return (last);
}
