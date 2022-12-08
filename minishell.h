/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:27 by zcherrad          #+#    #+#             */
/*   Updated: 2022/12/08 06:25:59 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <sys/ioctl.h>

# define BLUMG "\e[1;35m"
# define END "\e[0m"
# define RED "\e[31m"
# define RESET "\e[0m"

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
}	t_error;

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
}	t_type;

typedef struct s_tokens
{
	char			*token;
	t_type			type;
	int				no_space;
	char			*here_doc_txt;
	struct s_tokens	*next;
}	t_tokens;

typedef struct s_var
{
	int		exit_status;
	int		_status;
	char	*tty_name;
}	t_var;

t_var	g_glb;

typedef struct s_env
{
	char			*str;
	struct s_env	*next;
}	t_env;

typedef struct s_global
{
	char		*line;
	bool		cmd_status;
	bool		heredoc_activ;
	t_tokens	*tokens;
	t_error		errnum;
	t_env		**env;
	int			fd[2];
	bool		is_redir;
	bool		herdoc_stat;
}	t_global;

typedef struct s_pipe
{
	int					pipe[2];
	pid_t				pid;
	int					temp;
	int					cmd_no;
	char				*tty_num;
}	t_pipe;

typedef struct s_get_bin
{
	int					i;
	char				*bin_file;
	char				*env_path_value;
	char				**dirs;
	char				*tmp;
	char				*tmp2;
}	t_get_bin;

typedef struct s_for_expantion
{
	int		i;
	int		j;
	char	res[4096];
	char	*val;
	int		len;
}	t_for_expantion;

int			stock_env(char **env, t_global *global);
t_env		**get_adress(void);
void		printferror(t_global *global);
void		check_tokens(t_global *global);
int			len_key(char *env);
char		*expantion(char *token);
void		sig_handler(int var);
char		*ft_getenv(char *str, int len);
size_t		ft_strlen_char(const char *s);
void		free_tokens(t_tokens *tokens);
void		ft_print_env(void);
int			ft_sizearray(char **args);
int			check_arg_valid(char *str, int *l);
t_env		*check_var_exist(t_env **env, char *arg, int len);
int			execute(t_global *global);
char		*get_path(t_env **envirement, char *name);
char		*get_bin_file(char **cmd, t_env **env);
int			do_builtin(char **args, t_env **env);
int			is_a_builtin(char **cmd);
int			ft_unset(t_env **env, char **args);
int			pwd(void);
int			ft_export(t_env **env, char **args);
void		ft_print_export(t_env **env);
int			ft_env(char **args);
int			echo(char **args);
int			c_cd(char **args, t_env **env);
int			ft_exit(char **args, t_env **env);
char		*ft_strdup2(char *str);
int			push(char *env, t_env **begin_lst);
char		**list_to_tabs(t_env **env_list);
int			len_path(char **cmd);
int			if_exist(t_env **env_list, char *var_name);
void		oldpwd(char *cwd, t_env **env_list);
char		*get_cwd(void);
t_tokens	*go_to_redir(t_tokens *token);
void		execute_redirection(t_tokens *red);
int			is_a_builtin_child(char *cmd);
void		ft_strdel(char **as);
void		ft_free(char **splt, int i);
void		tokenization(t_global *global);
int			is_dolar(t_global *global, int	*i, int *no_space);
int			is_param(t_global *global, int	*i, int *no_space);
int			is_command(t_global *global, int *i, int *no_space);
int			is_quote(t_global *global, int	*i, char c, int *no_space);
int			is_flag(t_global *global, int *i, int *no_space);
int			is_heredoc_key(t_global *global, int	*i);
int			is_heredoc_or_append(t_global *global, int	*i, char c);
int			is_redir(t_global *global, int	*i, char c);
int			is_blank(char c);
int			is_charachter(char c);
char		*go_to_herdoc(t_tokens *tokens);
int			is_a_builtin_child(char *cmd);
int			check_arg(int *i, char *str);
int			len_of_cmd_sin_pipes(t_tokens *token);
t_tokens	*join_expantion(t_tokens *token, char **cmd, int *i);
int			is_valid_type(t_tokens *token);
char		**get_cmd(t_tokens *token);
t_tokens	*skip_to_pipe(t_tokens *token);
int			is_valid_cmd(const char *cmd);
void		ft_execve(const char *file_name, char **cmd, char **env);
void		execute_direct(char **cmd, char **env);
void		execute_undirect(char **cmd, char **tabs, t_env **env);
int			ft_sizearray(char **args);
int			is_there_pipe(t_tokens *head);
int			push2(char *env, t_env **begin_lst);
char		*ft_strdup2(char *str);
int			check_unset_arg_valid(char *str);
void		ft_remove(t_env **env, char *var_name);
char		*generate_dolar(char *str);
char		*inside_quote(char *tokens);
void		ft_handle_arg(t_env **env, char *arg, int len);
void		ft_free_arr(char **env);
void		ft_wait(int *status);
t_tokens	*skip_to_pipe(t_tokens *token);
void		sig_hd(int i);
void		is_here(t_tokens *tmp, t_global *global);
#endif
