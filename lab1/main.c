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

#include "parse.h"
#include "actionModule.h"
#include "signals.h"
//jobs.h in signals.h

#define STOPPED 0
#define RUNNING 1
#define DONE    2  


int main(){
	
    //printf("Hello World\n");

    char *command;  
    pid_t pid = 0;
    int status = 0;
	
    job **stackPtr = makeStack();
    registerJobStack(stackPtr);

    pid_t yashPid = tcgetpgrp(STDIN_FILENO);
    saveYash(yashPid);
   
    // 0. Register Signal Handlers 
    registerSignals();
    signal(SIGTTOU,SIG_IGN); //done due to tcset quirk

    while(1){ 
      // 1. Print prompt   
      command = readline("# "); 

      //Ctrl-d (EOF) will exit the shell 
      if(command == NULL){
	 freeStack();
         printf("\n");
	 break;
      }

      checkKilledPids(); 
      printFinishedJobs();

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);

      //3. Check for job control Token
      if(jobsCommandCheck(command,tokens) == 0 && strcmp(command,"")!=0){
	 //4. Determine number of children processes to create (# times to call fork()) 
         pid = fork(); //Child block act as process control parent 
	 if(pid == 0){
            pid_t childPid = getpid();
      	    setpgid(0,0);
	    tcsetpgrp(0,childPid);

      	    // 5. execute commands using execvp or execlp   
	    executeCommand(tokens,tokenNum,status,childPid);
            exit(0);	    

	 } else {
	    setpgid(pid,0); //turn child into new process group
	    tcsetpgrp(0,pid); //child into foreground
            push(pid,RUNNING,command);
	    waitpid(pid,&status,WUNTRACED); 
	    tcsetpgrp(0,yashPid);

            if(WIFEXITED(status) || WIFSIGNALED(status)){
     	      job node = pop();
	      free(node.command);
            } else if(WIFSTOPPED(status)){  	
              changeRunState(peek(),STOPPED);
            }
	 }

      } else if(jobsCommandCheck(command,tokens)==1){
         // 6. Other commands for job stuff
         executeJobs(command,tokens,yashPid); 
      }

      freeParseCommand(tokens,tokenNum);
      free(command);	       

   }

}


