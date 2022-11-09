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
        write(1, "\nminishell$> ", 13);
        return;
    }
}


int is_charachter(char c)
{
    if(c == '\\' || c == '&' || c == ')' || c == '('  || c == ';' || c == '+' || c == '*') 
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

    while (ft_isdigit(global->line[*i]))
        (*i)++;
    if(global->line[*i] == c)
    {
        (*i)++;
        while (ft_isdigit(global->line[*i]))
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

    while(global->line[*i] && global->heredoc_activ == 1)
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
        if(global->line[*i] == c)
        {
            (*i)++;
            return(1);
        }
    }
    *i = start;
    return(0);
}

// int	check_quotes(t_global *global, int *i)
// {
//     int start = *i;
// 	t_type	quote;

// 	quote = NON;
// 	while (global->line[*i])
// 	{
// 		if (global->line[*i] == '"')
// 		{
// 			if (quote == NON)
// 				quote = D_QUOTE;
// 			else if (quote == D_QUOTE)
// 				return (D_QUOTE);
// 		}
// 		if (global->line[*i] == '\'')
// 		{
// 			if (quote == NON)
// 				quote = S_QUOTE;
// 			else if (quote == S_QUOTE)
// 				return(S_QUOTE);
// 		}
// 		(*i)++;
// 	}
//     *i = start;
// 	return (quote);
// }
int is_command(t_global *global, int *i)
{
    int start = *i;
    while(global->cmd_status == 0 && ft_isalnum(global->line[*i]))
    {
        (*i)++;
        if(is_blank(global->line[*i]) || global->line[*i] == '\0')
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
        if(is_blank(global->line[*i]) || !global->line[*i] || is_charachter(global->line[*i]))
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
    new = (t_tokens *)malloc(sizeof(t_tokens));
    new->type = type(global, i);
    if(new->type == NON)
    {
        free(new);
        global->cmd_status = 0;
        return(NULL);
    }
    len = *i - start;
    new->token = ft_substr(global->line, start, len);
    if(!new->token)
        return(NULL);
    new->next = NULL;
    // print_global(global);
    print_type(new->type);
    return(new);
}
void print_tokens(t_global *global)
{
    t_tokens *token;

    token = global->tokens;
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
    global->tokens = NULL;
    global->heredoc_activ = 0;
}

int main(int ac, char **av)
{
    t_global global;

    init_global(&global);
    signal(SIGQUIT, sig_handler);
    signal(SIGINT, SIG_IGN);
    if(!ac && !av)
		return(0);
    while (1)
    {
        global.line = readline("minishell$> ");
        add_history(global.line);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, sig_handler);
        if(global.line == NULL)
        {
            write(1, "exit\n", 6);
            exit(1);
        }
        tokenization(&global);
        print_tokens(&global);
    }
    // free(global.line);
    return(0);
}

