/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:33 by zcherrad          #+#    #+#             */
/*   Updated: 2022/12/02 13:00:43 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int var)
{
	if (var == SIGINT && !g_glb._status)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		rl_replace_line("", 0);
	}
}

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
		&& !is_charachter(global->line[*i]))
	{
			(*i)++;
		if (is_blank(global->line[*i]) || !global->line[*i])
		{
			global->heredoc_activ = 0;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_flag(t_global *global, int *i, int *no_space)
{
	int	start;

	start = *i;
	if (global->line[*i] == '-' && !is_blank(global->line[*i + 1])
		&& ft_isalpha(global->line[*i + 1]))
	{
		(*i)++;
		while (ft_isalpha(global->line[*i]))
			(*i)++;
		if (!is_blank(global->line[*i]) && global->line[*i] != '|' && global->line[*i] != '<' && global->line[*i] != '>')
				*no_space = 1;
		return (1);
	}
	*i = start;
	return (0);
}

int	is_quote(t_global *global, int	*i, char c, int *no_space)
{
	int	start;

	start = *i;
	if (global->line[*i] == c)
	{
		(*i)++;
		while (global->line[*i] && global->line[*i] != c)
			(*i)++;
		if (global->line[*i] == '\0')
			global->errnum = ERROR_QUOTE;
		if (global->line[*i] == c)
		{
			(*i)++;
			if (global->line[*i] && !is_blank(global->line[*i])
				&& global->line[*i] != '|' && global->line[*i] != '<'
				&& global->line[*i] != '>')
				*no_space = 1;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_command(t_global *global, int *i, int *no_space)
{
	int	start;

	start = *i;
	while (global->line[*i] && global->is_redir == 0 && global->cmd_status == 0
		&& !is_blank(global->line[*i]))
	{
		(*i)++;
		if (is_blank(global->line[*i]) || global->line[*i] == '\0'
			|| is_charachter(global->line[*i]))
		{
			if (!is_blank(global->line[*i]) && global->line[*i] != '|'
				&& global->line[*i] != '<' && global->line[*i] != '>')
				*no_space = 1;
			global->cmd_status = 1;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_param(t_global *global, int	*i, int *no_space)
{
	int	start;

	start = *i;
	while ((global->cmd_status == 1 || global->is_redir == 1)
		&& !is_blank(global->line[*i]) && global->line[*i])
	{
		(*i)++;
		if (is_blank(global->line[*i]) || !global->line[*i]
			|| is_charachter(global->line[*i]))
		{
			if (!is_blank(global->line[*i]) && global->line[*i] != '|'
				&& global->line[*i] != '<' && global->line[*i] != '>')
				*no_space = 1;
			global->is_redir = 0;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_dolar(t_global *global, int	*i, int *no_space)
{
	int	start;

	start = *i;
	if (global->line[*i] && global->line[*i] == '$' && (ft_isalnum(global->line[*i + 1]) || global->line[*i + 1] == '?'))
	{
		(*i)++;
		while (global->line[*i] && !is_blank(global->line[*i])
			&& (ft_isalnum(global->line[*i])
				|| global->line[*i] == '?') && !is_charachter(global->line[*i]))
			(*i)++;
		if (!is_blank(global->line[*i]) && global->line[*i] != '|'
			&& global->line[*i] != '<' && global->line[*i] != '>')
			*no_space = 1;
		return (1);
	}
	*i = start;
	return (0);
}

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

void skip_blanks(t_global *global, int	*i)
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

void tokenization(t_global *global)
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

void	init_global(t_global *global)
{
	global->line = NULL;
	global->cmd_status = 0;
	global->fd[0] = -1;
	global->fd[1] = -1;
	global->tokens = NULL;
	global->heredoc_activ = 0;
	global->errnum = 0;
	global->is_redir = 0;
}

void	ft_print_env(void)
{
	t_env	*env_list;

	env_list = *get_adress();
	while (env_list)
	{
		ft_putstr_fd(env_list->str, 1);
		ft_putchar_fd('\n', 1);
		if (!env_list->next)
			break ;
		env_list = env_list->next;
	}
}

void print_tokens(t_global *global)
{
    t_tokens *token;

    token = global->tokens;
    if (!token)
        return;
    while (token)
    {
        printf("token: %s, type: %d, no_space=%d\n ", token->token, token->type, token->no_space);
        token = token->next;
    }
}

void	ft_read_line(t_global *global)
{
	g_glb._status = 0;
	global->line = readline("minishell$> ");
	if (global->line == NULL)
	{
		write(1, "exitt\n", 6);
		exit(1);
	}
	if (global->line[0])
		add_history(global->line);
}

void	free_all(t_global *global)
{
	free(global->line);
	global->line = NULL;
	free_tokens(global->tokens);
}

int	main(int ac, char **av, char **env)
{
	t_global	global;

	if ((!ac && !av) || ac != 1)
	{
		write(2, "minishell :to many args .\n", 26);
		return (0);
	}
	stock_env(env, &global);
	init_global(&global);
	rl_catch_signals = 0;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		ft_read_line(&global);
		tokenization(&global);
		check_tokens(&global);
		print_tokens(&global);
		if (global.errnum != 0)
			printferror(&global);
		if (global.tokens)
			execute(&global);
		free_all(&global);
	}
	return (0);
}
