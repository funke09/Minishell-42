#include "minishell.h"

void	ft_remove2(t_env **env, char *var_name)
{
	int		l;
	t_env	*curr;
	t_env	*tmp;

	l = ft_strlen (var_name);
	if (!env || !*env)
		return ;
	curr = *env;
	while (curr && curr->next)
	{
		if (!ft_strncmp(var_name, curr->next->str, l)
			&& l == len_key(curr->next->str))
		{
			tmp = curr->next;
			curr->next = curr->next->next;
			free(tmp);
		}
		else
			curr = curr->next;
	}
}

void	ft_remove(t_env **env, char *var_name)
{
	int		l;
	t_env	*tmp;

	l = ft_strlen (var_name);
	if (!env || !*env)
		return ;
	while (*env && l == len_key((*env)->str)
		&& !ft_strncmp(var_name, (*env)->str, l))
	{
		tmp = (*env)->next;
		free(*env);
		*env = tmp;
	}
	ft_remove2(env, var_name);
}

int	check_unset_arg_valid(char *str)
{
	int	i;

	i = -1;
	if (ft_isdigit(str[0]))
		return (1);
	while (str[++i])
	{
		if (!ft_isdigit(str[i]) && !ft_isalpha(str[i]) && str[i] != '_')
			return (1);
	}
	return (0);
}

char	*ft_strdup2(char *str)
{
	char	*ret;
	int		i;
	int		j;

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
	env_new->str = ft_strdup2(env);
	if (env_new->str == NULL)
		return (42);
	env_new->next = *begin_lst;
	*begin_lst = env_new;
	return (0);
}
