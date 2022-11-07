#include "minishell.h"

void sig_handler(int var)
{
    if(var == SIGINT)
    {
        write(1, "minishell$> \n", 13);
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

int is_valid(char c)
{
    if(c == '|' || c == '<' || c == '>')
        return 1;
    else if(c == '<<' || c == '>>')
        return 2;
    return 0;
}

int is_blank(char c)
{
    if(c == ' ' || c == '\t')
        return 1;
    return 0;
}

int is_redir_in(char *str, int *i)
{
    int start = *i;

    while (ft_isdeghit(str[*i]))
        *i++;
    if(str[i] == '<')
    {
        *i++;
        return(1);
    }
    *i = start;
    return(0); 
    
}

t_type type(t_global *global, int *i)
{

    if(global->line[*i] == '|')
    {
        *i++;
        return(PIPE);
    }
    else if(is_redir_in(global->line[*i], &i))
        return(REDIR_IN);

    else if(global->line[*i] == )
    {
        *i++;
        return(REDIR_OUT);
    }
    else if(global->line[*i] == '\'')
    {
        *i++;
        return(S_QUOTE);
    }
    else if(global->line[*i] == '"')
    {
        *i++;
        return(D_QUOTE);
    }
    else if(global->line[*i] == )



}

t_tokens *add_token(t_global *global, int *i)
{
    int len = 0;

    skip_blank(global, i);
    len = is_squote(global, i);
    len = is_dquote(global, i);
    len = is_pipe(global, i);
    len = is_heredoc(global, i);
    len = is_redir_in(global, i);
    len = is_redir_out(global, i);
    len = is_append(global, i);
    len = is_flag
    len = is_cmd(global);
    
}

void tokenization(t_global *global)
{
    int i = 0;
    t_tokens *current; 

    global->tokens = add_token(global, &i);
    current = global->tokens;
    while(current)
    {
        current = current->next;
        current = add_token(global, &i);
    }
}

void init_global(t_global *global)
{
    global->line = NULL;
    global->cmd_status = 0;
}

int main(int ac, char **av)
{
    t_global global;

    init_global(&global);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, sig);

    while (1)
    {
        global->line = readline("minishell$> ");
        add_history(global->line);
        tokenization(global);
    }
    free(global->line);
    global = global->next;
    

    return(0);
}

