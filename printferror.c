/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printferror.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:14 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/18 02:05:13 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen_char(const char *s, char c)
{
    size_t	i;

    i = 0;
    if(!s)
        return(0);
    while (s[i] && s[i] != c)
        i++;
    return (i);
}


char *generate_dolar(t_global *global, char *tokens)
{
    int  i = 0;
    int  j = 0;
    char *str;
    char res[4096] = {0};
    char *tmp;
    char *val;
    
    int len;
    
    str = tokens;
    if(str[0] == '\"')
        i++;
    // res = malloc(sizeof(char) * 4096);
    while (str[i])
    {
        if (str[i] == '\"')
            break ;
        if(str[i] == '$')
        {
            len = ft_strlen_char(str + i + 1, ' ');
            val = expantion(global, str + i);
            printf("val = %s\n", val);
            tmp = ft_strjoin(res, val);
            printf("tmp = %s\n", tmp);
            if(!tmp)
                return (NULL);
            i += len;
            printf("len = %d\n", len);
            j = 0;
            
        }
        else
            res[j++] = str[i];
        i++;
    }
    res[j] = '\0';
    printf("res = %s\n", res);
    return(tmp);
    
}

int  go_to_herdoc(t_global *global, t_tokens *tokens)
{
    char *str;
    char txt[100];
    
    if(global->fd[0] != -1)
        close(global->fd[0]);
    
    
    if(pipe(global->fd) < 0)
        return(42);
    while(1)
    {
        str = readline("> ");
        if(!str || !ft_strcmp(str, tokens->token))
            break;
        write(global->fd[1], str, ft_strlen(str));
        free(str);
    }
    close(global->fd[1]);
    int n;
    if((n = read(global->fd[0], txt ,10)) < 0)
        return(42);
    txt[n] = 0;
    printf("%s\n", txt);
    return(0);
}

t_type	check_quote(t_tokens *tokens)
{
	int		i;
	t_type	quote;
    char	*str;

	i = 0;
    str = tokens->token;
	quote = NON;
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (quote == NON)
				quote = D_QUOTE;
			else if (quote == D_QUOTE)
				quote = NON;
		}
		if (str[i] == '\'')
		{
			if (quote == NON)
				quote = S_QUOTE;
			else if (quote == S_QUOTE)
				quote = NON;
		}
		i++;
	}
	return (quote);
}


//free_tokens
void free_tokens(t_tokens *tokens)
{
    t_tokens *tmp;

    while(tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->token);
        free(tmp);
    }
}

void printferror(t_global *global)
{
    t_error error;

    error = global->errnum;


    if(error == ERROR_QUOTE)
        printf("ERROR_QUOTE\n");
    if(error == ERROR_PIPE)
        printf("ERROR_PIPE\n");
    if(error == ERROR_REDIR)
        printf("ERROR_REDIR\n");
    if(error == ERROR_HEREDOC)
        printf("ERROR_HEREDOC\n");
    if(error == ERROR_APPEND)
        printf("ERROR_APPEND\n");
    if(error == ERROR_PARAM)
        printf("ERROR_PARAM\n");
    if(error == ERROR_COMMAND)
        printf("ERROR_COMMAND\n");
    if(error == ENV_NOT_FOUND)
        printf("ENV_NOT_FOUND\n");
    

//free if there is something to be freed// not sure if it's the right place// sometimes it's not working exactly when we have a quote in our line but not closed (heap use after free)
    if(global->tokens)
    {
        free_tokens(global->tokens);
        global->tokens = NULL;
    } 
    global->errnum = 0;
}

// function that check tokens and return if there is an error
void check_tokens(t_global *global)
{
    t_tokens *tmp;
    char *temp;
    tmp = global->tokens;
    int start = 1;
    // printf("tmp = %d, token = %d", tmp->type , global->tokens->type);
    while(tmp)
    {
        if(tmp->type == ENV_VAR)
        {
            temp = tmp->token;
            tmp->token = expantion(global, tmp->token);
            if (!tmp->token)
                global->errnum = ENV_NOT_FOUND;
            printf("tmp->token = %s\n", tmp->token);
            free(temp);
        }
        else if(tmp->type == PIPE)
        {
            if(start == 1 || !tmp->next || tmp->next->type == PIPE)
                global->errnum = ERROR_PIPE;
        }
        else if(tmp->type == REDIR_IN)
        {
            if(!tmp->next || tmp->next->type == REDIR_IN || tmp->next->type == REDIR_OUT || tmp->next->type == APPEND || tmp->next->type == HEREDOC || tmp->next->type == PIPE)
                global->errnum = ERROR_REDIR;
        }
        else if(tmp->type == REDIR_OUT)
        {
            if(!tmp->next || tmp->next->type == REDIR_OUT || tmp->next->type == HEREDOC || tmp->next->type == APPEND || tmp->next->type == REDIR_IN || tmp->next->type == PIPE)// redir out if the next is pipe and filename its working normally but if the next is just pipe schould be a syntax error 
                global->errnum = ERROR_REDIR;
        }
        else if(tmp->type == APPEND)
        {
            if(!tmp->next || tmp->next->type == APPEND || tmp->next->type == REDIR_OUT || tmp->next->type == REDIR_IN || tmp->next->type == PIPE || tmp->next->type == HEREDOC)
                global->errnum = ERROR_APPEND;
        }
        else if(tmp->type == HEREDOC)
        {
            if(!tmp->next || tmp->next->type == HEREDOC || tmp->next->type == REDIR_IN || tmp->next->type == REDIR_OUT || tmp->next->type == APPEND || tmp->next->type == PIPE) // problem in quote (missunderstanding the problem check bash)
                global->errnum = ERROR_HEREDOC;
            else if(tmp->next->type == HERDOC_KEY)
            {
                if(go_to_herdoc(global, tmp->next) == 42)
                        global->errnum = ERROR_HEREDOC;
            }
        }
        else if(tmp->type == D_QUOTE)
        {
            temp = tmp->token;
            tmp->token = generate_dolar(global, tmp->token);
            printf("tmp->token = %s\n", tmp->token);
            free(temp);
        }
 
        tmp = tmp->next;
        start = 0;
    }
}
// function that print syntax error and free what must be freed and return 0
