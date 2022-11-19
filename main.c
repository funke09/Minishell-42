/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:33 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/19 16:17:52 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

void sig_handler(int var)
{
    if(var == SIGINT)
    {
        signal(SIGQUIT, SIG_IGN);
        write(1, "\nminishell$> ", 13);
    
    }
}

int is_charachter(char c)
{
    if(c == '<' || c == '|' || c == '>' || c == ';' || c == '+' || c == '$' || c == '\'' || c == '"') 
        return(1);
    return(0);   
}

void syntax_error(void)
{
    write(2, "minishell: syntax error\n", 25);
    return;
}



int is_blank(char c)
{
    if(c == ' ' || c == '\t')
        return 1;
    return 0;
}

int is_redir(t_global *global, int *i, char c)
{
    int start = *i;

    if(global->line[*i] == c)
    {
        (*i)++;
        return(1);
    }
    *i = start;
    return(0);
}
int is_heredoc_or_append(t_global *global, int *i, char c)
{
    int start = *i;
    if(global->line[*i] == c && global->line[*i + 1] == c)
    {
        if(c == '<')
            global->heredoc_activ = 1;
        *i += 2;
        return(1);
    }
    *i = start;
    return(0);
}

int is_heredoc_key(t_global *global, int *i)
{
    int start = *i;

    while(global->line[*i] && global->heredoc_activ == 1 && !is_charachter(global->line[*i]))
    {  
            (*i)++;
        if(is_blank(global->line[*i]) || !global->line[*i])
        {
            global->heredoc_activ = 0;
            return(1);
        }
    }
    *i = start;
   return 0;
}

int is_flag(t_global *global, int *i)
{
    int start = *i;
    if(global->line[*i] == '-'  && !is_blank(global->line[*i + 1]) && ft_isalpha(global->line[*i + 1]))
    {
        (*i)++;
        while (ft_isalpha(global->line[*i]))
            (*i)++;
        return(1);
    }
    *i = start;
    return(0);
}

int is_quote(t_global *global, int *i, char c)
{

    int start = *i;
    if(global->line[*i] == c)
    {
        (*i)++;
        while (global->line[*i] && global->line[*i] != c)
            (*i)++;
        if(global->line[*i] == '\0')
            global->errnum = ERROR_QUOTE;
        if(global->line[*i] == c)
        {
            (*i)++;
            return(1);
        }
    } 
    *i = start;
    return(0);
}

int is_command(t_global *global, int *i)
{
    int start = *i;
    while(global->cmd_status == 0 && ft_isalnum(global->line[*i]))
    {
        (*i)++;
        if(is_blank(global->line[*i]) || global->line[*i] == '\0'  || is_charachter(global->line[*i]))
        {
            global->cmd_status = 1;
            return(1);
        }
    }
    *i = start;
    return(0);
}


int is_param(t_global *global, int *i)
{
    int start = *i;
    while (global->cmd_status == 1 && !is_blank(global->line[*i]) && global->line[*i])
    {
        (*i)++;
        if(is_blank(global->line[*i]) || !global->line[*i] || is_charachter(global->line[*i]))//iscaracter ??
            return(1);
    }
    *i = start;
    return(0);
}
int is_dolar(t_global *global, int *i)
{
    int start = *i;
    if(global->line[*i] && global->line[*i] == '$' && global->line[*i + 1] && global->line[*i + 1])
    {
        (*i)++;
        while(global->line[*i] && !is_blank(global->line[*i]) && !is_charachter(global->line[*i]))
            (*i)++;
        return(1);    
    }
    *i = start;
    return(0);
}

t_type type(t_global *global, int *i)
{
   if(is_heredoc_key(global, i))
        return(HERDOC_KEY);
    else if(is_quote(global, i, '\''))
            return(S_QUOTE);
    else if(is_quote(global, i, '\"'))
        return(D_QUOTE);
    else if(global->line[*i] == '|')
    {
        (*i)++;
        global->cmd_status = 0;
        return(PIPE);
    }
    else if(is_dolar(global, i))
        return(ENV_VAR);
    else if(is_heredoc_or_append(global, i, '>'))
        return(APPEND);
    else if(is_heredoc_or_append(global, i, '<'))
        return(HEREDOC);
    else if(is_redir(global, i, '<'))
        return(REDIR_IN);
    else if(is_flag(global, i))
        return(FLAG);
    else if(is_redir(global, i, '>'))
        return(REDIR_OUT);
    else if(is_command(global, i))
        return(COMMAND);
    else if(is_param(global, i))
        return(PARAM);
    return(NON);
}
void skip_blanks(t_global *global, int *i)
{
    while (is_blank(global->line[*i]))
        (*i)++;
}

