#include"minishell.h"


t_tokens *go_to_redir(t_tokens *head)
{
    while (head)
    {
        if (head->type == PIPE)
            break ;
        if(head->type == REDIR_IN || head->type == REDIR_OUT || head->type == APPEND || head->type == HEREDOC)
            return(head);
        head = head->next;
    }
    return (NULL);
}
void redirect_in_out(t_tokens *token)
{
    int fd;

    fd = 0;
    if(token->type == REDIR_OUT)
    {
        fd = open(token->next->token, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if(fd > 0)
            dup2(fd, STDOUT_FILENO);
    }
    else if (token->type == REDIR_IN)
    {
        fd = open(token->next->token, O_RDONLY);
        if(fd > 0)
            dup2(fd, STDIN_FILENO);
    }
    else if (token->type == APPEND)
    {
        fd = open(token->next->token, O_CREAT | O_WRONLY | O_APPEND, 0644);
        if(fd > 0)
            dup2(fd, STDOUT_FILENO);
    }
    if (fd < 0)
        return ;
    close(fd);
}

void    execute_redirection(t_tokens *red)
{
	while (red != NULL)
	{
		// OUT IN *
		if ((red->token && (red->type == REDIR_IN || red->type == REDIR_OUT ||
            red->type == APPEND)))
			redirect_in_out(red);
		// APPENF *
		// else if (redirections->token && !ft_strcmp(redirections->token, ">>"))
		// 	append_redir(redirections);

		// HERE DOC *
		// else if (redirections->token && !ft_strcmp(redirections->token, "<<"))
		// 	 here_document(redirections, g_tty_name);
		red = go_to_redir(red->next);
    }
}