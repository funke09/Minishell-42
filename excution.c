#include "minishell.h"

int len_of_cmd_sin_pipes(t_tokens *token)
{
    int i;

    i = 0;
    while (token && token->type != PIPE)
    {
        token = token->next;
        i++;
    }
    return (i);
}

char **get_cmd(t_tokens *token)
{
    int i;
    int len;
    char **cmd;

    len = len_of_cmd_sin_pipes(token);
    cmd = (char**)malloc(sizeof(char *) * (len + 1));
    i = 0;
    while (token && token->type != PIPE)
    {
        if (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND)
            token = token->next;
        else if(token->type == COMMAND || token->type == FLAG || token->type == PARAM || token->type == D_QUOTE || token->type == S_QUOTE || token->type == ENV_VAR)
            cmd[i++] = ft_strdup(token->token);
        if (!token)
            break; 
        token = token->next;
    }
    cmd[i] = 0;
    return (cmd);
}

t_tokens *skip_to_pipe(t_tokens *token)
{
    while (token && token->type != PIPE)
    {
        token = token->next;
    }
    if (token == NULL || token->next == NULL)
    {
        // printf("NULL EROOOR");
        return (NULL);
    }
    return(token->next);
}

int is_valid_cmd(const char *cmd)
{
    struct stat buf;
    if (stat(cmd, &buf) == -1)
        return (1);
	if (buf.st_mode & S_IXUSR )
		return (0);
    return(2);
}

void			ft_execve(const char *file_name, char **cmd, char **env)
{
	if ((!is_valid_cmd(file_name)))
	{
		if (execve(file_name, cmd, env) == -1)
		{
			ft_putendl_fd("21sh: Error: Execution Failed.", 2);
			exit(1);
		}
		exit(0);
	}
	else
	{
		ft_putendl_fd("21sh: Command not found: ", 2);
		exit (1);
	}

	return ;
}

void	execute_blt(char **args, t_env **env)/////// ret int
{
	int pid = fork(), ret;
	if (pid == -1)
	{atus
		ft_putendl_fd("Error while forking a child.\n");
		return;
	}
	if (pid == 0)
	{
		ret = do_builtin(args, env);
		if (ret != 0)
		{
			ft_putendl_fd("Error while executing ", 2);
			ft_putendl_fd(args[0], 2);
			ft_putendl_fd("\n");
		}
		else return (0);
	}
	else
	{
		int wstatus;
		wait(&wstatus);
		// if (WIFEXITED(wstatus)) // if exited return true if processs terminated normaly  
			return (WEXITSTATUS(wstatus)); //w exit status
	}


}

void			execute_direct(char **cmd)
{
	const char	*file_name;
	pid_t		pid;

	if (cmd[0][0] == '.' && cmd[0][1] == '/')
		file_name = ft_strrchr(cmd[0], '/') + 1;
	else
		file_name = cmd[0];
	pid = fork();
	if (pid < 0)
		return (ft_putendl_fd("21sh: Error: forking Failded.\n", 2));
	if (pid == 0)
	{
		if (!is_valid_cmd(file_name))
			ft_execve(file_name, cmd, NULL);
		else
			ft_putendl_fd("21sh: no such file or directory: ", 2);//perror("");
		exit(1);
	}
	else
		wait(NULL);
}

void			execute_undirect(char **cmd, char **tabs, t_env **env)
{
	char		*bin_file;
	pid_t		pid;

	if (!(bin_file = get_bin_file(cmd, env)))
	{
		ft_putendl_fd("21sh: command not found: ", 2);
		return ;
	}
	if ((pid = fork()) < 0)
		return (ft_putendl_fd("21sh: Error: forking Failded.", 2));
	if (pid == 0)
	{
		if (access(bin_file, F_OK) == 0)
			ft_execve(bin_file, cmd, tabs);
		else
			ft_putendl_fd("21sh: no such file or directory: ", 2);
		exit(1);
	}
	else
		wait(NULL);//exit status
	if (bin_file)
		ft_strdel(&bin_file);
}

static void		init_pipes(t_pipe *pipes)//???
{
	pipes->temp = 0;
	pipes->cmd_no = 0;
	pipes->pipe[0] = 0;
	pipes->pipe[1] = 0;
	pipes->pid = 0;
}

int is_there_pipe(t_tokens *head)
{
    while (head)
    {
        if (head->type == PIPE)
            return 1;
        head = head->next;
    }
    return 0;
}

void		execute_pipes2(t_tokens *token, t_pipe *pipes)
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

void		execute_pip_child(t_tokens *head, t_pipe *pipes, char **cmd, t_env **env)/////////////////////////////////////////////
{
	execute_pipes2(head, pipes);
	// if (tree->redirection)
	// 	execute_redirection(tree->redirection, g_tty_name);
	// if (!tree->pipe && pipes->cmd_no)
	//      close(pipes->temp);
	if (is_a_builtin(cmd[0]))
		execute_blt(args,env);
    else 
	if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/'))
		execute_direct(cmd);
	else if (cmd[0])
		execute_undirect(cmd, NULL, env);
	exit(EXIT_SUCCESS);
}


void    execute_pipes(t_tokens *token, char **cmd, t_pipe *pipes, t_env **env)
{
    if (pipe(pipes->pipe) == -1)
		return ;
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
		pipes->cmd_no ++;
	}
	return ;
}

int execute(t_global *global)
{
    t_pipe	pipes;

	init_pipes(&pipes);	//pipes->temp = 0; pipes->cmd_no = 0; pipes->pipe[0] = 0; pipes->pipe[1] = 0; pipes->pid = 0;
    t_tokens *token;
    char **cmd;

    head = global->tokens;
    while (token)
    {
        int j = 0;
        cmd = get_cmd(token);
        // while (cmd[j])
        //     ft_putstr_fd(cmd[j++], 1);
        // ft_putstr_fd("---------\n", 1);
        execute_pipes(token, cmd, &pipes, global->env);
        // execute_direct(cmd, NULL);
        j = 0;
        while (cmd[j])
            free(cmd[j++]);
        free(cmd);
        token = skip_to_pipe(token);
    }
    close(pipes.temp);
	if (pipes.pid)
		while (wait(NULL) > 0)
			continue;
    return (0);
}

int c_cd(char **args)
{
	if (!args[2])
	{
		ft_putendl_fd("sh: cd: too many arguments.\n");
		return (1);
	}
    chdir(args[1]);
	return (0);
}

int do_builtin(char **args, t_env **env)
{
	int len;

    len = ft_strlen(cmd);

    if (!cmd)
        return (1);
    if (len == 3 && !ft_strncmp(cmd, "pwd", len))
        return(pwd());
    else if (len == 6 && !ft_strncmp(cmd, "export", len))
        return (ft_export(args, env));
    else if (len == 3 && !ft_strncmp(cmd, "env", len))
        return (ft_env());
    else if (len == 4 && !ft_strncmp(cmd, "exit", len))
		return (ft_exit(args, env));
    else if (len == 4 && !ft_strncmp(cmd, "echo", len))
        return (echo(args));
    else if (len == 5 && !ft_strncmp(cmd, "unset", len))
        return (ft_unset(args, env));
    else if (len == 2 && !ft_strncmp(cmd, "cd", len))
        return (c_cd(args));
}

// char *get_path()
// {

// }