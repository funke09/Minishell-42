/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 19:01:18 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/22 03:13:02 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		len;
	char	*ns1;
	int		i;

	len = ft_strlen(s1);
	ns1 = (char *)ft_calloc(len + 1, sizeof(char));
	if (ns1 == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ns1[i] = s1[i];
		i++;
	}
	return (ns1);
}
