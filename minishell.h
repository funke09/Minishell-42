/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: funke <funke@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:27 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/17 23:23:02 by funke            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

typedef enum s_error
{
    SUCSSES,
    ERROR_QUOTE,
    ERROR_PIPE,
    ERROR_COMMAND,
    ERROR_REDIR,
    ERROR_HEREDOC,
    ERROR_APPEND,
    ERROR_PARAM,
    ENV_NOT_FOUND,
}   t_error;

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
    ENV_VAR,
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
    t_error errnum;
    t_env **env;
    int fd[2];
}   t_global;

int stock_env(char **env, t_global *global);
t_env	**get_adress(void);
void printferror(t_global *global);
void check_tokens(t_global *global);
void print_ast(t_ast *ast);
char *ft_strtrim_quotes(char *str);
t_ast *ast(t_global *tokens);
int	len_key(char *env);
int expantion(t_global *global, char *token);
int
#endif