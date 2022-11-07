/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 17:26:47 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/22 03:29:09 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

static int	is_in_set(char c, const char *set)
{
	int	s;

	s = 0;
	if (!set)
		return (0);
	while (set[s])
	{
		if (set[s] == c)
			return (1);
		s++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*sub;

	if (s1 == NULL || set == NULL)
		return (NULL);
	end = ft_strlen(s1) - 1;
	start = 0;
	while (is_in_set(s1[start], set) == 1)
	{
		start++;
	}
	while (is_in_set(s1[end], set) == 1)
	{
		end--;
	}
	sub = ft_substr(s1, start, (end - start) + 1);
	return (sub);
}
