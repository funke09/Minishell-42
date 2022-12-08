/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:25:07 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 01:25:08 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_of_cmd_sin_pipes(t_tokens *token)
{
	int	i;

	i = 0;
	while (token && token->type != PIPE)
	{
		token = token->next;
		i++;
	}
	return (i);
}

t_tokens	*join_expantion(t_tokens *token, char **cmd, int *i)
{
	char	*str;
	char	*res;
	char	*tmp;

	str = token->token;
	res = NULL;
	tmp = NULL;
	while (token && token->no_space == 1 && token->next)
	{
		tmp = res;
		res = ft_strjoin(str, token->next->token);
		if (!res)
			return (NULL);
		if (tmp)
			free(tmp);
		str = res;
		token = token->next;
	}
	cmd[*i] = res;
	*i += 1;
	return (token->next);
}

int	is_valid_type(t_tokens *token)
{
	if (token->type == COMMAND || token->type == FLAG
		|| token->type == PARAM
		|| token->type == D_QUOTE || token->type == S_QUOTE
		|| token->type == ENV_VAR)
		return (1);
	return (0);
}

void	while_cmd(t_tokens *token, char **cmd, int *i)
{
	while (token && token->type != PIPE)
	{
		if (token->type == REDIR_IN || token->type == REDIR_OUT
			|| token->type == APPEND)
				token = token->next;
		else if (token && is_valid_type(token))
		{
			if (token->no_space && token->next)
			{
				token = join_expantion(token, cmd, i);
				continue ;
			}
			cmd[*i] = ft_strdup(token->token);
			*i += 1;
		}
		if (!token)
			break ;
		token = token->next;
	}
}

char	**get_cmd(t_tokens *token)
{
	int		i;
	int		len;
	char	**cmd;

	len = len_of_cmd_sin_pipes(token);
	cmd = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while_cmd(token, cmd, &i);
	cmd[i] = 0;
	return (cmd);
}
