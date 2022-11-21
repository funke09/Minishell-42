// #include <unistd.h>
// #include "minishell.h"
 
// int main(int ac, char **av, char **env)
// {

//   int ret = 2;
//   // char *arg0 = "/usr/bin/ls"; //path
//   char *args[] = {"/usr/bin/ls","-la",NULL};

//   struct stat buf;
//   if (stat("txt.txt", &buf) == -1)
//     perror("");

//   else
//   {  
//     int file = open("txt.txt", O_WRONLY ); //O_APPEND//////////////////>>
//     dup2(file, STDOUT_FILENO);
//     close (file);

//     printf("%s\n", getcwd(NULL, 0));
//     // if (execve(args[0], args, NULL) == -1)
//     //   perror("errrrrrrrrrr\n");
  
//   }

// //   ret = chdir("../..");
// //   printf("%s\n", getcwd(NULL, 0));
// //   printf("%d\n", ret);
//   return 0;
// }


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc > 1) {
        struct stat sb;
        stat(argv[1], &sb)
        printf("%o", argv[1],  == 0 &&  "" : " not");
    }
    return 0;
}   