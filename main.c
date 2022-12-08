/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 01:24:44 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 01:24:45 by flazerak         ###   ########.fr       */
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

void	ft_read_line(t_global *global)
{
	g_glb._status = 0;
	global->line = readline(BLUMG"minishell$> "END);
	if (global->line == NULL)
	{
		write(1, "exit\n", 6);
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
		if (global.errnum != 0)
			printferror(&global);
		if (global.tokens)
			execute(&global);
		free_all(&global);
	}
	return (0);
}
