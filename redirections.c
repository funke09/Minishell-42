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

void redir_here_doc(t_tokens *txt, char *tty_name)
{
    int pip[2];
    int fd;

    if ((fd = open(tty_name, O_RDWR)) == -1) // 5
		return ;
	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	if ((pipe(pip)) == -1)
		return ;
	if (txt->here_doc_txt)
		ft_putstr_fd(txt->here_doc_txt, pip[1]);
	close(pip[1]);
	dup2(pip[0], STDIN_FILENO);
	close(pip[0]);
}

void    execute_redirection(t_tokens *red)
{
    char *tty_name;

    tty_name = ttyname(0);
    // printf("ttyname=%s\n", ttyname(0));
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
		else if (red->token && red->type == HEREDOC)
			redir_here_doc(red, tty_name);
		red = go_to_redir(red->next);
    }
}