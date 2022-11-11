#include "minishell.h"

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
    

//free if there is something to be freed
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
    // printf("tmp = %d, token = %d", tmp->type , global->tokens->type);
    while(tmp)
    {
        // write(1, "hello\n", 6);
        if(tmp->type == PIPE)
        {
            if(tmp->next == NULL)
            {
                global->errnum = ERROR_PIPE;
                printferror(global);
                return(0);
            }
            if(tmp->next->type == PIPE)
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
            if(tmp->next && tmp->next->type == REDIR_IN)
            {
                global->errnum = ERROR_REDIR;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == REDIR_OUT)
        {
            if(tmp->next && tmp->next->type == REDIR_OUT)
            {
                global->errnum = ERROR_REDIR;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == APPEND)
        {
            if(tmp->next && tmp->next->type == APPEND)
            {
                global->errnum = ERROR_APPEND;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == HEREDOC)
        {
            if(tmp->next && tmp->next->type == HEREDOC)
            {
                global->errnum = ERROR_HEREDOC;
                printferror(global);
                return(0);
            }
        }
        else if(tmp->type == PARAM)
        {
            if(tmp->next && tmp->next->type == PARAM)
            {
                global->errnum = ERROR_PARAM;
                printferror(global);
                return(0);
            }
        }
        tmp = tmp->next;
    }
    return(1);
}