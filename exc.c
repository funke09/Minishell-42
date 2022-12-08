/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:25:02 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 01:25:03 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	oldpwd_var(t_env **env_list)
{
	char	**tmp;

	tmp = malloc(3 * sizeof(char *));
	tmp[2] = NULL;
	tmp[1] = ft_strjoin("OLDPWD=", getcwd(NULL, 0));
	tmp[0] = ft_strdup("export");
	ft_export(env_list, tmp);
	ft_free(tmp, 1);
}

void	pwd_var(t_env **env_list)
{
	char	**tmp;

	tmp = malloc(3 * sizeof(char *));
	tmp[2] = NULL;
	tmp[1] = ft_strjoin("PWD=", getcwd(NULL, 0));
	tmp[0] = ft_strdup("export");
	ft_export(env_list, tmp);
	ft_free(tmp, 1);
}

void	go_to_cd(char *path, t_env **env_list)
{
	struct stat	st;

	(void)env_list;
	if (path && access(path, F_OK) == 0)
	{
		stat(path, &st);
		if (!S_ISDIR(st.st_mode))
		{
			return (ft_putendl_fd("minishell : cd: not a directory", 2));
		}
		if (access(path, X_OK) == 0)
		{
			oldpwd_var(env_list);
			chdir(path);
			pwd_var(env_list);
		}
	}
	else
		ft_putendl_fd("minishell: cd: No such file or directory", 2);
}

int	c_cd(char **args, t_env **env)
{
	char	*path;
	int		len;

	len = ft_sizearray(args);
	if (len > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments.\n", 2);
		return (1);
	}
	if (len == 1)
	{
		path = get_path(env, "HOME");
		if (!path)
			return (1);
		go_to_cd(path, env);
	}
	if (len == 2)
	{
		go_to_cd(args[1], env);
	}
	return (0);
}

int	do_builtin(char **args, t_env **env)
{
	int	len;

	len = ft_strlen(args[0]);
	if (!args[0])
		return (1);
	if (len == 3 && !ft_strncmp(args[0], "pwd", len))
		return (pwd());
	else if (len == 6 && !ft_strncmp(args[0], "export", len))
		return (ft_export(env, args));
	else if (len == 3 && !ft_strncmp(args[0], "env", len))
		return (ft_env(args));
	else if (len == 4 && !ft_strncmp(args[0], "exit", len))
		return (ft_exit(args, env));
	else if (len == 4 && !ft_strncmp(args[0], "echo", len))
		return (echo(args));
	else if (len == 5 && !ft_strncmp(args[0], "unset", len))
		return (ft_unset(env, args));
	else if (len == 2 && !ft_strncmp(args[0], "cd", len))
		return (c_cd(args, env));
	return (1);
}
