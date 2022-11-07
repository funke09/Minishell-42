/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 16:43:36 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/22 03:15:57 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	s;
	size_t	z;

	z = ft_strlen(src);
	if (!dest && size == 0)
		return (z);
	s = ft_strlen(dest);
	i = 0;
	if (size < s)
		return (z + size);
	size -= s;
	if (size != 0)
	{
		while (src[i] && size - 1 != 0)
		{
			dest[i + s] = src[i];
			i++;
			size--;
		}
		dest[s + i] = '\0';
	}
	return (s + z);
}
