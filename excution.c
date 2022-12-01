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

t_tokens	*join_expantion(t_tokens *token, char **cmd, int i)
{
	char *str;
	char *res;
	char *tmp = NULL;

	str = token->token;
	res = NULL;
	while (token && token->no_space == 1 && token->next)
	{
		tmp = res;
		if (!(res = ft_strjoin(str, token->next->token)))
			return (NULL);
		printf(">>>>>>>>>>>>>>>> %s\n <<<<<", res);
		if (tmp)
			free(tmp);
		// if (str)
		// 	free(str);
		str = res;
		token = token->next;
	}
	cmd[i] = res;
	return (token->next);
}

char	**get_cmd(t_tokens *token)
{
	int		i;
	int		len;
	char	**cmd;

	len = len_of_cmd_sin_pipes(token);
	cmd = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == REDIR_IN || token->type == REDIR_OUT
			|| token->type == APPEND)
				token = token->next;
		else if (token->type == COMMAND || token->type == FLAG
			|| token->type == PARAM
			|| token->type == D_QUOTE || token->type == S_QUOTE
			|| token->type == ENV_VAR)
			{
				if ((token->type == ENV_VAR || token->type == COMMAND || token->type == FLAG
					|| token->type == PARAM
					|| token->type == D_QUOTE || token->type == S_QUOTE)
					&& token->no_space && token->next)
				{
					token = join_expantion(token, cmd, i);
					i++;
					continue ;
					// token = token->next;
				}
				else
					cmd[i++] = ft_strdup(token->token);
			}
		if (!token)
			break ;
		token = token->next;
	}
	cmd[i] = 0;
	return (cmd);
}

t_tokens	*skip_to_pipe(t_tokens *token)
{
	while (token && token->type != PIPE)
	{
		token = token->next;
	}
	if (token == NULL || token->next == NULL)
		return (NULL);
	return (token->next);
}

int	is_valid_cmd(const char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == -1)
		return (1);
	if (buf.st_mode & S_IXUSR)
		return (0);
	return (2);
}

void	ft_execve(const char *file_name, char **cmd, char **env)
{
	if (access(file_name, X_OK) == 0)
	{
		if (execve(file_name, cmd, env) == -1)
		{
			ft_putendl_fd("minishell: Error: Execution Failed.", 2);
			exit(1);
		}
		exit(0);
	}
	else
	{
		ft_putendl_fd("minishell: Command not found ", 2);
		exit (1);
	}
	return ;
}

// int	execute_blt(char **args, t_env **env)
// {
// 	int	ret;

// 	ret = do_builtin(args, env);
// 	if (ret != 0)
// 	{
// 		ft_putendl_fd("Error while executing ", 2);
// 		ft_putendl_fd(args[0], 2);
// 		ft_putendl_fd("\n", 2);
// 	}
// 	return (0);
// }

void	execute_direct(char **cmd, char **env)
{
	const char	*file_name;
	pid_t		pid;
	int	status;

	if (cmd[0][0] == '.' && cmd[0][1] == '/')
		file_name = ft_strrchr(cmd[0], '/') + 1;
	else
		file_name = cmd[0];
	g_glb._status = 1;
	pid = fork();
	if (pid < 0)
		return (ft_putendl_fd("minishell: Error: forking Failded.\n", 2));
	if (pid == 0)
	{
		if (!is_valid_cmd(file_name))
			ft_execve(file_name, cmd, env);
		else
			ft_putendl_fd("minishell: no such file or directory: ", 2);
		exit(1);
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			g_glb.exit_status = WEXITSTATUS(status);
	}
}

void	execute_undirect(char **cmd, char **tabs, t_env **env)
{
	char		*bin_file;
	pid_t		pid;
	int			status;

	if (!(bin_file = get_bin_file(cmd, env)))
	{
		ft_putendl_fd("minishell: command not found ", 2);
		g_glb.exit_status = 127;
		return ;
	}
	g_glb._status = 1;
	if ((pid = fork()) < 0)
		return (ft_putendl_fd("minishell: Error: forking Failded.", 2));
	if (pid == 0)
	{
		if (access(bin_file, F_OK) == 0)
			ft_execve(bin_file, cmd, tabs);
		else
			ft_putendl_fd("minishell: no such file or directory: ", 2);
		exit(1);
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			g_glb.exit_status = WEXITSTATUS(status);
	}
	if (bin_file)
		ft_strdel(&bin_file);
}

static void		init_pipes(t_pipe *pipes)
{
	pipes->temp = 0;
	pipes->cmd_no = 0;
	pipes->pipe[0] = 0;
	pipes->pipe[1] = 0;
	pipes->pid = 0;
}

