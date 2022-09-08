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
//jobs.h in signals.h

int main(){
	
   //printf("Hello World\n");

   char *command;  

   pid_t pgid = -1;
   pid_t newPgid = 0;
 
   int status = 0;
   int pipefd[2] = {0};
	
   int fd = 0;

   jobStack *stackPtr = makeStack();
   pid_t yashPid = tcgetpgrp(STDIN_FILENO);

   setSignalJobStack(stackPtr);
   saveYash(yashPid);

   // 0. Register Signal Handlers
   registerSignals();
  
   while(1){

      // 1. Print prompt
      command = readline("# "); 
    
      //Ctrl-d (EOF) will exit the shell 
      if(command == NULL){
         free(stackPtr);
         printf("\n");
         break;
      }

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);

      //3. Check for job control Token
      
      if(!jobsCommandCheck(command,tokens)){
     
         //4. Determine number of children processes to create (# times to call fork()) 

	 //   Child block act as process control parent 
         pgid = fork();
	 newPgid++;
	 push(pgid,status,command);

	 if(pgid == 0){
       	    pgid = newPgid;
	    tcsetpgrp(STDIN_FILENO,pgid);
            // 5. execute commands using execvp or execlp   
	    executeCommand(tokens,tokenNum,status,pgid);
	    tcsetpgrp(STDIN_FILENO,yashPid);
//printf("Child pgrp is %d\n",pgid);
	    exit(0);	    
	 } else {
//printf("Parent pgrp is %d\n",yashPid);	
	    waitpid(pgid,&status,WUNTRACED); 
	    pop();
 
	 }

      } else {
         // 6. Other commands for job stuff
         executeJobs(command,tokens,stackPtr); 
      }

      freeParseCommand(tokens,tokenNum);
      free(command);

   }

}

