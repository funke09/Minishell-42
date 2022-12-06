#include "minishell.h"

int	check_arg_valid(char *str, int *l)
{
	int	i;

	i = -1;
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (1);
	while (str[++i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			break ;
		if (!ft_isdigit(str[i]) && !ft_isalpha(str[i]) && str[i] != '_')
			return (1);
	}
	*l = i;
	return (0);
}

t_env	*check_var_exist(t_env **env, char *arg, int len)
{
	t_env	*en;

	en = *env;
	while (en)
	{
		if (!ft_strncmp(en->str, arg, len))
			return (en);
		en = en->next;
	}
	return (NULL);
}

void	ft_handle_arg(t_env **env, char *arg, int len)
{
	int		plus;
	t_env	*exist;
	char	*value;

	plus = (arg[len] == '+');
	exist = check_var_exist(env, arg, len);
	if (exist && plus)
	{
		value = ft_strjoin(exist->str, &arg[len + 1 + plus]);
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
	int	ch;

	i = 0;
	ch = 1;
	if (!s || !s[i])
		return ;
	if (s[i] == '_' && s[i + 1] == '=')
		return ;
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
	if (ch == 0)
		ft_putchar_fd('"', 1);
}

void	ft_print_export(t_env **env)
{
	t_env	*env_list;

	if (!env[0])
		return ;
	env_list = *env;
	while (env)
	{
		ft_putstr2(env_list->str);
		ft_putchar_fd('\n', 1);
		if (!env_list->next)
			break ;
		env_list = env_list->next;
	}
}
