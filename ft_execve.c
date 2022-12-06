#include "minishell.h"

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

void	ft_wait(int *status)
{
	wait(status);
	if (WIFEXITED(status))
		g_glb.exit_status = WEXITSTATUS(status);
}

void	execute_direct(char **cmd, char **env)
{
	const char	*file_name;
	pid_t		pid;
	int			status;

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
		ft_wait(&status);
}

void	execute_undirect(char **cmd, char **tabs, t_env **env)
{
	char		*bin_file;
	pid_t		pid;
	int			status;

	bin_file = get_bin_file(cmd, env);
	if (!bin_file)
	{
		g_glb.exit_status = 127;
		return (ft_putendl_fd("minishell: command not found ", 2));
	}
	g_glb._status = 1;
	pid = fork();
	if (pid < 0)
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
		ft_wait(&status);
	ft_strdel(&bin_file);
}

