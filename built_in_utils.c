/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:24:53 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 01:24:54 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_a_builtin_child(char *cmd)
{
	int	len;

	len = ft_strlen(cmd);
	if (!cmd)
		return (1);
	if (len == 3 && !ft_strncmp(cmd, "pwd", len))
		return (0);
	else if (len == 6 && !ft_strncmp(cmd, "export", len))
		return (0);
	else if (len == 3 && !ft_strncmp(cmd, "env", len))
		return (0);
	else if (len == 4 && !ft_strncmp(cmd, "echo", len))
		return (0);
	return (1);
}

int	check_arg(int *i, char *str)
{
	int	j;

	j = 1;
	while (str[j])
	{
		if (str[j] != 'n')
			return (0);
		j++;
	}
	*i += 1;
	return (1);
}

int	echo(char **args)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	if (args[1] && args[1][0] == '-')
		n = check_arg(&i, args[1]);
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1] == 'n' && !args[i][2])
			i++;
		else
		{
			ft_putstr_fd(args[i++], 1);
			if (args[i])
				ft_putchar_fd(' ', 1);
		}
	}
	if (!n)
		ft_putchar_fd('\n', 1);
	return (0);
}

t_env	**get_adress(void)
{
	static t_env	*new = NULL;

	return (&new);
}

int	ft_sizearray(char **args)
{
	int	i;

	i = 0 ;
	while (args[i])
		i++;
	return (i);
}
