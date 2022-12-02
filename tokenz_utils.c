#include "minishell.h"

int	is_flag(t_global *global, int *i, int *no_space)
{
	int	start;

	start = *i;
	if (global->line[*i] == '-' && !is_blank(global->line[*i + 1])
		&& ft_isalpha(global->line[*i + 1]))
	{
		(*i)++;
		while (ft_isalpha(global->line[*i]))
			(*i)++;
		if (!is_blank(global->line[*i]) && global->line[*i] != '|'
				&& global->line[*i] != '<' && global->line[*i] != '>')
				*no_space = 1;
		return (1);
	}
	*i = start;
	return (0);
}

int	is_quote(t_global *global, int	*i, char c, int *no_space)
{
	int	start;

	start = *i;
	if (global->line[*i] == c)
	{
		(*i)++;
		while (global->line[*i] && global->line[*i] != c)
			(*i)++;
		if (global->line[*i] == '\0')
			global->errnum = ERROR_QUOTE;
		if (global->line[*i] == c)
		{
			(*i)++;
			if (global->line[*i] && !is_blank(global->line[*i])
				&& global->line[*i] != '|' && global->line[*i] != '<'
				&& global->line[*i] != '>')
				*no_space = 1;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_command(t_global *global, int *i, int *no_space)
{
	int	start;

	start = *i;
	while (global->line[*i] && global->is_redir == 0 && global->cmd_status == 0
		&& !is_blank(global->line[*i]))
	{
		(*i)++;
		if (is_blank(global->line[*i]) || global->line[*i] == '\0'
			|| is_charachter(global->line[*i]))
		{
			if (!is_blank(global->line[*i]) && global->line[*i] != '|'
				&& global->line[*i] != '<' && global->line[*i] != '>')
				*no_space = 1;
			global->cmd_status = 1;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_param(t_global *global, int	*i, int *no_space)
{
	int	start;

	start = *i;
	while ((global->cmd_status == 1 || global->is_redir == 1)
		&& !is_blank(global->line[*i]) && global->line[*i])
	{
		(*i)++;
		if (is_blank(global->line[*i]) || !global->line[*i]
			|| is_charachter(global->line[*i]))
		{
			if (!is_blank(global->line[*i]) && global->line[*i] != '|'
				&& global->line[*i] != '<' && global->line[*i] != '>')
				*no_space = 1;
			global->is_redir = 0;
			return (1);
		}
	}
	*i = start;
	return (0);
}

int	is_dolar(t_global *global, int	*i, int *no_space)
{
	int	start;

	start = *i;
	if (global->line[*i] && global->line[*i] == '$'
		&& (ft_isalnum(global->line[*i + 1]) || global->line[*i + 1] == '?'))
	{
		(*i)++;
		while (global->line[*i] && !is_blank(global->line[*i])
			&& (ft_isalnum(global->line[*i])
				|| global->line[*i] == '?') && !is_charachter(global->line[*i]))
			(*i)++;
		if (!is_blank(global->line[*i]) && global->line[*i] != '|'
			&& global->line[*i] != '<' && global->line[*i] != '>')
			*no_space = 1;
		return (1);
	}
	*i = start;
	return (0);
}
