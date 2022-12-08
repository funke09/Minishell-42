/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toknezation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 06:33:59 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 06:38:27 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_type	if_pipe(t_global *global, int *i)
{
	(*i)++;
	global->cmd_status = 0;
	return (PIPE);
}

t_type	type(t_global *global, int	*i, int *no_space)
{
	if (is_heredoc_key(global, i))
		return (HERDOC_KEY);
	else if (is_quote(global, i, '\'', no_space))
		return (S_QUOTE);
	else if (is_quote(global, i, '\"', no_space))
		return (D_QUOTE);
	else if (global->line[*i] == '|')
		return (if_pipe(global, i));
	else if (is_dolar(global, i, no_space))
		return (ENV_VAR);
	else if (is_heredoc_or_append(global, i, '>'))
		return (APPEND);
	else if (is_heredoc_or_append(global, i, '<'))
		return (HEREDOC);
	else if (is_redir(global, i, '<'))
		return (REDIR_IN);
	else if (is_flag(global, i, no_space))
		return (FLAG);
	else if (is_redir(global, i, '>'))
		return (REDIR_OUT);
	else if (is_command(global, i, no_space))
		return (COMMAND);
	else if (is_param(global, i, no_space))
		return (PARAM);
	return (NON);
}

void	skip_blanks(t_global *global, int	*i)
{
	while (is_blank(global->line[*i]))
		(*i)++;
}

t_tokens	*add_token(t_global *global, int *i)
{
	int			len;
	int			start;
	t_tokens	*new;
	int			no_space;

	no_space = 0;
	skip_blanks(global, i);
	start = *i;
	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	new->type = type(global, i, &no_space);
	new->no_space = no_space;
	if (new->type == NON)
	{
		free(new);
		global->cmd_status = 0;
		global->heredoc_activ = 0;
		global->is_redir = 0;
		return (NULL);
	}
	len = *i - start;
	new->token = ft_substr(global->line, start, len);
	new->next = NULL;
	return (new);
}

void	tokenization(t_global *global)
{
	int			i;
	t_tokens	*current;

	i = 0;
	global->tokens = add_token(global, &i);
	current = global->tokens;
	while (current)
	{
		current->next = add_token(global, &i);
		current = current->next;
	}
}
