/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:33 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/25 18:15:43 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

void	sig_handler(int var)
{
	while (wait(NULL) > 0)
		;
	signal(SIGQUIT, SIG_IGN);
	if (var == SIGINT)
	{
		g_var = 1;
		ft_putstr_fd("\n\e[1;35mminishell$> \e[0m", 1);
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

int	is_flag(t_global *global, int	*i)
{
	int	start;

	start = *i;
	if (global->line[*i] == '-' && !is_blank(global->line[*i + 1])
		&& ft_isalpha(global->line[*i + 1]))
	{
		(*i)++;
		while (ft_isalpha(global->line[*i]))
			(*i)++;
		return (1);
	}
	*i = start;
	return (0);
}

int	is_quote(t_global *global, int	*i, char c)
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
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_command(t_global *global, int	*i)
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
			global->cmd_status = 1;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_param(t_global *global, int	*i)
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
			global->is_redir = 0;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_dolar(t_global *global, int	*i)
{
	int	start;

	start = *i;
	if (global->line[*i] && global->line[*i] == '$' && global->line[*i + 1])
	{
		(*i)++;
		while (global->line[*i] && !is_blank(global->line[*i])
			&& !is_charachter(global->line[*i]))
			(*i)++;
		return (1);
	}
	*i = start;
	return (0);
}

t_type	type(t_global *global, int	*i)
{
	if (is_heredoc_key(global, i))
		return (HERDOC_KEY);
	else if (is_quote(global, i, '\''))
		return (S_QUOTE);
	else if (is_quote(global, i, '\"'))
		return (D_QUOTE);
	else if (global->line[*i] == '|')
	{
		(*i)++;
		global->cmd_status = 0;
		return (PIPE);
	}
	else if (is_dolar(global, i))
		return (ENV_VAR);
	else if (is_heredoc_or_append(global, i, '>'))
		return (APPEND);
	else if (is_heredoc_or_append(global, i, '<'))
		return (HEREDOC);
	else if (is_redir(global, i, '<'))
		return (REDIR_IN);
	else if (is_flag(global, i))
		return (FLAG);
	else if (is_redir(global, i, '>'))
		return (REDIR_OUT);
	else if (is_command(global, i))
		return (COMMAND);
	else if (is_param(global, i))
		return (PARAM);
	return (NON);
}

void skip_blanks(t_global *global, int	*i)
{
	while (is_blank(global->line[*i]))
		(*i)++;
}

t_tokens	*add_token(t_global *global, int	*i)
{
	int			len;
	int			start;
	t_tokens	*new;

	skip_blanks(global, i);
	start = *i;
	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	new->type = type(global, i);
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

void	ft_print_env(void)/////key=value
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

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	main(int ac, char **av, char **env)
{
	t_global	global;

	g_var = 0;
	stock_env(env, &global);
	init_global(&global);
	// rl_catch_signals = 0;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	if ((!ac && !av) || ac != 1)
	{
		write(2, "minishell :to many args .\n", 26);
		return (0);
	}
	while (1)
	{
		global.line = readline("\e[1;35mminishell$> \e[0m");
		add_history(global.line);
		if (global.line == NULL)
		{
			write(1, "exitt\n", 6);
			exit(1);
		}
		if (ft_strcmp(global.line, "exit") == 0)
		{
			write(1, "exit\n", 6);
			free(global.line);
			//+free it all
			exit(1);
		}
		tokenization(&global);
		check_tokens(&global);
		if (global.errnum != 0)
			printferror(&global);
		if (global.tokens)
			execute(&global);
		free(global.line);
		global.line = NULL;
		free_tokens(global.tokens);
	}
	return (0);
}
