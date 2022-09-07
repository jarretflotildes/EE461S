/*
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

#include "parse.h"
#include "actionModule.h"
#include "signals.h"
#include "jobs.h"

#define FALSE 0

int main(){
	
   //printf("Hello World\n");

   char *command;  

   pid_t pid = -1;
   pid_t pgid = -1;

   int status = 0;
   int pipefd[2] = {0};
	
   int fd = 0;

   jobStack *stack = makeStack();

   // 0. Register Signal Handlers
   registerSignals();
   pid_t yashPid = tcgetpgrp(0);
printf("yash pid is %d\n",yashPid);

   saveYash(yashPid);
push(0,0,"hello");


   while(1){

      // 1. Print prompt
      command = readline("# "); 
    
      //Ctrl-d will exit the shell
      if(command == NULL){
         free(stack);
         printf("\n");
         break;
      }


      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);

      //3. Check for job control Token
      
      if(jobsCommandCheck(command) == FALSE){
     
         //4. Determine number of children processes to create (# times to call fork()) 
      
         //child block act as process control parent 
         pgid = fork();
	 if(pgid == 0){ 
	    executeCommand(tokens,tokenNum,status,pid); 
            exit(0);	    
	 } else {
           waitpid(pgid,&status,WUNTRACED); 
	 }

      } else {
         // 6. Other commands for job stuff
         executeJobs(command,tokens,stack); 
      }

      freeParseCommand(tokens,tokenNum);
      free(command);

   }

}

