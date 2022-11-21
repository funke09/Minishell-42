/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:27 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/21 07:52:47 by macos            ###   ########.fr       */
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
#include <sys/types.h>
#include <sys/stat.h>
#include<stdbool.h>
int g_var;
char *g_tty_name;

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
    struct s_env *next;
}   t_env;


typedef struct s_global
{
    char *line;
    bool cmd_status;
    bool heredoc_activ;
    t_tokens *tokens;
    t_error errnum;
    t_env **env;
    int fd[2];
    bool is_redir;
    bool herdoc_stat;
}   t_global;

typedef struct			s_pipe
{
	int					pipe[2];
	pid_t				pid;
	int					temp;
	int					cmd_no;
}						t_pipe;

typedef struct			s_get_bin
{
	int					i;
	char				*bin_file;
	char				*env_path_value;
	char				**dirs;
	char				*tmp;
	char				*tmp2;
}						t_get_bin;

int stock_env(char **env, t_global *global);
t_env	**get_adress(void);
void printferror(t_global *global);
void check_tokens(t_global *global);
char *ft_strtrim_quotes(char *str);
int	len_key(char *env);
char *expantion(char *token);
void sig_handler(int var);
char *ft_getenv(char *str, int len);
size_t	ft_strlen_char(const char *s);
void free_tokens(t_tokens *tokens);


int             execute(t_global *global);
char			*get_bin_file(char **cmd, t_env **env);


void	ft_strdel(char **as);
#endif