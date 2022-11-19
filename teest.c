/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:22 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/19 22:40:03 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_strlen_character(const char *s, char c)
{
    size_t	i;

    i = 0;
    if(!s)
        return(0);
    while (s[i] && s[i] != c)
        i++;
    return (i);
}

char *ft_getenv(char *str)
{
	t_env	*env_list;
    char    *path;
    size_t     len;

	env_list = *get_adress();
	while (env_list)
	{
        len = ft_strlen_character(env_list->str, '=');
        printf("len %zu\n", len);
        if (!ft_strncmp(str, env_list->str, len))
        {
            path = ft_substr(env_list->str, len, ft_strlen(env_list->str) - len);
            return(path);
        }
		if (!env_list->next)
			break;
		env_list = env_list->next;
	}
    return (NULL);
}

char *expantion(char *token)
{
    char  *str;
    char  *var;
    char  *tmp;
    int len;

    printf("this is the token=%s\n", token);
    
    str = token;

    len = ft_strlen_char(str + 1);
    printf("len = %d\n" , len);

    if(str[1] == '?')
    {
       return NULL;
        
    }
    else
    {
        var = ft_substr(str,  1, len);
        
        // printf("var = %s len = %d\n" , var, len);
        tmp = ft_getenv(var);
        // printf("tmp = %s\n" , tmp);
        free(var);
        if(!tmp)
            return(NULL);  
        tmp = ft_strchr(tmp, '=') + 1;
        tmp = ft_substr(tmp, 0, ft_strlen(tmp));
        // printf("tmp = %s\n", tmp);
    }
    return(tmp);
}
