#include "minishell.h"

int	ft_strlen_key(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*ft_getenv(char *str, int len)
{
	t_env	*env_list;
	char	*path;
	int		lengh;

	env_list = *get_adress();
	while (env_list)
	{
		lengh = ft_strlen_key(env_list->str);
		if (len == lengh && !ft_strncmp(str, env_list->str, lengh))
		{
			path = ft_substr(env_list->str, len,
					ft_strlen(env_list->str) - len);
			return (path);
		}
		if (!env_list->next)
			break ;
		env_list = env_list->next;
	}
	return (NULL);
}

char	*_exit_status(char *tok, char *sub, int len)
{
	char	*status;

	status = ft_itoa(g_glb.exit_status);
	if (len > 2)
	{
		sub = ft_substr(tok, 2, (len - 1));
		status = ft_strjoin(status, sub);
		free(sub);
	}
	return (status);
}

char	*expantion(char *token)
{
	char	*str;
	char	*var;
	char	*tmp;
	int		len;

	str = token;
	var = NULL;
	len = ft_strlen_char(str + 1);
	if (str[1] == '?')
		return (_exit_status(str, var, len));
	else
	{
		var = ft_substr(str, 1, len);
		tmp = ft_getenv(var, len);
		free(var);
		if (!tmp)
			return (NULL);
		tmp = ft_strchr(tmp, '=') + 1;
		tmp = ft_substr(tmp, 0, ft_strlen(tmp));
	}
	return (tmp);
}

int	is_there_pipe(t_tokens *head)
{
	while (head)
	{
		if (head->type == PIPE)
			return (1);
		head = head->next;
	}
	return (0);
}
