#include "minishell.h"

char	*get_cwd(void)
{
	char		tmp[4096];
	char		*buff;

	buff = malloc(sizeof(char) * 4096);
	if (!buff)
		return (NULL);
	if (!getcwd(tmp, sizeof(tmp)))
		ft_strdel(&buff);
	else
		ft_strcpy(buff, tmp);
	return (buff);
}

int	if_exist(t_env **env_list, char *var_name)
{
	t_env		*current;
	int			len;

	if (var_name && *env_list)
	{
		current = *env_list;
		while (current)
		{
			len = len_key(current->str);
			if (ft_strncmp(current->str, var_name, len))
				return (1);
			current = current->next;
		}
		return (0);
	}
	return (-1);
}

int	check_name(char *cmd)
{
	int		i;

	i = 0;
	if (!cmd || !ft_isalpha(*cmd))
		return (0);
	while (cmd[i])
	{
		if (i && !ft_isalnum(cmd[i]))
			return (0);
		i++;
	}
	return (1);
}

int	len_path(char **cmd)
{
	int			i;

	i = 0;
	while (cmd[i++] != NULL)
		;
	return (i - 1);
}

int	ft_exit(char **args, t_env **env)
{
	// free_env
	//free_tokens
	(void)env;
	(void)args;
	exit(0);
}

int	pwd(void)
{
	char	*ret;

	ret = getcwd(NULL, 0);
	ft_putendl_fd(ret, 1);
	return (0);
}

void	ft_free(char **splt, int i)
{
	while (i >= 0)
		free(splt[i--]);
	free(splt);
}

int is_a_builtin(char **cmd)
{
	int	len;

	if (!cmd[0])
		return (1);
	len = ft_strlen(cmd[0]);
	if (len == 4 && !ft_strncmp(cmd[0], "exit", len))
	{
		ft_putstr_fd("exit\n", 1);
		exit(0);
	}
	else if (len == 2 && !ft_strncmp(cmd[0], "cd", len))
		return (0);
	else if (len == 6 && !ft_strncmp(cmd[0], "export", len) && cmd[1] != NULL)
		return (0);
	else if (len == 5 && !ft_strncmp(cmd[0], "unset", len))
		return (0);
	return (1);
}

int	is_a_builtin_child(char *cmd)
{
	int	len;

	len = ft_strlen(cmd);
	if (!cmd)
		return (1);
	if (len == 3 && !ft_strncmp(cmd, "pwd", len))
		return (0);
	else if (len == 6 && !ft_strncmp(cmd, "export", len))
		return (0);
	else if (len == 3 && !ft_strncmp(cmd, "env", len))
		return (0);
	else if (len == 4 && !ft_strncmp(cmd, "echo", len))
		return (0);
	return (1);
}

int check_arg(int *i, char *str)
{
	int	j;

	j = 1;
	while (str[j])
	{
		if (str[j] != 'n')
			return (0);
		j++;
	}
	*i += 1;
	return (1);
}

int	echo(char **args)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	if (args[1] && args[1][0] == '-')
		n = check_arg(&i, args[1]);
	while (args[i])
	{
		ft_putstr_fd(args[i++], 1);
		if (args[i])
			ft_putchar_fd(' ', 1);
	}
	if (!n)
		ft_putchar_fd('\n', 1);
	return (0);
}
