#include "minishell.h"

void ft_remove(t_env **env, char *var_name)
{
    int l;
    t_env *curr;
    t_env *tmp;

    l = ft_strlen (var_name);
    if (!env || !*env)
        return;
    while (*env && !ft_strncmp(var_name, (*env)->str, l))
    {
        tmp = (*env)->next;
        free(*env);
        *env = tmp;
    }
    curr = *env;
    while (curr && curr->next)
    {
        if (!ft_strncmp(var_name, curr->next->str, l))
        {
            tmp = curr->next;
            curr->next = curr->next->next;
            free(tmp);
        }
        else 
            curr = curr->next;
    }
}



int ft_unset(t_env **env, char **args)
{
    int i = -1;

    while(args[++i])
    {
        // check_valid args : only numbers, underscores, and digits .==
        // if (valid_env_var(args[i]), &l)
        // {
        //     ft_putendl_fd("Not a valid unset identifier.\n", 2);
        //     return (1);
        // }
        ft_remove(env, args[i]);
        return (0);
    }  

    return (0);
}

int check_var_exist_replace(t_env **env, char *arg, int len)
{
    // int i;
    t_env *envir;
    char *tmp;

    // i = -1;
    envir = *env;
    while (envir)
    {
        if (!strncmp(envir->str, arg, len))
        {
                printf("AAAAAAAAAAA  %c  AAAAAAAAAAAA\n", arg[len - 1]);////
            if (arg[len - 1] == '+')
            {
                tmp = ft_strjoin(envir->str, (ft_strrchr(arg, '=') + 1));
                free(envir->str);
                envir->str = tmp;
            }
            else
            {
                free(envir->str);
                envir->str = ft_strdup(arg);
                return (0);
            }
        }
        envir = envir->next;
    }
    return (1);
}



char *ft_strdup2(char *str)
{
    char *ret;
    int i;
    int j;

    i = 0;
    j = 0;
    ret = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
    while (str[j])
    {
        if (str[j] == '+')
            j++;
        ret[i++] = str[j++];
    }
    ret[i] = '\0';
    return (ret);
}

int	push2(char *env, t_env **begin_lst)
{
	t_env	*env_new;

	env_new = malloc(sizeof(t_env));
	if (env_new == NULL)
		return (42);
    printf("ppppp\n");
	env_new->str = ft_strdup2(env);
	if (env_new->str == NULL)
		return (42);
	env_new->next = *begin_lst;
	*begin_lst = env_new;
	return (0);
}

// int  export_no_args()
// {

// }

// int ft_export(t_env **env,char **args)
// {
//     int i;
//     int len;
//     int ch;


//     i = 0;
//     // if (!args[1])
//     //     export_no_args();
//     while (args[++i])
//     {
//         if (valid_env_var(args[i]))/////to check
//         {
//             ft_putendl_fd("Not a valid export identifier.\n", 2);
//             return (1);
//         }
//         len = len_key(args[1]);
//         if (args[i][len] == '=')
//         {
//             ch = check_var_exist_replace(env, args[i], len);
//             if (!ch)
//             {
//                 printf("AAA2222\n");////
//                 push2(args[i], env);
//                 return (1);
//             }
//         }

//     }
//     return (0);

// }

int check_arg_valid(char *str, int *l)
{
    int i;

    i = -1;
    if (ft_isdigit(str[0]))
        return 1;
    while (str[++i] && str[i] != '=')
    {
        if (str[i] == '+' && str[i+1] == '=')
            break ;
        if (!ft_isdigit(str[i]) && !ft_isalpha(str[i]) && str[i] != '_')
            return (1);
    }
    *l = i;
    return (0);
}

t_env *check_var_exist(t_env **env, char *arg, int len)
{
    t_env *en;

    en = *env;
    // printf(" cmp = %d\n", ft_strncmp("kjhzskjadksj", arg, len));
    write(1, arg, len);
    printf("+++++++++++\n");
    while (en)
    {
        if (!ft_strncmp(en->str, arg, len))
            return (en);
        en = en->next;
    }
    return (NULL);
}


void    ft_handle_arg(t_env **env, char *arg, int len)
{
    int plus;
    t_env *exist;
    char *value;

    plus = (arg[len] == '+');
    exist = check_var_exist(env, arg, len);
    if (exist && plus)
    {
        value = ft_strjoin(exist->str, &arg[len + 1 + plus]);
        printf("{%s}\n", value);
        free(exist->str);
        exist->str = value;
    }
    else if (exist)
    {
        free(exist->str);
        exist->str = ft_strdup(arg);
    }
    else
    {
        push(arg, env);
    }
}


int ft_export(t_env **env, char **args)
{
    int i;
    int len;

    i = 1;
    len = -1;
    if (!args[1])
    {
        ft_print_env();
        return 0;
    }
    while (args[i])
    {
        // printf("loop%s\n", args[i]);
        if (!check_arg_valid(args[i], &len))
            ft_handle_arg(env, args[i], len);
        else
        {
            ft_putendl_fd("", 2);
            return (1);
        }
        i++;
    }
    ft_print_env();
    return 0;
}

// if value start with a $

int ft_env(char **args)
{
    int l;

    l = ft_sizearray(args);
    if (l > 1)
    {
        ft_putendl_fd("Too many arguments.\n", 2);
    }
    else
        ft_print_env();
    return (0);
}
