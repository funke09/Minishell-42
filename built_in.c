#include "minishell.h"



void r_red_in(char *file_name)
{
    int file;
    
    file = open(file_name, O_WRONLY | O_CREAT);
    if (file < 0)
        return (ft_putendl_fd("Error opening the file descriptor.\n", 2));
    dup2(file, STDIN_FILENO);
    close (file);
}

void r_red_out(char *file_name)
{
    int file;

        file = open(file_name, O_WRONLY | O_CREAT);
        if (file < 0)
        {
            ft_putendl_fd("Error opening the file descriptor.\n", 2);
            return (2);
        }
        dup2(file, STDOUT_FILENO);
        close (file);
}

int a_append(char *file_name)
{

    int file;

    file = open(file_name, O_WRONLY |O_CREAT | O_APPEND);
    if (file < 0)
    {
        ft_putendl_fd("Error opening the file descriptor.\n", 2);
        return (1);
    }
    dup2(file, STDOUT_FILENO);
    close (file);
}


int ft_exit()
{
    // free_env
    //free_tokens
    exit(0);
}



int  c_cd(char *path)
{
    chdir(path);
    return (0);
}

int pwd(void)
{
    char *ret;

    ret = getcwd(NULL, 0);
    

    return (0);
}



void ft_free(char **splt, int i)
{
    while (i >= 0)
        free(splt[i--]);
    free(splt);
}




char *ft_getenv(char *str)
{
	t_env	*env_list;
    char    *path;
    size_t     len;

	env_list = *get_adress();
	while (env_list)
	{
        len = len_key(env_list->str);
        if (!ft_strncmp(str, env_list->str, len))
        {
            path = ft_substr(env_list->str, len, ft_strlen(env_list->str) - len);
            return(path);
        }
		if (!env_list->next)
			break;
		env_list = env_list->next;
	}
    return (NULL);
}

int is_a_builtin(char *cmd) //pwd, export,env, exit, echo, unset, cd
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
    else if (len == 4 && !ft_strncmp(cmd, "exit", len))
        return (0);
    else if (len == 4 && !ft_strncmp(cmd, "echo", len))
        return (0);
    else if (len == 5 && !ft_strncmp(cmd, "unset", len))
        return (0);
    else if (len == 2 && !ft_strncmp(cmd, "cd", len))
        return (0);
    return (1);
}

int echo(char **args)
{
    int i;
    int n;

    i = 0;
    n = 0;
    if (strncmp(args[i], "-n", 2) && !args[2])
    {
        n = 1;
        i++;
    }
    while (args[i])
    {
        ft_putendl_fd(args[i++], 1);
        if (args[i])
            ft_putendl_fd(" ", 1);   
    }
    if (n = 0)
        ft_putendl_fd("\n", 1);
}

int herredoc()
{
    
}