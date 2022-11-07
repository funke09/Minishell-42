#include "minishell.h"

void signals_hnd(int num)
{
	if(num == SIGINT)
        write(1,"\nmyshell$> ", 11);
		return;
}

t_env	**get_adress(void)
{
	static t_env	*new = NULL;

	return (&new);
}

t_tokens *get_tokens(void)
{
    t_tokens *tokens = NULL;
    return (tokens);
}

//function that will check if the command is a built in or not
int check_built_in(char *str)
{
    if(ft_strncmp(str, "echo", 4) == 0)
        return (1);
    else if(ft_strncmp(str, "cd", 2) == 0)
        return (1);
    else if(ft_strncmp(str, "pwd", 3) == 0)
        return (1);
    else if(ft_strncmp(str, "export", 6) == 0)
        return (1);
    else if(ft_strncmp(str, "unset", 5) == 0)
        return (1);
    else if(ft_strncmp(str, "env", 3) == 0)
        return (1);
    else if(ft_strncmp(str, "exit", 4) == 0)
        return (1);
    else if(ft_strncmp(str, "<<", 2) == 0)
        return (1);
    return (0);
}
int check_characters(char *str)
{
    int i = 0;

    while(str[i])
    {
        if (str[i] == '&' || str[i] == ';' || str[i] == '(' || str[i] == ')' || str[i] == '\\')
        {
            printf("minishell: syntax error near unexpected token `%c'\n", str[i]);
            return(1);
        }
        i++;
    }
    return(0);
}

int  check_syntax(char *str)
{
	int	i;
	int	dif;
    t_type    quote;

	i = 0;
	dif = 0;
    quote = check_quotes(str);
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '|')
			dif = 1;
		if (str[i] == '|')
		{
			if (dif == 0)
				return (1);
			dif = 0;
			if(str[i + 1] == '\0' && dif == 0)
				return (1);
		}
        // printf("%d\n", check_built_in(str));
        if((check_built_in(str) == 0))
            return (1);
        if(check_characters(str) == 1 || quote != NON)
        {
            write(2, "minishell : missing quote\n", 26);
            return (1);
        }
		i++;
	}
	return (0);
}


int stock_env(char **env)
{

	t_env	**env_list;
	int		i;

	env_list = get_adress();
	i = 0;
	while (env[i])
		i++;
	if (i == 0)
		if (init_env(env_list) == 42)
			return (42);
	i--;
	while (i >= 0)
	{
		if (push(env[i], env_list, 0) == 42)
		{
			ft_clean_envlist(env_list);
			return (42);
		}
		i--;
	}
	return (0);
}

char *arg_double_quote(char *str)
{
    int i;
    int j;
    char *new;

    i = 0;
    j = 0;
    new = malloc(sizeof(char) * (ft_strlen(str) + 1));
    while(str[i])
    {
        if(str[i] == '\"')
        {
            i++;
            while(str[i] != '\"')
            {
                new[j] = str[i];
                i++;
                j++;
            }
        }
        i++;
    }
    new[j] = '\0';
    return (new);
}
void ft_print_env(void)
{
	t_env	*env_list;

	env_list = *get_adress();
	while (env_list)
	{
		ft_putstr_fd(env_list->str, 1);
		ft_putchar_fd('\n', 1);
		if (!env_list->next)
			break;
		env_list = env_list->next;
	}
}
void ft_print_token(t_tokens *tokens)
{
    while(tokens)
    {
        printf("%s\n", tokens->command);
        tokens = tokens->next;
    }
}
int main(int ac, char **av, char **env)
{
    
    t_global *global;

    global = (t_global *) malloc(sizeof(t_global));
    stock_env(env);
    // ft_print_env();
    if(!ac && !av)
		return(0);
    signal(SIGINT, signals_hnd);
	signal(SIGQUIT, SIG_IGN);
    while(1)
    {
        global->line = readline("minishell$> ");
        add_history(global->line);
        if(global->line == NULL)
        {
            write(1, "exit\n", 6);
            exit(1);
        }
        if (check_syntax(global->line))
        {
            // printf("%d\n", check_syntax(global->line));
            printf("syntax error: near unexpected token '%s' or unvalide commande \n", global->line);
        }
        global->tokens = ft_split_tokens(global->line);
        ft_print_token(global->tokens);
        free(global->line);
        global->next = NULL;
    }
    return(0);
}