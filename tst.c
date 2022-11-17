#include <unistd.h>
#include "minishell.h"
 
int main(void) {
  char *programName = "ls";
  char *args[] = {"ls", "-la", "/home/fzara/Desktop/mmmmmmmmm", NULL};
  struct stat buf;
  if (stat("txt.txt", &buf) == -1)
    perror("error stat\n");
  // if () //check stat
  // {

  // }
  printf("%ld\n", buf.st_size);
  int file = open("txt.txt", O_WRONLY | O_APPEND); //O_APPEND//////////////////>>
  dup2(file, STDOUT_FILENO);
  close (file);
  execvp(programName, args);


  return 0;
}