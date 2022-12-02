/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:33 by zcherrad          #+#    #+#             */
/*   Updated: 2022/12/02 17:44:39 by macos            ###   ########.fr       */
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

void print_tokens(t_global *global) //just for tesst
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
