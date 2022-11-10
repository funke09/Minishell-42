#include "minishell.h"

void printferror(t_global *global)
{
    t_error error;

    error = global->error;


    if(error == ERROR_QUOTE)
        printf("ERROR_QUOTE");
    if(error == ERROR_PIPE)
        printf("ERROR_PIPE");

//free if there is something to be free
    
}
