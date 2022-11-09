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



typedef enum s_type
{
    COMMAND,
    PIPE,
    NON,
    S_QUOTE,
    D_QUOTE,
    HEREDOC,
    REDIR_IN,
    REDIR_OUT,
    APPEND,
    FLAG,
    PARAM,
    HERDOC_KEY,
} t_type;


typedef struct s_tokens
{
    char *token;
    t_type type;
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
    int cmd_status;
    int heredoc_activ;
    t_tokens *tokens;
}   t_global;

int stock_env(char **env);
t_env	**get_adress(void);

#endif