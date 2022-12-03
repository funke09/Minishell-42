/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printferror.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:14 by zcherrad          #+#    #+#             */
/*   Updated: 2022/12/03 22:47:12 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen_char(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != '$' && s[i] != ' ' && s[i] != '\"'
		&& s[i] != '\'' && s[i] != '/')
		i++;
	return (i);
}

char	*generate_dolar(char *str)
{
	t_for_expantion	var;

	ft_bzero(&var, sizeof(t_for_expantion));
	if (str[0] == '\"')
		var.i++;
	while (str[var.i])
	{
		if (str[var.i] == '\"')
			break ;
		if (str[var.i] == '$' && str[var.i + 1] != ' ' && str[var.i + 1])
		{
			var.len = ft_strlen_char(str + var.i + 1);
			var.val = expantion((str + var.i));
			if (!var.val)
					var.val = "";
			ft_strncpy(var.res + var.j, var.val, ft_strlen(var.val));
			var.i += var.len;
			var.j += ft_strlen(var.val);
		}
		else
			var.res[var.j++] = str[var.i];
		var.i++;
	}
	var.res[var.j] = '\0';
	return (ft_strdup(var.res));
}

char	*go_to_herdoc(t_global *global, t_tokens *tokens)
{
	char	*str;
	char	*tmp;

	tokens->here_doc_txt = ft_strdup("");
	(void)global;
	while (1)
	{
		str = readline("heredoc> ");
		if (!str || !ft_strcmp(str, tokens->token))
			break ;
		tmp = str;
		str = generate_dolar(str);
		free(tmp);
		tmp = str;
		str = ft_strjoin(tokens->here_doc_txt, str);
		free(tmp);
		tmp = tokens->here_doc_txt;
		tokens->here_doc_txt = ft_strjoin(str, "\n");
		free(tmp);
		free(str);
	}
	if (!str)
		ft_strdel(&(tokens->here_doc_txt));
	return (tokens->here_doc_txt);
}

void	free_tokens(t_tokens *tokens)
{
	t_tokens	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->token);
		free(tmp);
	}
}
char	*inside_quote(char *tokens)
{
	int		i;
	int		j;
	char	*token;
	char	*str;

	i = 0;
	j = 0;
	token = tokens;
	str = (char *)malloc(sizeof(char) * ft_strlen(tokens));
	if (token[0] == '\'')
		i++;
	while (token[i])
	{
		if (token[i] == '\'')
			break ;
		str[j++] = token[i++];
	}
	str[j] = '\0';
	return (str);
}

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
		if (!tmp->next || tmp->next->type == HEREDOC
			|| tmp->next->type == REDIR_IN || tmp->next->type == REDIR_OUT
			|| tmp->next->type == APPEND || tmp->next->type == PIPE)
			global->errnum = ERROR_HEREDOC;
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
