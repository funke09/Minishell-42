/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:27 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/19 16:10:24 by macos            ###   ########.fr       */
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
char *ft_strtrim_quotes(char *str);
int	len_key(char *env);
char *expantion(char *token);
void sig_handler(int var);
char *ft_getenv(char *str);
size_t	ft_strlen_char(const char *s);
void free_tokens(t_tokens *tokens);

#endif