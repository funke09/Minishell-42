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
    file = open("txt.txt", O_WRONLY | O_CREAT); //O_APPEND//////////////////>>
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
    file = open("txt.txt", O_WRONLY |O_CREAT | O_APPEND); 
    close (file);
    }
}

void ft_remove(t_env **env, char *var_name)
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

void ft_unset(t_env **env, char **args)
{
    int i = -1;

    while(args[++i])
    {
        // check_valid args : isdigit w c
        ft_remove(env, args[i]);
    }
    
    
}


void ft_export(char **args)
{
    //push
    // if already exist str join
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
            perror("");
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

// typedef enum s_type
// {
//     COMMAND, 
//     PIPE,
//     NON,
//     S_QUOTE,
//     D_QUOTE,
//     HEREDOC,
//     REDIR_IN,
//     REDIR_OUT,
//     APPEND,
//     FLAG,
//     PARAM,
//     HERDOC_KEY,
//     ENV_VAR,
// } t_type;

int nb_args(t_tokens *token)
{
    int i = 0;
    while (!token && (token->type == FLAG || token->type == PARAM))
    {
        i++;
        token = token->next;
    }
    return (i);
}

char *get_args(t_tokens *token, int *after_args)
{
    char **args;
    int i = 0;;
    args = malloc(nb_args(token) * sizeof(char *));
    while (!token && (token->type == FLAG || token->type == PARAM))
    {
        args[i++] = token->token;
        token = token->next;
    }
    args[i] = NULL;
    if (!token)
        *after_args = token->type;
    return (args);
}


void ft_execute(t_global *global)
{
    t_tokens *token;
    int      after_args;
    char    *args;


    after_args = -1;
    token = global->tokens;
    if (!token)
        return;
    while (token)
    {
        get_args(token, &after_args);

        if (token->type == PIPE || token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND)
            token = token->next;
        if (after_args == PIPE)
        {

        }
        if (after_args == REDIR_IN)
        {
            
        }
        if (after_args == REDIR_OUT)
        {
            
        }
        if (after_args == APPEND)
        {
            
        }
        if (token->type == COMMAND)
        {

        }
        // printf("token: %s, type: %d\n ", token->token, token->type);
        // token = token->next;
    }
}


