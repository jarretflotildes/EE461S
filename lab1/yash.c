/*
 *
 *
 *
 *
 *
 *
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

int main(){
	
   printf("Hello World\n");
   char *command;   
   pid_t pid = -1;

   while(1){
  
      // 0. Register Signal Handlers

      // 1. Print prompt
      
      command = readline("# "); 
//printf("Command is %s\n",command);

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
      if(pid == 0){

      }

      // 6. Other commands for job stuff

   }

}


