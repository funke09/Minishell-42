#include "minishell.h"


t_tokens *ft_split_tokens(char *str)
{
    t_tokens *tokens;
    t_tokens *head;
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;
    tokens = malloc(sizeof(t_tokens));
    head = tokens;
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t')
        {
            tokens->token = ft_substr(str, j, i - j);
            j = i + 1;
            tokens->next = malloc(sizeof(t_tokens));
            tokens = tokens->next;
        }
        i++;
    }
    tokens->token = ft_substr(str, j, i - j);
    tokens->next = NULL;
    return (head);
}



