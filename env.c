/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:24:59 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 01:25:00 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clean_envlist(t_env **env)
{
	t_env	*cpy;
	t_env	*tmp;

	cpy = *env;
	tmp = cpy;
	while (cpy)
	{
		tmp = cpy;
		cpy = cpy->next;
		free(tmp->str);
		free(tmp);
	}
}

int	len_key(char *env)
{
	int	i;

	i = 0;
	while (env[i] != '\0' && env[i] != '=')
		i++;
	return (i);
}

int	push(char *env, t_env **begin_lst)
{
	t_env	*env_new;

	env_new = malloc(sizeof(t_env));
	if (env_new == NULL)
		return (42);
	env_new->str = ft_strdup(env);
	if (env_new->str == NULL)
		return (42);
	env_new->next = *begin_lst;
	*begin_lst = env_new;
	return (0);
}

int	init_env(t_env **env_list)
{
	char	*s;

	s = ft_strjoin("PWD=", getcwd(NULL, 0));
	if (s == NULL)
		return (42);
	if (push("SHLVL=1", env_list) == 42)
	{
		ft_clean_envlist(env_list);
		return (42);
	}
	if (push(s, env_list) == 42)
	{
		free(s);
		ft_clean_envlist(env_list);
		return (42);
	}
	free(s);
	if (push("OLDPWD", env_list) == 42)
	{
		ft_clean_envlist(env_list);
		return (42);
	}
	return (0);
}

int	stock_env(char **env, t_global *global)
{
	int		i;

	global->env = get_adress();
	i = 0;
	while (env[i])
		i++;
	if (i == 0)
	{	
		if (init_env(global->env) == 42)
			return (42);
	}
	i--;
	while (i >= 0)
	{
		if (push(env[i], global->env) == 42)
		{
			ft_clean_envlist(global->env);
			return (42);
		}
		i--;
	}
	return (0);
}
