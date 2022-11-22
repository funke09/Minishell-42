/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printferror.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:14 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/22 03:05:24 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen_char(const char *s)
{
    size_t	i;

    i = 0;
    if(!s)
        return(0);
    while (s[i] && s[i] != '$' && s[i] != ' ' && s[i] != '\"')
        i++;
    return (i);
}

char *generate_dolar(t_global *global, char *tokens)
{
    int  i = 0;
    int  j = 0;
    char *str;
    char res[4096] = {0};
    char *val;
    (void)global;
    
    int len;
    
    str = tokens;
    if(str[0] == '\"')
        i++;
    while (str[i])
    {
        if (str[i] == '\"')
            break ;
        if(str[i] == '$' && str[i + 1] != ' ' && str[i + 1])
        {
            len = ft_strlen_char(str + i + 1);
            if (!(val = expantion(str + i)))
                    val = "";
            ft_strncpy(res + j, val, ft_strlen(val));
            i += len;
            j += ft_strlen(val);
        }
        else
            res[j++] = str[i];
        i++;
    }
    res[j] = '\0';
    return(ft_strdup(res));
    
}

int  go_to_herdoc(t_global *global, t_tokens *tokens)
{
    char *str;
    char txt[100];
    char    *tmp;
    
    if(global->fd[0] != -1)
        close(global->fd[0]);
    if(pipe(global->fd) < 0)
        return(42);
    while(1)
    {
        str = readline("> ");
        if(g_var)
        {
            g_var = 0;
            break;
        }
        if(!str || !ft_strcmp(str, tokens->token) || global->herdoc_stat)
            break;
        tmp = str;
        str = generate_dolar(global, str);
        free(tmp);
        write(global->fd[1], str, ft_strlen(str));
        // add_history(str); 
        free(str);
    }
    close(global->fd[1]);
    //***********************test:need to del
    int n;
    if((n = read(global->fd[0], txt ,100)) < 0)
        return(42);
    txt[n] = 0;
    printf("%s\n", txt);
    return(0);
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
char *inside_quote(char *tokens)
{
    int i;
    int j = 0;
    char *token = tokens;
    char *str;
    i = 0;
    str = (char *)malloc(sizeof(char) * ft_strlen(tokens));
    if(token[0] == '\'')
        i++;
    while (token[i])
    {
        if(token[i] == '\'')
            break;
        str[j++] = token[i++];
    }
    str[j] = '\0';
    return(str);
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
    if(global->tokens)
    {
        free_tokens(global->tokens);
        global->tokens = NULL;
    } 
    global->errnum = 0;
}


void check_tokens(t_global *global)
{
    t_tokens *tmp;
    char *temp;
    tmp = global->tokens;
    int start = 1;
    
    while(tmp)
    {
        if(tmp->type == ENV_VAR)
        {
            temp = tmp->token;
            tmp->token = generate_dolar(global, tmp->token);
            if (!tmp->token)
                global->errnum = ENV_NOT_FOUND;
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
            free(temp);
        }
        else if(tmp->type == S_QUOTE)
        {
            temp = tmp->token;
            tmp->token = inside_quote(tmp->token);
            free(temp);
        }
        tmp = tmp->next;
        start = 0;
    }
}
// function that print syntax error and free what must be freed and return 0
