#include <unistd.h>
#include "minishell.h"
 
int main(int ac, char **av, char **env)
{

  char *arg0 = "/bin/echo"; //path
  char *args[] = {"echo", "rtgfsfe","ghnm" ,NULL};

  struct stat buf;
  if (stat("txt.txt", &buf) == -1)
    perror("");

  // else
  {  
    // int file = open("txt.txt", O_WRONLY | O_APPEND); //O_APPEND//////////////////>>
    // dup2(file, STDOUT_FILENO);
    // close (file);
    execve(arg0, args, NULL);
  }
  return 0;
}