int	is_there_pipe(t_tokens *head)
{
	while (head)
	{
		if (head->type == PIPE)
			return (1);
		head = head->next;
	}
	return (0);
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

void	execute_pip_child(t_tokens *head, t_pipe *pipes, char **cmd, t_env **env)/////////////////////////////////////////////
{
	t_tokens	*redir;
	char		**tabs;

	redir = NULL;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	execute_pipes2(head, pipes);
	if ((redir = go_to_redir(head)))
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
	if (cmd && !is_a_builtin(cmd) )
	{
		do_builtin(cmd, env);
		return ;
	}
	if (pipe(pipes->pipe) == -1)
		return ;
	g_glb._status = 1;
	if ((pipes->pid = fork()) == -1)
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
	int 		j;
	int status;

	env = get_adress();
	init_pipes(&pipes);
	token = global->tokens;
	while (token)
	{
		j = 0;
		cmd = get_cmd(token);

		// if (cmd && cmd[0])
		// while (cmd[j])
		// 	ft_putendl_fd(cmd[j++], 1);
		execute_pipes(token, cmd, &pipes, env);
		j = 0;
		while (cmd[j])
			free(cmd[j++]);
		free(cmd);
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

int	ft_sizearray(char **args)
{
	int	i;

	i = 0 ;
	while (args[i])
		i++;
	return (i);
}


// void			oldpwd2(char *cwd, t_env **env_list)
// {
// 	bool		flag;
// 	char		*cwd_;
// 	char		*cmd[4];

// 	flag = false;
// 	cwd_ = NULL;
// 	if (cwd == NULL)
// 	{
// 		if (!(cwd_ = get_path(env_list, "PWD")))
// 			return ;
// 		cwd = cwd_;
// 		flag = true;
// 	}
// 	cmd[0] = "export";
// 	cmd[1] = ft_strjoin("OLDPWD=", cwd);
// 	cmd[2] = NULL;
// 	export(cmd, env_list);
// 	if (flag)
// 		ft_strdel(&cwd_);
// 	return ;
// }

void oldpwd_var(t_env **env_list)
{
	char **tmp;

	tmp = malloc(3 * sizeof(char *));
	tmp[2]= NULL;
	tmp[1] = ft_strjoin("OLDPWD=", getcwd(NULL, 0));
	tmp[0] = ft_strdup("export");

	ft_export(env_list, tmp);
	ft_free(tmp, 1);
}

void pwd_var(t_env **env_list)
{
	char **tmp;

	tmp = malloc(3 * sizeof(char *));
	tmp[2]= NULL;
	tmp[1] = ft_strjoin("PWD=", getcwd(NULL, 0));
	tmp[0] = ft_strdup("export");

	ft_export(env_list, tmp);

	ft_free(tmp, 1);
}



void	go_to_cd(char *path, t_env **env_list)
{
	struct stat	st;
	(void)env_list;

	if (path && access(path, F_OK) == 0)
	{
		stat(path, &st);
		if (!S_ISDIR(st.st_mode) )
		{
			return (ft_putendl_fd("minishell : cd: not a directory", 2));
		}
	}
		if (access(path, X_OK) == 0)
		{
			oldpwd_var(env_list);
			// oldpwd((tmp = get_cwd()), env_list);
			chdir(path);
			pwd_var(env_list);
			// generate_pwd((tmp2 = get_cwd()), env_list);
			// ft_strdel_2(&tmp, &tmp2);
		}
	// 	else if (ft_strrchr(path, '/'))
	// 		ft_putendl_fd_error(ERROR5, ft_strrchr(path, '/') + 1, "\n", NULL);
	// }
	// else if (ft_strrchr(path, '/'))
	// 	ft_putendl_fd_error(ERROR6, ft_strrchr(path, '/') + 1, "\n", NULL);
	// else
	// 	ft_putendl_fd_error(ERROR6, path, "\n", NULL);
}

int	c_cd(char **args, t_env **env)
{
	char	*path;
	// char	*cwd;
	int		len;

	len = ft_sizearray(args);
	if (len > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments.\n", 2);
		return (1);
	}
	if (len ==  1)
	{
		if (!(path = get_path(env, "HOME")))
			return (1);
		go_to_cd(path, env);
	}
	if (len == 2)
	{
		go_to_cd(args[1], env);
		// if (args[1][0] != '/' && args[1][0] != '.' && args[1][0] != '-')
		// {
		// 	if (!(cwd = get_cwd()))
		// 		return (1);
		// 	ft_strcpy(cwd + ft_strlen(cwd), "/");
		// 	if (!(path = ft_strjoin(cwd, args[1])))
		// 		return (1);
		// 	go_to_cd(path, env);
		// 	// cd_simple(new_path, env_list);
		// 	free(path);
		// 	free(cwd);
		// 	// ft_strdel_2(&new_path, &cwd);
		// }
		// else if (cmd[1][0] == '-' && cmd[1][1] == '\0')
		// 	cd_back(env_list);
		// else
			// go_to_cd(args[1], env);
	}
	// go_to_cd(path, env);
	// if len = 1
	// go home(env)
	return (0);
}

int	do_builtin(char **args, t_env **env)
{
	int	len;

	len = ft_strlen(args[0]);

	if (!args[0])
		return (1);
	if (len == 3 && !ft_strncmp(args[0], "pwd", len))
		return (pwd());
	else if (len == 6 && !ft_strncmp(args[0], "export", len))
			return (ft_export(env, args));
	else if (len == 3 && !ft_strncmp(args[0], "env", len))
		return (ft_env(args));
	else if (len == 4 && !ft_strncmp(args[0], "exit", len))
		return (ft_exit(args, env));
	else if (len == 4 && !ft_strncmp(args[0], "echo", len))
		return (echo(args));
	else if (len == 5 && !ft_strncmp(args[0], "unset", len))
		return (ft_unset(env, args));
	else if (len == 2 && !ft_strncmp(args[0], "cd", len))
		return (c_cd(args, env));
	return (1);
}
