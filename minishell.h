/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:27 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/26 18:56:44 by macos            ###   ########.fr       */
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
#include <stdbool.h>

// int g_glb.g_var;

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
    char    *token; // << EOF
    t_type  type; // HERE
    char    *here_doc_txt; //  data
    struct s_tokens *next;
}   t_tokens; 

typedef struct s_var
{
    int exit_status;
    int g_var;
}   t_var;

t_var g_glb;

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
    char                *tty_num;
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
void check_tokens(t_global *global, t_var *g_glb);
char *ft_strtrim_quotes(char *str);
int	len_key(char *env);
char *expantion(char *token);
void sig_handler(int var);
char *ft_getenv(char *str, int len);
size_t	ft_strlen_char(const char *s);
void free_tokens(t_tokens *tokens);
void ft_print_env(void);
int ft_sizearray(char **args);
int check_arg_valid(char *str, int *l);
t_env *check_var_exist(t_env **env, char *arg, int len);


int             execute(t_global *global, t_var *g_glb);
char *get_path(t_env **envirement, char *name);
char			*get_bin_file(char **cmd, t_env **env);

int do_builtin(char **args, t_env **env);
int is_a_builtin(char *cmd);
int ft_unset(t_env **env, char **args);
int pwd(void);
int ft_export(t_env **env,char **args);
int ft_env(char **args);
int echo(char **args);
int  c_cd(char **args, t_env **env);
int ft_exit(char **args, t_env **env);
char *ft_strdup2(char *str);
int	push(char *env, t_env **begin_lst);
char		**list_to_tabs(t_env **env_list);
int				len_path(char **cmd);
int				if_exist(t_env **env_list, char *var_name);
void			oldpwd(char *cwd, t_env **env_list);
char			*get_cwd(void);

//REDIRECTION
t_tokens* go_to_redir(t_tokens *token);
void    execute_redirection(t_tokens *red);
int is_a_builtin_child(char *cmd);
void	ft_strdel(char **as);
#endif

