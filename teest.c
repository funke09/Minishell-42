/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:22 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/30 02:31:39 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strlen_key(char *str)
{
    int i ;
    
    i = 0;
    if(!str)
        return(0);
    while (str[i] && str[i] != '=')
        i++;
    return(i);
}


char *ft_getenv(char *str, int len)
{
	t_env	*env_list;
    char    *path;
    int lengh;

	env_list = *get_adress();
	while (env_list)
	{
        lengh = ft_strlen_key(env_list->str);
        if (len == lengh && !ft_strncmp(str, env_list->str, lengh))
        {
            path = ft_substr(env_list->str, len, ft_strlen(env_list->str) - len);
            printf("Path=%s\n", path);
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
    char    *status;
    int len;
    
    str = token;
    len = ft_strlen_char(str + 1);
    if(str[1] == '?')
    {
        status = ft_itoa(g_glb.exit_status);
        if(len > 2)
        {   
            var = ft_substr(str, 2, (len - 1));
            status = ft_strjoin(status, var);
            free(var);
        }
       return (status);  
    }
    else
    {
        var = ft_substr(str,  1, len);
        tmp = ft_getenv(var, len);
        // printf("tmp = %s\n", tmp);
        free(var);
        if(!tmp)
            return(NULL);  
        tmp = ft_strchr(tmp, '=') + 1;
        tmp = ft_substr(tmp, 0, ft_strlen(tmp));
    }
    return(tmp);
}
