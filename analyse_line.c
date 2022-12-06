#include "minishell.h"

size_t	ft_strlen_char(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != '$' && s[i] != ' ' && s[i] != '\"'
		&& s[i] != '\'' && s[i] != '/')
		i++;
	return (i);
}

char	*generate_dolar(char *str)
{
	t_for_expantion	var;

	ft_bzero(&var, sizeof(t_for_expantion));
	if (str[0] == '\"')
		var.i++;
	while (str[var.i])
	{
		if (str[var.i] == '\"')
			break ;
		if (str[var.i] == '$' && str[var.i + 1] != ' ' && str[var.i + 1])
		{
			var.len = ft_strlen_char(str + var.i + 1);
			var.val = expantion((str + var.i));
			if (!var.val)
					var.val = "";
			ft_strncpy(var.res + var.j, var.val, ft_strlen(var.val));
			var.i += var.len;
			var.j += ft_strlen(var.val);
		}
		else
			var.res[var.j++] = str[var.i];
		var.i++;
	}
	var.res[var.j] = '\0';
	return (ft_strdup(var.res));
}

char	*go_to_herdoc(t_global *global, t_tokens *tokens)
{
	char	*str;
	char	*tmp;

	tokens->here_doc_txt = ft_strdup("");
	(void)global;
	while (1)
	{
		str = readline(RED"heredoc> "RESET);
		if (!str || !ft_strcmp(str, tokens->token))
			break ;
		tmp = str;
		str = generate_dolar(str);
		free(tmp);
		tmp = str;
		str = ft_strjoin(tokens->here_doc_txt, str);
		free(tmp);
		tmp = tokens->here_doc_txt;
		tokens->here_doc_txt = ft_strjoin(str, "\n");
		free(tmp);
		free(str);
	}
	if (!str)
		ft_strdel(&(tokens->here_doc_txt));
	return (tokens->here_doc_txt);
}

void	free_tokens(t_tokens *tokens)
{
	t_tokens	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->token);
		free(tmp);
	}
}

char	*inside_quote(char *tokens)
{
	int		i;
	int		j;
	char	*token;
	char	*str;

	i = 0;
	j = 0;
	token = tokens;
	str = (char *)malloc(sizeof(char) * ft_strlen(tokens));
	if (token[0] == '\'')
		i++;
	while (token[i])
	{
		if (token[i] == '\'')
			break ;
		str[j++] = token[i++];
	}
	str[j] = '\0';
	return (str);
}
