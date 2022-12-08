/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:25:12 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 01:25:13 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_env(void)
{
	t_env	*env_list;
	int		len;

	env_list = *get_adress();
	while (env_list)
	{
		len = len_key(env_list->str);
		if (env_list->str[len])
		{
			ft_putstr_fd(env_list->str, 1);
			ft_putchar_fd('\n', 1);
		}
		if (!env_list->next)
			break ;
		env_list = env_list->next;
	}
}

int	ft_export(t_env **env, char **args)
{
	int	i;
	int	len;

	i = 0;
	len = -1;
	if (!args[1])
	{
		ft_print_export(env);
		return (0);
	}
	while (args[++i])
	{
		if (!check_arg_valid(args[i], &len))
			ft_handle_arg(env, args[i], len);
		else
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd(" : not a valid identifier\n", 2);
			return (1);
		}
	}
	return (0);
}

int	ft_env(char **args)
{
	int	l;

	l = ft_sizearray(args);
	if (l > 1)
	{
		ft_putendl_fd("Too many arguments.\n", 2);
	}
	else
		ft_print_env();
	return (0);
}

int	ft_unset(t_env **env, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (check_unset_arg_valid(args[i]))
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd(" : not a valid identifier\n", 2);
			return (1);
		}
		ft_remove(env, args[i]);
		i++;
	}
	return (0);
}

t_tokens	*skip_to_pipe(t_tokens *token)
{
	while (token && token->type != PIPE)
	{
		token = token->next;
	}
	if (token == NULL || token->next == NULL)
		return (NULL);
	return (token->next);
}
