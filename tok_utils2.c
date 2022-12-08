/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:25:48 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 01:27:41 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_charachter(char c)
{
	if (c == '<' || c == '|' || c == '>' || c == ';'
		|| c == '$' || c == '\'' || c == '"')
		return (1);
	return (0);
}

int	is_blank(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	is_redir(t_global *global, int	*i, char c)
{
	int	start;

	start = *i;
	if (global->line[*i] == c)
	{
		(*i)++;
		global->is_redir = 1;
		return (1);
	}
	*i = start;
	return (0);
}

int	is_heredoc_or_append(t_global *global, int	*i, char c)
{
	int	start;

	start = *i;
	if (global->line[*i] == c && global->line[*i + 1] == c)
	{
		if (c == '<')
			global->heredoc_activ = 1;
		if (c == '>')
			global->is_redir = 1;
		*i += 2;
		return (1);
	}
	*i = start;
	return (0);
}

int	is_heredoc_key(t_global *global, int	*i)
{
	int	start;

	start = *i;
	while (global->line[*i] && global->heredoc_activ == 1
		&& (!is_charachter(global->line[*i]) || global->line[*i] == '$'))
	{
			(*i)++;
		if (is_blank(global->line[*i]) || !global->line[*i]
			|| is_charachter(global->line[*i]))
		{
			global->heredoc_activ = 0;
			return (1);
		}
	}
	*i = start;
	return (0);
}
