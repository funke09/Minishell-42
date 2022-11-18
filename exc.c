#include "minishell.h"

//if there is a / in a cmd???

int is_a_builtin(char *cmd) //pwd, export,env, exit, echo, unset, cd
{
    int len;

    len = ft_strlen(cmd);

    if (!cmd)
        return (1);
    /////////ls
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

// ignore the flags if not valid


// int is_valid_cmd(char *cmd)
// {
//     return(1);
// }

int p_pwd(void)
{
    char *tmp;

    tmp = getcwd(NULL, 0);
    ft_putendl_fd(tmp, 1);
    free(tmp);
    return (0);
}

int e_echo(char *str)
{
    ft_putendl_fd(str, 1);
    return (0);
}

void r_red_out(char *file_name)
{
    struct stat buf;
    int file;
    
    if (stat(file_name, &buf) == -1)
        perror("");
    else
    {  
    file = open("txt.txt", O_WRONLY ); //O_APPEND//////////////////>>
    dup2(file, STDOUT_FILENO);
    close (file);
    }
}

void a_append(char *file_name)
{
    struct stat buf;
    int file;
    
    if (stat(file_name, &buf) == -1)
        perror("");
    else
    {  
    file = open("txt.txt", O_WRONLY | O_APPEND); 
    close (file);
    }
}

// void ft_print_env(void)
// {
// 	t_env	*env_list;

// 	env_list = *get_adress();
// 	while (env_list)
// 	{
// 		ft_putstr_fd(env_list->str, 1);
// 		ft_putchar_fd('\n', 1);
// 		if (!env_list->next)
// 			break;
// 		env_list = env_list->next;
// 	}

// }

// init env if empty






int l_ls(char **args)
{
//   char *args[] = {"ls", "-la", "/home/fzara/Desktop/mmmmmmmmm", NULL};

    int fd[2];
    int pid;
    t_env	*env_list;

	env_list = *get_adress();
    pid = fork();
    if (pid == -1)
    {
        printf("%s\n",strerror(errno));
        return (1);
    }
    if (pid == 0)
    {
        execve(args[0], args, env_list);
    }
    else
    {
        int wstatus;
        wait(&wstatus);
        int statuscode = WEXITSTATUS(wstatus);
        return (statuscode);
    }
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


char *get_path(char *cmd)
{
    char *path;
    char **splt;
    int l;

    path = ft_getenv(cmd);


    splt = ft_split;

    return (path);
}

// int p_pipe()
// {
//     int pid;
//     int fd[2];

//     pid = fork();
// }

int main()
{

}