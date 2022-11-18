/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 00:18:22 by zcherrad          #+#    #+#             */
/*   Updated: 2022/11/18 01:34:06 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_getenv(t_env *env, char *str)
{
    while(env)
    {
        //  printf("env =%s%%\n", env->str);
        int len =  len_key(env->str);
        // printf("len = %d\n", len);
        if(!ft_strncmp(str, env->str, len))
            return(env->str);
        env = env->next;
    }
    return(NULL);
}

char *expantion(t_global *global, char *token)
{
    char  *str;
    char  *var;
    char  *tmp;
    int len;

    
    str = token;
    len = ft_strlen(str) - 1;


    if(str[1] == '?')
    {
       return NULL;
        
    }
    else
    {
        var = ft_substr(str,  1, len);
        // printf("var = %s\n" , var);
        tmp = ft_getenv(*(global->env), var);
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
