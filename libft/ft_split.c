/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 17:24:40 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/25 04:22:15 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

typedef struct s_vars
{
	char	**rsv;
	int		i;
	int		j;
	int		k;
	int		start;
	int		count;
}				t_vars;

static int	counter(char const *s, char c)
{
	int	count;

	count = 0;
	if (c == 0 && *s == 0)
		return (0);
	while (*s == c)
	{
		s++;
	}
	while (*s)
	{
		if (*s == c)
			count++;
		while (*s != '\0' && *s == c)
			s++;
		if (*s != '\0')
			s++;
	}
	if (*(s - 1) != c)
		count++;
	return (count);
}

static int	cntchar(const char *s, int *start, char c)
{
	int	i;
	int	j;

	i = *start;
	j = 0;
	while (s[i] == c)
		i++;
	while (s[i] && s[i] != c)
	{
		j++;
		i++;
	}
	*start = i;
	return (j);
}

static char	**loop(t_vars vars, const char *s, char c)
{
	while (vars.i < vars.count)
	{
		vars.k = 0;
		vars.rsv[vars.i] = \
			(char *)malloc(sizeof(char) * (cntchar(s, &vars.start, c) + 1));
		if (!vars.rsv[vars.i])
		{
			while (--(vars.i) >= 0)
				free(vars.rsv[vars.i]);
			free(vars.rsv);
			return (NULL);
		}
		while (s[vars.j] == c)
			vars.j++;
		while (s[vars.j] != '\0' && s[vars.j] != c)
		{
			vars.rsv[vars.i][vars.k] = s[vars.j];
			vars.j++;
			vars.k++;
		}
		vars.rsv[vars.i][vars.k] = '\0';
		vars.i++;
	}
	vars.rsv[vars.i] = NULL;
	return (vars.rsv);
}

char	**ft_split(char const *s, char c)
{
	t_vars	vars;

	vars.i = 0;
	vars.j = 0;
	vars.k = 0;
	vars.start = 0;
	if (s == NULL)
		return (NULL);
	vars.count = counter(s, c);
	vars.rsv = (char **)malloc(sizeof (char *) * (vars.count + 1));
	if (!vars.rsv)
		return (NULL);
	if (!*s)
	{
		vars.rsv[0] = NULL;
		return (vars.rsv);
	}
	return (loop(vars, s, c));
}
