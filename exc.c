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



//if flag not valid invalid option


int is_valid_cmd(char *cmd)
{
    struct stat buf;
    if (stat(cmd, &buf) == -1)
        return (1);

    return(0);
}

int p_pwd(void)
{

    printf("%s\n", getcwd(NULL, 0));

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

void ft_unset(t_env **env, void *var_name)
{
    int l;
    t_env *curr;
    t_env *tmp;

    l = ft_strlen (var_name);
    if (!env || !*env)
        return;
    while (*env && !ft_strncmp(var_name, (*env)->str, l))
    {
        tmp = (*env)->next;
        free(*env);
        *env = tmp;
    }
    curr = *env;
    while (curr && curr->next)
    {
        if (!ft_strncmp(var_name, curr->next->str, l))
        {
            tmp = curr->next;
            curr->next = curr->next->next;
            free(tmp);
        }
        else 
            curr = curr->next;
    }
}

void ft_execute()
{
    
}
void ft_export(char *var)
{
    //push
}

void c_cd(char *path)
{
    chdir(path);
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



void ft_free(char **splt, int i)
{
    while (i >= 0)
        free(splt[i--]);
    free(splt);
}



int do_cmd(char **args)
{
//   char *args[] = {"ls", "-la", "/home/fzara/Desktop/mmmmmmmmm", NULL};

    int ret;
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
        ret = execve(args[0], args, NULL);
        if (ret == -1)
            perror();
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
    int l, i = -1;
    char *tmp ;
    struct stat buf;

    path = ft_getenv(cmd);


    splt = ft_split(path, ':');
    while (splt[++i])
    {
        splt[i] = ft_strjoin(splt[i],"/");
        tmp = ft_strjoin(splt[i], cmd);
        if (stat(tmp, &buf) != -1)
        {
            // ft_free
            return(tmp);
        }
    }
    // ft_free(splt)
    return (NULL);
}
