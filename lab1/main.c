/*
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

#include "parse.h"
#include "actionModule.h"
#include "signals.h"

int main(){
	
   printf("Hello World\n");

   char *command;  

   pid_t pid = -1;

   int status = 0;
   int pipefd[2] = {0};
	
   mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

   // 0. Register Signal Handlers


   while(1){

      // 1. Print prompt
      command = readline("# "); 

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);
      //3. Check for job control Token
      //4. Determine number of children processes to create (# times to call fork()) 
   
      //User wants to Pipe
      if(strstr(command,"|")){
	int location = getTokenLocation(tokens,"|");        
        pid = fork();              //child takes care of piping children
	if(pid == 0){
          pipeCommand(tokens,tokenNum,status,location);		
	} 
	else{		
           waitpid(pid,&status,0); //parent waits for child in charge of piping children	
        }
      } 
      //normal exec
      else {  
  	 pid = fork();
         // 5. execute commands using execvp or execlp   
         if(pid == 0){
            execvp(tokens[0],tokens);// first in array is always command
	    exit(0);
         } 
         wait((int*)NULL); //wait for any child
      }

      // 6. Other commands for job stuff
      

      freeParseCommand(tokens,tokenNum);
      free(command);

   }

}

