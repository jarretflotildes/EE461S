#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

int main(){
	

   printf("Hello World\n");
   char *command = readline("$");

   printf("%s\n",command);
   free(command);
   
/*
   while(1){
  
      // 0. Register Signal Handlers

      // 1. Print prompt
      //printf(">");
      


      // 2. Grab and parse input - remove newLine modifier (\n)  
      //each token will be no more than 30 characters
  
      //3. Check for job control Tokens
      


      // 4. Determine number of children processes to create (# times to call fork() ) 
         

      // 5. execute commands using execvo or execlp
      

      // 6. Other commands for job stuff

   }	  
*/
}