void    print_type(t_type type)
{
    if(type == PIPE)
        printf("PIPE\n");
    else if(type == APPEND)
        printf("APPEND\n");
    else if(type == HEREDOC)
        printf("HEREDOC\n");
    else if(type == HERDOC_KEY)
        printf("HERDOC_KEY\n");
    else if(type == REDIR_IN)
        printf("REDIR_IN\n");
    else if(type == REDIR_OUT)
        printf("REDIR_OUT\n");
    else if(type == FLAG)
        printf("FLAG\n");
    else if(type == S_QUOTE)
        printf("S_QUOTE\n");
    else if(type == D_QUOTE)
        printf("D_QUOTE\n");
    else if (type == COMMAND)
        printf("COMMAND\n");
    else if(type == PARAM)
        printf("PARAM\n");
    else if(type == NON)
        printf("NON\n");
    else if(type == ENV_VAR)
        printf("ENV_VAR\n");
}

void print_global(t_global *global)
{
    printf("line :%s\n", global->line);
    printf("cmd status :%d\n", global->cmd_status);
    printf("heredoc :%d\n", global->heredoc_activ);
}


t_tokens *add_token(t_global *global, int *i)
{
    int len = 0;
    int start = 0;
    t_tokens *new;

    skip_blanks(global, i);
    start = *i;
    new = (t_tokens *)malloc(sizeof(t_tokens));//!malloc

    new->type = type(global, i);
    if(new->type == NON)
    {
        free(new);
        global->cmd_status = 0;
        global->heredoc_activ = 0;
        return(NULL);
    }
    len = *i - start;
    new->token = ft_substr(global->line, start, len);
    new->next = NULL;
    // print_global(global);
    print_type(new->type);
    return(new);
}

void print_tokens(t_global *global)
{
    t_tokens *token;

    token = global->tokens;
    if(!token)
        return;
    while(token)
    {
        printf("token: %s, type: %d\n ", token->token, token->type);
        token = token->next;
    }
}

void tokenization(t_global *global)
{
    int i = 0;
    t_tokens *current; 

    global->tokens = add_token(global, &i);
    current = global->tokens;
    while(current)
    {
        // write(1, "tokenization\n", 13);
        // printf("%s\n", current->token);
        // print_tokens(global);
        current->next = add_token(global, &i);
        current = current->next;
    }
}


void init_global(t_global *global)
{
    global->line = NULL;
    global->cmd_status = 0;
    global->fd[0] = -1;
    global->fd[1] = -1;
    global->tokens = NULL;
    global->heredoc_activ = 0;
    global->errnum = 0;
}


void ft_print_env(void)
{
	t_env	*env_list;

	env_list = *get_adress();
	while (env_list)
	{
		ft_putstr_fd(env_list->str, 1);
		ft_putchar_fd('\n', 1);
		if (!env_list->next)
			break;
		env_list = env_list->next;
	}
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int line_is_empty(char *line)
{
    int i = -1;

    while (line[++i])
    {
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
            return (0);
    }
    return (1);
}

int main(int ac, char **av, char **env)
{
    t_global global;
    // int check = 0;
    
    stock_env(env,  &global);
    // ft_print_env();
    init_global(&global);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, sig_handler);
    if(!ac && !av)
		return(0);
    while (1)
    {
        global.line = readline("minishell$> ");
        add_history(global.line);
        // signal(SIGQUIT, SIG_IGN);
        // signal(SIGINT, sig_handler);
        if(global.line == NULL)
        {
            write(1, "exit\n", 6);
            exit(1);
        }
        if( ft_strcmp(global.line, "exit") == 0)
        {
            write(1, "exit\n", 6);
            free(global.line);
            //+free it all
            exit(1);
        }
        if( ft_strcmp(global.line, "env") == 0)
        {
            ft_print_env();
            free(global.line);
            continue;
    
        }
        // if(ft_strcmp(global.line, "\n") == 0)// no need too cz we shouldnt handel pipeing
        // {
        //     // free(global.line);
        //     continue;
        // }
        // global.line = check_dolar(&global, global.env);
        tokenization(&global);
        // printf("tokenz\n");
        check_tokens(&global);
        // printf("check tokenz\n");
        if(global.errnum != 0)
            printferror(&global);
        // else 
        // {
        //     if (!line_is_empty(global.line) && !check)// no need to handel this cz if we have just spaces or tabs in our line those not mean the line is empty (try bash)
        //         add_history(global.line);
        // }
        print_tokens(&global);
        free(global.line);
        free_tokens(global.tokens);
        global.line = NULL;
    }
    return(0);
}
