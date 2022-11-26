#include "minishell.h"



// void r_red_in(char *file_name)
// {
//     int file;
    
//     file = open(file_name, O_WRONLY | O_CREAT);
//     if (file < 0)
//         return (ft_putendl_fd("Error opening the file descriptor.\n", 2));
//     dup2(file, STDIN_FILENO);
//     close (file);
// }

// int r_red_out(char *file_name)
// {
//     int file;

//         file = open(file_name, O_WRONLY | O_CREAT);
//         if (file < 0)
//         {
//             ft_putendl_fd("Error opening the file descriptor.\n", 2);
//             return (2);
//         }
//         dup2(file, STDOUT_FILENO);
//         close (file);
// }

// int a_append(char *file_name)
// {

//     int file;

//     file = open(file_name, O_WRONLY |O_CREAT | O_APPEND);
//     if (file < 0)
//     {
//         ft_putendl_fd("Error opening the file descriptor.\n", 2);
//         return (1);
//     }
//     dup2(file, STDOUT_FILENO);
//     close (file);
// }

char			*get_cwd(void)
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

int				if_exist(t_env **env_list, char *var_name)
{
	t_env		*current;
    int len;

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

int		check_name(char *cmd)
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

int				len_path(char **cmd)
{
	int			i;

	i = 0;
	while (cmd[i++] != NULL)
		;
	return (i - 1);
}

void			export(char **cmd, t_env **env_list)
{
	if ((len_path(cmd)) == 1)
		return (ft_print_env());
	if ((len_path(cmd)) != 2)
		return (ft_putendl_fd("EROOR EXPORT\n", 2));
	else if (!check_name(cmd[1]))
	{
		ft_putstr_fd("EROOR\n", 2);
		ft_putendl_fd("ERROR22\n", 2);
		return ;
	}
	if (if_exist(env_list, cmd[1]) == 0)
        write(1, "exist\n", 7);
	// 	addtolist(env_list, ft_strdup(cmd[1]), ft_strdup(cmd[2]));
	// else
	// 	modify_env(env_list, cmd[1], cmd[2]);
	return ;
}


void			oldpwd(char *cwd, t_env **env_list)
{
	bool		flag;
	char		*cwd_;
	char		*cmd[4];

	flag = false;
	cwd_ = NULL;
	if (cwd == NULL)
	{
		if (!(cwd_ = get_path(env_list, "PWD")))
			return ;
		cwd = cwd_;
		flag = true;
	}
	cmd[0] = "export";
	cmd[1] = ft_strjoin("OLDPWD=", cwd);
	cmd[2] = NULL;
	export(cmd, env_list);
	if (flag)
		ft_strdel(&cwd_);
	return ;
}


int ft_exit(char **args, t_env **env)
{
    // free_env
    //free_tokens
    (void)env;
    (void)args;
    exit(0);
}




int pwd(void)
{
    char *ret;

    ret = getcwd(NULL, 0);
    ft_putendl_fd(ret, 1);
    return (0);
}


void ft_free(char **splt, int i)
{
    while (i >= 0)
        free(splt[i--]);
    free(splt);
}


// char *ft_getenv(char *str)
// {
// 	t_env	*env_list;
//     char    *path;
//     size_t     len;

// 	env_list = *get_adress();
// 	while (env_list)
// 	{
//         len = len_key(env_list->str);
//         if (!ft_strncmp(str, env_list->str, len))
//         {
//             path = ft_substr(env_list->str, len, ft_strlen(env_list->str) - len);
//             return(path);
//         }
// 		if (!env_list->next)
// 			break;
// 		env_list = env_list->next;
// 	}
//     return (NULL);
// }

int is_a_builtin(char *cmd) //pwd, export,env, exit, echo, unset, cd
{
    int len;

    len = ft_strlen(cmd);

    if (!cmd)
        return (1);
    else if (len == 4 && !ft_strncmp(cmd, "exit", len))
        return (0);
    // else if (len == 5 && !ft_strncmp(cmd, "unset", len))
    //     return (0);
    else if (len == 2 && !ft_strncmp(cmd, "cd", len))
        return (0);
    return (1);
}

int is_a_builtin_child(char *cmd) //pwd, export,env, exit, echo, unset, cd
{
    int len;

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
    else if (len == 5 && !ft_strncmp(cmd, "unset", len))
        return (0);
    return (1);
}

int check_arg(int *i, char *str)
{
	int j;

	j = 1;
	while (str[j])
	{
		if (str[j] != 'n')
			return 0;
		j++;
	}
	*i += 1;
	return (1);
}

int echo(char **args)
{
    int i;
    int n;

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
    return(0);
}
