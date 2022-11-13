#include "minishell.h"

char *ft_strtrim_quotes(char *str)
{
    int i;
    int j;
    char *new_str;

    i = 0;
    j = 0;
    new_str = malloc(sizeof(char) * ft_strlen(str));
    if(new_str == NULL)
        return (NULL);
    while(str[i] != '\0')
    {
        if(str[i] != '\'' && str[i] != '\"')
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    new_str[j] = '\0';
    return (new_str);
}

t_ast *ast(t_tokens *tokens) // trying to aplemen the tree and give each token his childs its just test im not sure 
{
    t_ast *ast;
    t_tokens *tmp;

    ast = NULL;
    tmp = tokens;
    while (tmp)
    {
        if (tmp->type == COMMAND)
        {
            if (ast == NULL)
            {
                ast = (t_ast *)malloc(sizeof(t_ast));
                ast->type = COMMAND;
                ast->opperator = NULL;
                ast->flag = NULL;
                ast->cmd = ft_strdup(tmp->token);
                ast->arg = NULL;
                ast->next = NULL;
            }
            else
            {
                ast->next = (t_ast *)malloc(sizeof(t_ast));
                ast->next->type = COMMAND;
                ast->next->opperator = NULL;
                ast->next->flag = NULL;
                ast->next->cmd = ft_strdup(tmp->token);
                ast->next->arg = NULL;
                ast->next->next = NULL;
                ast = ast->next;
            }
        }
        if (tmp->type == PIPE)
        {
            ast->next = (t_ast *)malloc(sizeof(t_ast));
            ast->next->type = PIPE;
            ast->next->opperator = ft_strdup(tmp->token);
            ast->next->flag = NULL;
            ast->next->cmd = NULL;
            ast->next->arg = NULL;
            ast->next->next = NULL;
            ast = ast->next;
        }
        if (tmp->type == REDIR_IN)
        {
            ast->next = (t_ast *)malloc(sizeof(t_ast));
            ast->next->type = REDIR_IN;
            ast->next->opperator = ft_strdup(tmp->token);
            ast->next->flag = NULL;
            ast->next->cmd = NULL;
            ast->next->arg = NULL;
            ast->next->next = NULL;
            ast = ast->next;
        }
        if (tmp->type == REDIR_OUT)
        {
            ast->next = (t_ast *)malloc(sizeof(t_ast));
            ast->next->type = REDIR_OUT;
            ast->next->opperator = ft_strdup(tmp->token);
            ast->next->flag = NULL;
            ast->next->cmd = NULL;
            ast->next->arg = NULL;
            ast->next->next = NULL;
            ast = ast->next;
        }
        if (tmp->type == APPEND)
        {
            ast->next = (t_ast *)malloc(sizeof(t_ast));
            ast->next->type = APPEND;
            ast->next->opperator = ft_strdup(tmp->token);
            ast->next->flag = NULL;
            ast->next->cmd = NULL;
            ast->next->arg = NULL;
            ast->next->next = NULL;
            ast = ast->next;
        }
        if (tmp->type == FLAG)
        {
            ast->next = (t_ast *)malloc(sizeof(t_ast));
            ast->next->type = FLAG;
            ast->next->opperator = NULL;
            ast->next->flag = ft_strdup(tmp->token);
            ast->next->cmd = NULL;
            ast->next->arg = NULL;
            ast->next->next = NULL;
            ast = ast->next;
        }
        if (tmp->type == PARAM)
        {
            ast->next = (t_ast *)malloc(sizeof(t_ast));
            ast->next->type = PARAM;
            ast->next->opperator = NULL;
            ast->next->flag = NULL;
            ast->next->cmd = NULL;
            ast->next->arg = ft_strdup(tmp->token);
            ast->next->next = NULL;
            ast = ast->next;
        }
        if(tmp->type == D_QUOTE)
        {
            ast->next = (t_ast *)malloc(sizeof(t_ast));
            ast->next->type = D_QUOTE;
            ast->next->opperator = NULL;
            ast->next->flag = NULL;
            ast->next->cmd = NULL;
            ast->next->arg = ft_strtrim_quotes(tmp->token);
            ast->next->next = NULL;
            ast = ast->next;
        }
        if(tmp->type == S_QUOTE)
        {
            ast->next = (t_ast *)malloc(sizeof(t_ast));
            ast->next->type = S_QUOTE;
            ast->next->opperator = NULL;
            ast->next->flag = NULL;
            ast->next->cmd = NULL;
            ast->next->arg = ft_strtrim_quotes(tmp->token);
            ast->next->next = NULL;
            ast = ast->next;
        }
        tmp = tmp->next;
    }
    return (ast);
}
