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
pop();
pop();*/
   while(1){

      // 1. Print prompt
      
      command = readline("# "); 

      //Ctrl-d (EOF) will exit the shell 
      if(command == NULL){
	 freeStack();
         printf("\n");
	 break;
      }

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);

      //3. Check for job control Token
      
      if(command != "\n" && !jobsCommandCheck(command,tokens)){
         //4. Determine number of children processes to create (# times to call fork()) 

         pid = fork(); //Child block act as process control parent 
	
	 if(pid == 0){
            pid_t childPid = getpid();
      	    setpgid(0,0);

            // 5. execute commands using execvp or execlp   
	    executeCommand(tokens,tokenNum,status,childPid);
            exit(0);	    

	 } else {
	    setpgid(pid,0); //turn child into new process group
	    tcsetpgrp(0,pid); //child into foreground
	    waitpid(pid,&status,WUNTRACED); 
	    tcsetpgrp(0,yashPid);
	 }

      } else{
         // 6. Other commands for job stuff
         executeJobs(command,tokens,stackPtr); 
      }

      freeParseCommand(tokens,tokenNum);
      free(command);

   }

}

