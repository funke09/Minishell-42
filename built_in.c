/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:24:56 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 01:24:57 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cwd(void)
{
	char		tmp[4096];
	char		*buff;

	buff = malloc(sizeof(char) * 4096);
	if (!buff)
		return (NULL);
	if (!getcwd(tmp, sizeof(tmp)))
		ft_strdel(&buff);
	else
		ft_strcpy(buff, tmp);
	return (buff);
}

int	ft_exit(char **args, t_env **env)
{
	(void)env;
	(void)args;
	exit(0);
}

int	pwd(void)
{
	char	*ret;

	ret = getcwd(NULL, 0);
	ft_putendl_fd(ret, 1);
	return (0);
}

void	ft_free(char **splt, int i)
{
	while (i >= 0)
		free(splt[i--]);
	free(splt);
}

int	is_a_builtin(char **cmd)
{
	int	len;

	if (!cmd[0])
		return (1);
	len = ft_strlen(cmd[0]);
	if (len == 4 && !ft_strncmp(cmd[0], "exit", len))
	{
		ft_putstr_fd("exit\n", 1);
		exit(0);
	}
	else if (len == 2 && !ft_strncmp(cmd[0], "cd", len))
		return (0);
	else if (len == 6 && !ft_strncmp(cmd[0], "export", len) && cmd[1] != NULL)
		return (0);
	else if (len == 5 && !ft_strncmp(cmd[0], "unset", len))
		return (0);
	return (1);
}
