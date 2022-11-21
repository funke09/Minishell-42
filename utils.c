#include "minishell.h"

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

void		ft_free_arr(char **arr)
{
	int		i;

	if (arr)
	{
		i = 0;
		while (arr[i] != NULL)
		{
			ft_strdel(&arr[i]);
			i++;
		}
		if (arr)
			free(arr);
		arr = NULL;
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

char *get_path(t_env **envirement) // key=value
{
    char *tmp;
    int len;
    t_env *env;

    env = NULL;
    if (envirement)
        env = *envirement;
    while (env)
    {
        len = len_key(env->str);
        if(ft_strncmp(env->str, "PATH", len) == 0)
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
    return (NULL);
}

char			*get_bin_file(char **cmd, t_env **env)
{
	t_get_bin	v;

	ft_bzero(&v, sizeof(t_get_bin));
	if (!(v.env_path_value = get_path(env)))
    {
        printf("chi7ajaa %s\n", v.env_path_value);
		return (NULL);}
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
