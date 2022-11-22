#include "minishell.h"

char		**list_to_tabs(t_env **env_list)
{
	char	**tabs;
	// char	*str;
	t_env	*current;
	int		i;

	if (!(current = *env_list))
		return (NULL);
	tabs = NULL;
	i = -1;
	while (++i >= 0 && current != NULL)
		current = current->next;
	if (!(tabs = (char **)malloc(sizeof(char *) * (i + 1))))
		return (NULL);
	current = *env_list;
	i = 0;
	while (current != NULL)
	{
		if (current->str)
			tabs[i++] = ft_strdup(current->str);
		current = current->next;
	}
	tabs[i] = NULL;
	return (tabs);
}

void	ft_memdel(void **ap)
{
	if (ap)
	{
		free(*ap);
		*ap = NULL;
	}
}

void	ft_strdel(char **as)
{
	ft_memdel((void **)as);
}

void		ft_free_arr(char **env)
{
	int		i;

	if (env)
	{
		i = 0;
		while (env[i] != NULL)
		{
			ft_strdel(&env[i]);
			i++;
		}
		if (env)
			free(env);
		env = NULL;
	}
}

char		*ft_freejoin(char *s1, char *s2, int num)
{
	char	*str;

	if (!(str = ft_strjoin(s1, s2)))
		return (NULL);
	if (num == 0)
		free(s1);
	else if (num == 1)
		free(s2);
	else if (num == 2)
	{
		free(s1);
		free(s2);
	}
	return (str);
}

char *get_path(t_env **envirement, char *name) // key=value
{
    char *tmp;
    int len;
    t_env *env;

    env = NULL;
    if (envirement)
        env = *envirement;
	if(name && env)
	{

		while (env)
		{
			len = len_key(env->str);
			if(ft_strncmp(env->str, name, len) == 0)
			{
				tmp = ft_strchr(env->str, '=');
				if (tmp)
				{
					tmp = ft_substr(tmp + 1, 0, ft_strlen(env->str) - len);
					
					return (tmp);
				}
			}
			env = env->next;
		}
	}
    return (NULL);
}

char			*get_bin_file(char **cmd, t_env **env)//////////////////////////////////////////////////////////////////////////////////////
{
	t_get_bin	v;

	ft_bzero(&v, sizeof(t_get_bin));
	if (!(v.env_path_value = get_path(env, "PATH")))
		return (NULL);
	if (!(v.dirs = ft_split(v.env_path_value, ':')))
		return (NULL);
	ft_strdel(&v.env_path_value);
	v.i = -1;
	while (v.dirs[++v.i] != NULL)
	{
		if (!(v.tmp = ft_strdup(v.dirs[v.i])))
			return (NULL);
		v.tmp2 = ft_freejoin(v.tmp, "/", 0);
		if (!(v.bin_file = ft_strjoin(v.tmp2, cmd[0])))
			return (NULL);
		ft_strdel(&v.tmp2);
		if (access(v.bin_file, F_OK) == 0)
			break ;
		ft_strdel(&v.bin_file);
	}
	ft_free_arr(v.dirs);
	return (v.bin_file);
}
