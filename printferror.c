/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printferror.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:14 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/16 16:50:10 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_type	check_quote(t_global *global)
{
	int		i;
	t_type	quote;
    char	*str;

	i = 0;
    str = global->tokens->token;
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
    

//free if there is something to be freed// not sure if it's the right place// sometimes it's not working exactly when we have a quote in our line but not closed (heap use after free)
    if(global->tokens)
    {
        free_tokens(global->tokens);
        global->tokens = NULL;
    }   
}

// function that check tokens and return if there is an error
int check_tokens(t_global *global)
{
    t_tokens *tmp;
    tmp = global->tokens;
    int start = 1;
    // printf("tmp = %d, token = %d", tmp->type , global->tokens->type);
    while(tmp)
    {
        if(tmp->type == PIPE)
        {
            if(start == 1 || !tmp->next || tmp->next->type == PIPE)
            {
                global->errnum = ERROR_PIPE;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == COMMAND)
        {
            if(tmp->next && tmp->next->type == COMMAND)
            {
                global->errnum = ERROR_COMMAND;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == REDIR_IN)
        {
            if(!tmp->next || tmp->next->type == REDIR_IN || tmp->next->type == REDIR_OUT || tmp->next->type == APPEND || tmp->next->type == HEREDOC || tmp->next->type == PIPE)
            {
                global->errnum = ERROR_REDIR;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == REDIR_OUT)
        {
            if(!tmp->next || tmp->next->type == REDIR_OUT || tmp->next->type == HEREDOC || tmp->next->type == APPEND || tmp->next->type == REDIR_IN || tmp->next->type == PIPE)// redir out if the next is pipe and filename its working normally but if the next is just pipe schould be a syntax error 
            {
                global->errnum = ERROR_REDIR;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == APPEND)
        {
            if(!tmp->next || tmp->next->type == APPEND || tmp->next->type == REDIR_OUT || tmp->next->type == REDIR_IN || tmp->next->type == PIPE || tmp->next->type == HEREDOC)
            {
                global->errnum = ERROR_APPEND;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == HEREDOC)
        {
            if(!tmp->next || tmp->next->type == HEREDOC || tmp->next->type == REDIR_IN || tmp->next->type == REDIR_OUT || tmp->next->type == APPEND || tmp->next->type == PIPE) // problem in quote (missunderstanding the problem check bash)
            {
                global->errnum = ERROR_HEREDOC;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == S_QUOTE || tmp->type == D_QUOTE)
        {
            if(check_quote(global) != NON)
            {
                global->errnum = ERROR_QUOTE;
                printferror(global);
                return(0);
            }
        }
        tmp = tmp->next;
        start = 0;
    }
    return(1);
}
// function that print syntax error and free what must be freed and return 0
