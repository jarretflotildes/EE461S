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

   pid_t pid = 0;
   
   int status = 0;
   int pipefd[2] = {0};
	
   int fd = 0;

   jobStack *stackPtr = makeStack();
   pid_t yashPid = tcgetpgrp(STDIN_FILENO);

   setSignalJobStack(stackPtr);
   saveYash(yashPid);

   // 0. Register Signal Handlers
   registerSignals();
/*push(0,0,"hello");
printStack();
pop();
printf("popped\n");
printStack();
printf("size of stack is %d\n",getStackSize());
*/ 
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
         pid = fork();
	 push(pid,status,command);

	 if(pid == 0){
            // 5. execute commands using execvp or execlp   
   	    setpgid(getpid(),getpid()); //turn self into new process group
//printf("pgid is %d\n",getpgid(0));
	    executeCommand(tokens,tokenNum,status,getpid());
	    exit(0);	    
	 } else {
//printf("parent pgid is %d\n",getpgid(0));	
            //TODO ADD WAY TO CONTINUE IF CHLD DIED AND DONT POP IF CHLD DIED 
   	    waitpid(pid,&status,WUNTRACED); 
printStack();
	    //	    pop();
//	    tcsetpgrp(STDIN_FILENO,yashPid);
	 }

      } else {
         // 6. Other commands for job stuff
         executeJobs(command,tokens,stackPtr); 
      }

      freeParseCommand(tokens,tokenNum);
      free(command);

   }

}

