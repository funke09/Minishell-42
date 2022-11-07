/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 19:49:38 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/22 02:42:02 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_size(long long n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len += 1;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	ft_putin(char *p, long long n, int size)
{
	while (n > 0)
	{
		p[size - 1] = (n % 10) + 48;
		n /= 10;
		size--;
	}
}

char	*ft_itoa(int n)
{
	int		size;
	char	*p;

	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	size = count_size(n);
	p = (char *)malloc(sizeof(char) * size + 1);
	if (!p)
		return (NULL);
	if (n < 0)
	{
		p[0] = '-';
		n = -n;
	}
	ft_putin(p, n, size);
	p[size] = '\0';
	return (p);
}
