#include "minishell.h"

void	ft_clean_envlist(t_env **env)
{
	t_env	*cpy;
	t_env	*tmp;

	cpy = *env;
	tmp = cpy;
	while (cpy)
	{
		tmp = cpy;
		cpy = cpy->next;
		free(tmp->str);
		free(tmp);
	}
}

int	len_key(char *env)
{
	int	i;

	i = 0;
	while (env[i] != '\0' && env[i] != '=')
		i++;
	return (i);
}

char	*ft_add(char *env)
{
	int		len;
	char	*s;
	int		i;
	int		y;

	i = 0;
	y = 0;
	len = len_key(env) - 1;
	if (env[len_key(env) - 1] != '+')
		return (ft_strdup(env));
	s = malloc(sizeof(char) * ft_strlen(env));
	if (s == NULL)
		return (NULL);
	while (env[y])
	{
		s[i] = env[y];
		i++;
		y++;
		if (y == len)
			y++;
	}
	s[i] = '\0';
	return (s);
}

int	push(char *env, t_env **begin_lst, int var)
{
	t_env	*env_new;

	env_new = malloc(sizeof(t_env));
	if (env_new == NULL)
		return (42);
	env_new->str = ft_add(env);
	env_new->var = var;
	if (env_new->str == NULL)
		return (42);
	env_new->next = *begin_lst;
	*begin_lst = env_new;
	return (0);
}

int init_env(t_env **env_list)
{
    char *s;

    s = ft_strjoin("PWD=", getcwd(NULL, 0));
    if(s == NULL)
        return (42);
    if(push("SHLVL=1", env_list, 1) == 42)
    {
        ft_clean_envlist(env_list);
        return (42);
    }
    if(push(s, env_list, 1) == 42)
    {
        free(s);
        ft_clean_envlist(env_list);
        return (42);
    }
    free(s);
    if(push("OLDPWD", env_list, 1) == 42)
    {
        ft_clean_envlist(env_list);
        return (42);
    }
    return (0);
}

t_env	**get_adress(void)
{
	static t_env	*new = NULL;

	return (&new);
}

int stock_env(char **env)
{

	t_env	**env_list;
	int		i;

	env_list = get_adress();
	i = 0;
	while (env[i])
		i++;
	if (i == 0)
		if (init_env(env_list) == 42)
			return (42);
	i--;
	while (i >= 0)
	{
		if (push(env[i], env_list, 0) == 42)
		{
			ft_clean_envlist(env_list);
			return (42);
		}
		i--;
	}
	return (0);
}