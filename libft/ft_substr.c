/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 14:06:10 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/25 04:19:07 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*p;
	char	*c;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s))
		len = ft_strlen(s) - start;
	p = (char *)malloc(sizeof(char) * (len + 1));
	if (!p)
		return (NULL);
	c = (char *)s;
	while (len > 0)
	{
		p[i] = c[start];
		i++;
		start++;
		len--;
	}
	p[i] = '\0';
	return (p);
}
