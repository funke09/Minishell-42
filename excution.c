#include "minishell.h"

void	init_pipes(t_pipe *pipes)
{
	pipes->temp = 0;
	pipes->cmd_no = 0;
	pipes->pipe[0] = 0;
	pipes->pipe[1] = 0;
	pipes->pid = 0;
}

void	execute_pipes2(t_tokens *token, t_pipe *pipes)
{
	close(pipes->pipe[0]);
	if (pipes->cmd_no != 0)
	{
		dup2(pipes->temp, STDIN_FILENO);
		close(pipes->temp);
	}
	if (is_there_pipe(token))
		dup2(pipes->pipe[1], STDOUT_FILENO);
	close(pipes->pipe[1]);
	return ;
}

void	execute_pip_child(t_tokens *head, t_pipe *pipes,
	char **cmd, t_env **env)
{
	t_tokens	*redir;
	char		**tabs;

	redir = NULL;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	execute_pipes2(head, pipes);
	redir = go_to_redir(head);
	if (redir)
		execute_redirection(redir);
	tabs = list_to_tabs(env);
	if (cmd[0] && !is_a_builtin_child(cmd[0]))
	{
		do_builtin(cmd, env);
		exit(EXIT_SUCCESS);
	}
	if (cmd[0] && (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/')))
		execute_direct(cmd, tabs);
	else if (cmd[0])
		execute_undirect(cmd, tabs, env);
	exit(g_glb.exit_status);
}

void	execute_pipes(t_tokens *token, char **cmd, t_pipe *pipes, t_env **env)
{
	if (cmd && !is_a_builtin(cmd))
	{
		do_builtin(cmd, env);
		return ;
	}
	if (pipe(pipes->pipe) == -1)
		return ;
	g_glb._status = 1;
	pipes->pid = fork();
	if (pipes->pid == -1)
		return ;
	if (pipes->pid == 0)
		execute_pip_child(token, pipes, cmd, env);
	else
	{
		close(pipes->pipe[1]);
		if (pipes->temp)
			close(pipes->temp);
		pipes->temp = pipes->pipe[0];
		if (!is_there_pipe(token))
			close(pipes->temp);
		pipes->cmd_no++;
	}
	return ;
}

int	execute(t_global *global)
{
	t_pipe		pipes;
	t_env		**env;
	t_tokens	*token;
	char		**cmd;
	int			status;

	env = get_adress();
	init_pipes(&pipes);
	token = global->tokens;
	while (token)
	{
		cmd = get_cmd(token);
		execute_pipes(token, cmd, &pipes, env);
		ft_free_arr(cmd);
		token = skip_to_pipe(token);
	}
	if (pipes.temp)
		close(pipes.temp);
	if (pipes.pid)
		while (wait(&status) > 0)
			;
	if (WIFEXITED(status))
		g_glb.exit_status = WEXITSTATUS(status);
	return (0);
}
