#include "minishell.h"

// the _= value will match the location of the env binary (e.g. /usr/bin/env)
// export | hang
// echo $? result not correct
//grep"ll" shouldn't work
// cd "".."" too many args!! prob f getting args

//exit : free global line!!
//expantion f export !!!!!
//leaks f echo a var !!!
// leaks f invalid commands
//leaks f crl c
// export expantion  
//
// SHLVL
//handling shlvl f minishell embarque
////grep"string"  ////
//echo aa"bb" tokenization should be one arg not 2
//also echo aa'n'
// echo "="="="

// builtin fork


void ft_remove(t_env **env, char *var_name)
{
    int l;
    t_env *curr;
    t_env *tmp;

    l = ft_strlen (var_name);
    if (!env || !*env)
        return;
    while (*env && l == len_key((*env)->str) && !ft_strncmp(var_name, (*env)->str, l))
    {
        tmp = (*env)->next;
        free(*env);
        *env = tmp;
    }
    curr = *env;
    while (curr && curr->next)
    {
        if (!ft_strncmp(var_name, curr->next->str, l) && l == len_key(curr->next->str))
        {
            tmp = curr->next;
            curr->next = curr->next->next;
            free(tmp);
        }
        else 
            curr = curr->next;
    }
}

int check_unset_arg_valid(char *str)
{
    int i;

    i = -1;
    if (ft_isdigit(str[0]))
        return 1;
    while (str[++i])
    {
        if (!ft_isdigit(str[i]) && !ft_isalpha(str[i]) && str[i] != '_')
            return (1);
    }
    return (0);
}

int ft_unset(t_env **env, char **args)
{
    int i = 1;

    while(args[i])
    {
        if (check_unset_arg_valid(args[i]))
        {
            ft_putstr_fd("minishell: unset: ", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd(" : not a valid identifier\n", 2);
            return (1);
        }
        ft_remove(env, args[i]);
        i++;
    }  

    return (0);
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
    // printf("ppppp\n");
	env_new->str = ft_strdup2(env);
	if (env_new->str == NULL)
		return (42);
	env_new->next = *begin_lst;
	*begin_lst = env_new;
	return (0);
}



int check_arg_valid(char *str, int *l)
{
    int i;

    i = -1;
    if (ft_isdigit(str[0]) || str[0] == '=')
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
        push2(arg, env);
    }
}

void	ft_putstr2(char *s)
{	
	int	i;
    int ch;

	i = 0;
    ch = 1;
	if (!s || !s[i])
		return ;
    if (s[i] == '_'  && s[i + 1] == '=')
        return;
    ft_putstr_fd("declare -x ", 1);
	while (s[i])
	{
        ft_putchar_fd(s[i], 1);
        if (s[i] == '=' && ch)
        {
            ch = 0;
            ft_putchar_fd('"', 1);
        }
		
		i++;
	}
    ft_putchar_fd('"', 1);
}


void ft_print_export(t_env **env)
{
    t_env *env_list;

    if(!env[0])
        return;
    env_list = *env;
    while (env)
	{
		ft_putstr2(env_list->str);
		ft_putchar_fd('\n', 1);
		if (!env_list->next)
			break;
		env_list = env_list->next;
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
        ft_print_export(env);
        return 0;
    }
    while (args[i])
    {
        if (!check_arg_valid(args[i], &len))
        {
            if (!args[i][len] || !(args[i][len + 1]))
                return (0);
            ft_handle_arg(env, args[i], len);
        }
        else
        {
            ft_putstr_fd("minishell: export: ", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd(" : not a valid identifier\n", 2);
            return (1);
        }
        i++;
    }
    return 0;
}



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
