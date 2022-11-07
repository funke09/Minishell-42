#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"



typedef enum e_type
{
    COMMAND,
    PIPE,
    HERDOC,
    S_QUOTE,
    D_QUOTE,
    FLAG,
    NON,

}   t_type;

typedef struct s_tokens
{
    char    *command;
    struct s_tokens *next;
}   t_tokens;

typedef struct s_ast
{
    t_type type;
    char *opperator;
    char *flag;
    char *cmd;
    char *arg;
    struct s_ast *next;
}   t_ast;

typedef struct s_env
{
    char *str;
    int var;
    struct s_env *next;
}   t_env;

typedef struct s_global
{
    char *line;
    t_tokens *tokens;
    struct s_global *next;
    
}   t_global;

int	push(char *env, t_env **begin_lst, int var);
int init_env(t_env **env_list);
void	ft_clean_envlist(t_env **env);
int	check_quotes(char	*str);
int parssing(char *str);
t_tokens *ft_split_tokens(char *str);

#endif