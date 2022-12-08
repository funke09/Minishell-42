/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigal_here.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flazerak <flazerak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 05:37:13 by flazerak          #+#    #+#             */
/*   Updated: 2022/12/08 06:27:57 by flazerak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_hd(int i)
{
	(void)i;
	rl_replace_line("", 0);
	ioctl(0, TIOCSTI, "\4");
}

void	is_here(t_tokens *tmp, t_global *global)
{
	if (!tmp->next || tmp->next->type == HEREDOC
		|| tmp->next->type == REDIR_IN || tmp->next->type == REDIR_OUT
		|| tmp->next->type == APPEND || tmp->next->type == PIPE)
		global->errnum = ERROR_HEREDOC;
	else if (tmp->next->type == HERDOC_KEY)
	{
		tmp->here_doc_txt = go_to_herdoc(tmp->next);
		signal(SIGINT, sig_handler);
	}
}
