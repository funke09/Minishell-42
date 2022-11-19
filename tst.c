#include <unistd.h>
#include "minishell.h"
 
int main(int ac, char **av, char **env)
{

  int ret = 2;
  // char *arg0 = "/home/fzara/Desktop/tttttt"; //path
  // char *args[] = {"tttt",NULL};

  // // struct stat buf;
  // // if (stat("txt.txt", &buf) == -1)
  // //   perror("");

  // // else
  // // {  
  //   // int file = open("txt.txt", O_WRONLY | O_APPEND); //O_APPEND//////////////////>>
  //   // dup2(file, STDOUT_FILENO);
  //   // close (file);
  //   if (execve(arg0, args, NULL) == -1)
  //     perror("errrrrrrrrrr\n");
  // // }
printf("%s\n", getcwd(NULL, 0));

  ret = chdir("../..");
  printf("%s\n", getcwd(NULL, 0));
  printf("%d\n", ret);
  return 0;
}


