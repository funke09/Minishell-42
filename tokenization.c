#include "minishell.h"


int is_blank(char c)
{
    if(c == ' ' || c == '\t')
        return 1;
    return 0;
}

int is_redir(char *str, int *i, char c)
{
    int start = *i;

    while (ft_isdeghit(str[*i]))
        *i++;
    if(str[*i] == c)
    {
        *i++;
        while (ft_isdeghit(str[*i]))
            *i++;
        return(1);
    }
    *i = start;
    return(0);
}
int is_heredoc_or_append(char *str, int *i, char c)
{
    int start = *i;
    if(str[*i] == c && str[*i + 1] == c)
    {
        *i += 2;
        return(1);
    }
    *i = start;
    return(0);
}

int	check_quotes(char	*str, int *i)
{
    int start = *i;
	t_type	quote;

	quote = NON;
	while (str[*i])
	{
		if (str[*i] == '"')
		{
			if (quote == NON)
				quote = D_QUOTE;
			else if (quote == D_QUOTE)
				quote = NON;
		}
		if (str[*i] == '\'')
		{
			if (quote == NON)
				quote = S_QUOTE;
			else if (quote == S_QUOTE)
				quote = NON;
		}
		*i++;
	}
    start = *i;
	return (quote);
}

t_type type(t_global *global, int *i)
{
    if(global->line[*i] == '|')
    {
        *i++;
        return(PIPE);
    }
    else if(is_heredoc_or_append(global->line[*i], &i, '>'))
        return(APPEND);
    else if(is_heredoc_or_append(global->line[*i], &i, '<'))
        return(HEREDOC);
    else if(is_redir(global->line[*i], &i, '<'))
        return(REDIR_IN);
    else if(is_redir(global->line[*i], &i, '>'))
        return(REDIR_OUT);
    else if(check_quotes(global->line[*i], &i) == S_QUOTE)
        return(S_QUOTE);
    else if(check_quotes(global->line[*i], &i) == D_QUOTE)
        return(D_QUOTE);
}

void init_type(t_global *global, int *i)
{
    t_tokens *new;
    t_tokens *tmp;

    new = malloc(sizeof(t_tokens));
    new->token = NULL;
    new->type = type(global, i);
    new->next = NULL;
    if(global->tokens == NULL)
        global->tokens = new;
    else
    {
        tmp = global->tokens;
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}


int get_len(t_global *global, int *i)
{
    int len = 0;
    int start = *i;
    while (global->line[*i] && global->line[*i] != start)
    {
        len++;
        *i++;
    }
    return(len);
    // int address = &(global->line[*i]);
    // printf("%d", (int)ft_strchr(global->line[*i + 1], "\"") - address);
}
char get_str(t_global *global, int *i)
{
    int len = get_len(global, &i);
    char *str = malloc(sizeof(char) * (len + 1));
    int j = 0;
    while (j < len)
    {
        str[j] = global->line[*i];
        *i++;
        j++;
    }
    str[j] = '\0';
    return(str);
}

t_tokens *add_token(t_global *global, int *i)
{
    int len = 0;
    t_tokens *new;

    skip_blank(global, i);
    init_type(global, i);
    if(global->tokens->type == S_QUOTE || global->tokens->type == D_QUOTE)
    {
        len = get_len(global, i);
        new = (t_tokens *)malloc(sizeof(t_tokens));
        new->token = (char *)malloc(sizeof(char) * (len + 1));
        new->type = global->tokens->type;
        new->next = NULL;
        // new->prev = NULL;
        new->token = get_str(global, i);
        return(new);
    }
    else if(global->tokens->type == PIPE || global->tokens->type == REDIR_IN || global->tokens->type == REDIR_OUT || global->tokens->type == APPEND || global->tokens->type == HEREDOC)
    {
        new = (t_tokens *)malloc(sizeof(t_tokens));
        new->token = (char *)malloc(sizeof(char) * 2);
        new->type = global->tokens->type;
        new->next = NULL;
        // new->prev = NULL;
        new->token = get_str(global, i);
        return(new);
    }
    else
    {
        len = get_len(global, i);
        new = (t_tokens *)malloc(sizeof(t_tokens));
        new->token = (char *)malloc(sizeof(char) * (len + 1));
        new->type = global->tokens->type;
        new->next = NULL;
        // new->prev = NULL;
        new->token = get_str(global, i);
        return(new);
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
        current = current->next;
        current = add_token(global, &i);
    }
}

void init_global(t_global *global)
{
    global->line = NULL;
    global->cmd_status = 0;
}
