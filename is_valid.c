#include "minishell.h"

int	check_quotes(char	*str)
{
	int		i;
	t_type	quote;

	i = 0;
	quote = NON;
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (quote == NON)
				quote = D_QUOTE;
			else if (quote == D_QUOTE)
				quote = NON;
		}
		if (str[i] == '\'')
		{
			if (quote == NON)
				quote = S_QUOTE;
			else if (quote == S_QUOTE)
				quote = NON;
		}
		i++;
	}
	return (quote);
}