// #include "minishell.h"



// int is_a_builtin(char *cmd) //pwd, export,env, exit, echo, unset, cd
// {
//     int len;

//     len = ft_strlen(cmd);

//     if (!cmd)
//         return (1);
//     if (len == 3 && !ft_strncmp(cmd, "pwd", len))
//         return (0);
//     else if (len == 6 && !ft_strncmp(cmd, "export", len))
//         return (0);
//     else if (len == 3 && !ft_strncmp(cmd, "env", len))
//         return (0);
//     else if (len == 4 && !ft_strncmp(cmd, "exit", len))
//         return (0);
//     else if (len == 4 && !ft_strncmp(cmd, "echo", len))
//         return (0);
//     else if (len == 5 && !ft_strncmp(cmd, "unset", len))
//         return (0);
//     else if (len == 2 && !ft_strncmp(cmd, "cd", len))
//         return (0);
//     return (1);
// }

// void    exec_builtin()
// {
//     if ()
//     {
//         printf("%s\n", getcwd(NULL, 0));
//         return ;
//     }

// }

// //if flag not valid invalid option


// int is_valid_cmd(char *cmd)
// {
//     struct stat buf;
//     if (stat(cmd, &buf) == -1)
//         return (1);

//     return(0);
// }



// void r_red_in(char *file_name)
// {
//     struct stat buf;
//     int file;
    
//     if (stat(file_name, &buf) == -1)
//         perror("");
//     else
//     {  
//         file = open(file_name, O_WRONLY | O_CREAT);
//         dup2(file, STDIN_FILENO);
//         close (file);
//     }
// }

// void r_red_out(char *file_name)
// {
//     struct stat buf;
//     int file;
    
//     if (stat(file_name, &buf) == -1)
//         perror("");
//     else
//     {  
//         file = open(file_name, O_WRONLY | O_CREAT);
//         dup2(file, STDOUT_FILENO);
//         close (file);
//     }
// }

// void a_append(char *file_name)
// {
//     struct stat buf;
//     int file;
    
//     if (stat(file_name, &buf) == -1)
//         perror("");
//     else
//     {  
//         file = open(file_name, O_WRONLY |O_CREAT | O_APPEND); 
//         dup2(file, STDOUT_FILENO);
//         close (file);
//     }
// }

// void ft_remove(t_env **env, char *var_name)
// {
//     int l;
//     t_env *curr;
//     t_env *tmp;

//     l = ft_strlen (var_name);
//     if (!env || !*env)
//         return;
//     while (*env && !ft_strncmp(var_name, (*env)->str, l))
//     {
//         tmp = (*env)->next;
//         free(*env);
//         *env = tmp;
//     }
//     curr = *env;
//     while (curr && curr->next)
//     {
//         if (!ft_strncmp(var_name, curr->next->str, l))
//         {
//             tmp = curr->next;
//             curr->next = curr->next->next;
//             free(tmp);
//         }
//         else 
//             curr = curr->next;
//     }
// }

// void ft_unset(t_env **env, char **args)
// {
//     int i = -1;

//     while(args[++i])
//     {
//         // check_valid args : only numbers, underscores, and digits.
//         ft_remove(env, args[i]);
//     }
    
    
// }

// void ft_exit()
// {
//     exit();
// }

// void ft_export(char **args)
// {
//     //push
//     //check if func exist if si strjoin
//     //v// check_valid args 
//     // if already exist str join

// }



// // void ft_print_env(void)
// // {
// // 	t_env	*env_list;

// // 	env_list = *get_adress();
// // 	while (env_list)
// // 	{
// // 		ft_putstr_fd(env_list->str, 1);
// // 		ft_putchar_fd('\n', 1);
// // 		if (!env_list->next)
// // 			break;
// // 		env_list = env_list->next;
// // 	}

// // }



// void ft_free(char **splt, int i)
// {
//     while (i >= 0)
//         free(splt[i--]);
//     free(splt);
// }




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

// // typedef enum s_type
// // {
// //     COMMAND, 
// //     PIPE,
// //     NON,
// //     S_QUOTE,
// //     D_QUOTE,
// //     HEREDOC,
// //     REDIR_IN,
// //     REDIR_OUT,
// //     APPEND,
// //     FLAG,
// //     PARAM,
// //     HERDOC_KEY,
// //     ENV_VAR,
// // } t_type;

// int nb_args(t_tokens *token)
// {
//     int i = 0;
//     while (!token && (token->type == FLAG || token->type == PARAM))
//     {
//         i++;
//         token = token->next;
//     }
//     return (i);
// }

// char *get_args(t_tokens *token, int *after_args)
// {
//     char **args;
//     int i = 0;;
//     args = malloc(nb_args(token) * sizeof(char *));
//     while (!token && (token->type == FLAG || token->type == PARAM))
//     {
//         args[i++] = token->token;
//         token = token->next;
//     }
//     args[i] = NULL;
//     if (!token)
//         *after_args = token->type;
//     return (args);
// }

// int fork_execve(char **args, int fd[2], int p)
// {
// //   char *args[] = {"ls", "-la", "/home/fzara/Desktop/mmmmmmmmm", NULL};

//     int ret;
//     int pid;
//     t_env	*env_list;

// 	env_list = *get_adress();

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("");
//     }
//     if (pid == 0)
//     {
//         if (p != -1)
//         {
//             if(p == 1)
//                 dup2(fd[1], STDOUT_FILENO);
//             else if(p = 0)
//                 dup2(fd[0], STDIN_FILENO);
//             close(fd[0]);
//             close(fd[1]);
//         }

//         ret = execve(args[0], args, NULL);
//         if (ret == -1)
//             perror("");
//     }
//     else
//     {
//         int wstatus;
//         wait(&wstatus);
//         int statuscode = WEXITSTATUS(wstatus);
//         return (statuscode);
//     }
    
// }


// // child_processs()
// // get_status

// // get_next

// void ft_execute(t_global *global)
// {
//     t_tokens *token;
//     int      after_args;
//     char    **args;
//     int fd[2];
//     int p = -1;


//     after_args = -1;
//     token = global->tokens;
//     if (!token)
//         return;
//     while (token)
//     {
//         args = get_args(token, &after_args);

//         if (after_args == PIPE)
//         {
//             if (pipe(fd) == -1)
//                 perror("");
//             p = 0;
//         }
//         else if (after_args == REDIR_IN)
//         {
            
//         }
//         else if (after_args == REDIR_OUT)
//         {
            
//         }
//         else if (after_args == APPEND)
//         {
            
//         }
//         if (token->type == COMMAND)
//         {
//             if (p = -1)
//                 fork_execve(args, fd, -1);
//             else
//                 fork_execve(args, fd, p);
//         }
//         else if (token->type == PIPE)
//         {
//             p = 1;
//         }
//         // skip args && params

//     }
// }


