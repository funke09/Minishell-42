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
    if(c == '\\' || c == '&' || c == ')' || c == '('  || c == ';' || c == '+' || c == '-' || c == '*') 
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
    if(is_heredoc_or_append(global, i, '<'))
    {
        if(is_blank(global->line[*i]))
            *i += 1;
         
        else if(global->line[*i] && !is_blank(global->line[*i]))
        {   
            (*i)++;
            return(1);
        }

    }
    start = *i;
   return 0;
}

int is_flag(t_global *global, int *i)
{
    int start = *i;
    if(global->line[*i] == '-' && ft_isalpha(global->line[*i + 1]))
    {
        (*i)++;
        return(1);
    }
    return(0);
}

int	check_quotes(t_global *global, int *i)
{
    int start = *i;
	t_type	quote;

	quote = NON;
	while (global->line[*i])
	{
		if (global->line[*i] == '"')
		{
			if (quote == NON)
				quote = D_QUOTE;
			else if (quote == D_QUOTE)
				quote = NON;
		}
		if (global->line[*i] == '\'')
		{
			if (quote == NON)
				quote = S_QUOTE;
			else if (quote == S_QUOTE)
				quote = NON;
		}
		(*i)++;
	}
    start = *i;
	return (quote);
}

t_type type(t_global *global, int *i)
{
    //loop over the line 

    if(ft_isalpha(global->line[*i]))
    {
        while(ft_isalpha(global->line[*i]) && global->line[*i] != ' ')
            (*i)++;
        
        return(COMMAND);
    }

    if(global->line[*i] == '|')
    {
        (*i)++;
        return(PIPE);
    }
    else if(is_heredoc_or_append(global, i, '>'))
        return(APPEND);
    else if(is_heredoc_or_append(global, i, '<'))
        return(HEREDOC);
    else if(global->heredoc_activ == 1 && is_heredoc_key(global, i))
    {
        global->heredoc_activ = 0;
        return(HERDOC_KEY);
    }
    else if(is_redir(global, i, '<'))
        return(REDIR_IN);
    else if(is_flag(global, i))
        return(FLAG);
    else if(is_redir(global, i, '>'))
        return(REDIR_OUT);
    else if(check_quotes(global, i) == S_QUOTE)
        return(S_QUOTE);
    else if(check_quotes(global, i) == D_QUOTE)
        return(D_QUOTE);
    return(NON);
}
void skip_blanks(t_global *global, int *i)
{
    while (is_blank(global->line[*i]))
        (*i)++;
}

t_tokens *add_token(t_global *global, int *i)
{
    int len = 0;
    int start = 0;
    t_tokens *new;

    // skip_blank(global, i);
    start = *i;
    new = (t_tokens *)malloc(sizeof(t_tokens));
    // if(!new)
    //     return(NULL);
    new->type = type(global, i);
    len = *i - start;
    new->token = ft_substr(global->line, start, len);
    if(!new->token)
        return(NULL);
    new->next = NULL;
    return(new);
}

void tokenization(t_global *global)
{
    int i = 0;
    t_tokens *current; 

    global->tokens = add_token(global, &i);
    current = global->tokens;
    while(current->next)
    {
        write(1, "tokenization\n", 13);
        current = current->next;
        current = add_token(global, &i);
        // if(current->next == NULL)
        //     break;
    }
}

void print_tokens(t_global *global)
{
    t_tokens *token;

    token = global->tokens;
    while(token)
    {
        printf("token: %s, type: %d\n, ", token->token, token->type);
        token = token->next;
    }
}

void init_global(t_global *global)
{
    global = (t_global *)malloc(sizeof(t_global));
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

