/*
 *
 *  
 * 
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

static void sig_int(int signo) {
   printf("caught SIGINT\n");
}

int main(){
	
   printf("Hello World\n");
   char *command;    
   pid_t pid = -1;

   int ifd[2];
   int ofd[2];
   int pipefd[2];
 
   // 0. Register Signal Handlers
   /*Signals (SIGINT, SIGTSTP, SIGCHLD)
	   ○ Ctrl-c must quit current foreground process (if one exists) and not the shell
	   and should not print the process (unlike bash)
	   ○ Ctrl-z must send SIGTSTP to the current foreground process and should not
	   print the process (unlike bash)
	   ○ The shell will not be stopped on SIGTSTP

   if (signal(SIGINT, sig_int) == SIG_ERR){
      printf("signal(SIGINT) error");
   }
*/

   while(1){

       // 1. Print prompt
      command = readline("# "); 

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char *tokens[64] = {NULL};
      char *token;
      char *savePtr;
      char *commandCopy = strdup(command);

      int i = 0;
      tokens [i] = strtok_r(commandCopy," ",&savePtr);
      while((token = strtok_r(NULL," ", &savePtr))){
          i++; 
          tokens[i] = strcat(token,"\0");
      }  
      
      free(command);
      //3. Check for job control Tokens

      //4. Determine number of children processes to create (# times to call fork() ) 
      
     
      // 5. execute commands using execvo or execlp
      
      int tokensSize = sizeof(tokens)/sizeof(tokens[0]);
//printf("Token size is %d",tokensSize);
      pid = fork();
      if(pid == 0){
         if(tokensSize == 1){
            execlp(tokens[0],tokens[0],(char *)NULL);
	 }
	 exit(0);
      } 
     
      // 6. Other commands for job stuff

   }

}


