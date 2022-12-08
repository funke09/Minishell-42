/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printferror.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:25:41 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 06:21:50 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	printferror(t_global *global)
{
	t_error	error;

	error = global->errnum;
	if (error == ERROR_QUOTE)
		printf("ERROR_QUOTE\n");
	if (error == ERROR_PIPE)
		printf("ERROR_PIPE\n");
	if (error == ERROR_REDIR)
		printf("ERROR_REDIR\n");
	if (error == ERROR_HEREDOC)
		printf("ERROR_HEREDOC\n");
	if (error == ERROR_APPEND)
		printf("ERROR_APPEND\n");
	if (error == ERROR_PARAM)
		printf("ERROR_PARAM\n");
	if (error == ERROR_COMMAND)
		printf("ERROR_COMMAND\n");
	if (error == ENV_NOT_FOUND)
		printf("ENV_NOT_FOUND\n");
	if (global->tokens)
	{
		free_tokens(global->tokens);
		global->tokens = NULL;
	}
	global->errnum = 0;
}

void	in_out_check(t_global *global, t_tokens *tmp, int start)
{
	if (tmp->type == PIPE)
	{
		if (start == 1 || !tmp->next || tmp->next->type == PIPE)
			global->errnum = ERROR_PIPE;
	}
	else if (tmp->type == REDIR_IN)
	{
		if (!tmp->next || tmp->next->type == REDIR_IN
			|| tmp->next->type == REDIR_OUT || tmp->next->type == APPEND
			|| tmp->next->type == HEREDOC || tmp->next->type == PIPE)
			global->errnum = ERROR_REDIR;
	}
	else if (tmp->type == REDIR_OUT)
	{
		if (!tmp->next || tmp->next->type == REDIR_OUT
			|| tmp->next->type == HEREDOC || tmp->next->type == APPEND
			|| tmp->next->type == REDIR_IN || tmp->next->type == PIPE)
			global->errnum = ERROR_REDIR;
	}
}

void	redirections_check(t_global *global, t_tokens *tmp)
{
	char	*temp;

	if (tmp->type == APPEND)
	{
		if (!tmp->next || tmp->next->type == APPEND
			|| tmp->next->type == REDIR_OUT
			|| tmp->next->type == REDIR_IN || tmp->next->type == PIPE
			|| tmp->next->type == HEREDOC)
			global->errnum = ERROR_APPEND;
	}
	else if (tmp->type == HEREDOC)
	{
		is_here(tmp, global);
	}
	else if (tmp->type == ENV_VAR)
	{
		temp = tmp->token;
		tmp->token = generate_dolar(tmp->token);
		if (!tmp->token)
			global->errnum = ENV_NOT_FOUND;
		free(temp);
	}
}

void	quotes_check(t_tokens *tmp)
{
	char	*temp;

	if (tmp->type == D_QUOTE)
	{
		temp = tmp->token;
		tmp->token = generate_dolar(tmp->token);
		free(temp);
	}
	else if (tmp->type == S_QUOTE)
	{
		temp = tmp->token;
		tmp->token = inside_quote(tmp->token);
		free(temp);
	}
}

void	check_tokens(t_global *global)
{
	t_tokens	*tmp;
	int			start;

	tmp = global->tokens;
	start = 1;
	while (tmp)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
			|| tmp->type == PIPE)
			in_out_check(global, tmp, start);
		else if (tmp->type == HEREDOC || tmp->type == APPEND
			|| tmp->type == ENV_VAR)
			redirections_check(global, tmp);
		else if (tmp->type == D_QUOTE || tmp->type == S_QUOTE)
			quotes_check(tmp);
		tmp = tmp->next;
		start = 0;
	}
}
