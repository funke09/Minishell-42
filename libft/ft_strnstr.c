/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 17:21:17 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/22 03:23:53 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	size;

	i = 0;
	size = ft_strlen(needle);
	if (!haystack || len == 0 || !needle)
		return (NULL);
	if (!size)
		return ((char *)haystack);
	while (haystack[i] && i < len)
	{
		if (i + size > len)
			return (NULL);
		if (!(ft_strncmp(haystack + i, needle, size)))
			return ((char *)haystack + i);
		i++;
	}
	return (NULL);
}
