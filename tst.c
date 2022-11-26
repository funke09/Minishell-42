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

typedef struct s_var
{
    int exit_status;
    int g_var;
}   t_var;

void func2(t_var *g_var)
{
	(*g_var).exit_status *= 20;
}


void func(t_var *g_var)
{
	func2(g_var);
}

int main(int argc, char **argv)
{
	t_var g_glb;

	g_glb.exit_status = 10;
	func(&g_glb);
	printf("%d\n", g_glb.exit_status);
    return 0;
}   