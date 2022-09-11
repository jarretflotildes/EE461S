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

   job **stackPtr = makeStack();
   registerJobStack(stackPtr);

   pid_t yashPid = tcgetpgrp(STDIN_FILENO);

   saveYash(yashPid);
   
    // 0. Register Signal Handlers 
    registerSignals();
    signal(SIGTTOU,SIG_IGN);
//    signal(SIGTTIN,SIG_IGN);

    while(1){ 
      // 1. Print prompt   
      command = readline("# "); 

      //Ctrl-d (EOF) will exit the shell 
      if(command == NULL){
	 freeStack();
         printf("\n");
	 break;
      } 
/*
pid = fork(); //Child block act as process control parent 	
if(pid == 0){

            pid_t childPid = getpid();
      	    setpgid(0,childPid);
	    tcsetpgrp(0,childPid); //child into foreground
char *COMMANDTOKENS[64] = {NULL};
COMMANDTOKENS[0] = "sleep";
COMMANDTOKENS[1] = "2";
execvp(COMMANDTOKENS[0],COMMANDTOKENS);	

exit(0);	    
	 } else {
	    setpgid(pid,0); //turn child into new process group
	    tcsetpgrp(0,pid); //child into foreground
            push(pid,1,command);
printf("child pid is %d\n",pid);
printf("parent pid is %d\n",yashPid);
	    waitpid(pid,&status,WUNTRACED);
	    tcsetpgrp(0,yashPid); //yash into foreground
   if(WIFEXITED(status)){
printf("status is %d\n",status);
     	pop();
    } else if(WIFSIGNALED(status)){ 	
printf("status is %d\n",status);
      	    pop();
    } else if(WIFSTOPPED(status)){  	
printf("status is %d\n",status);
    }
 

	 }	
 printStack(); 
    free(command);
 

    }*/

       
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
	    tcsetpgrp(0,childPid);
    
            signal(SIGINT,SIG_DFL);  //set disposition back to default
            signal(SIGTSTP,SIG_DFL);

	    // 5. execute commands using execvp or execlp   
	    executeCommand(tokens,tokenNum,status,childPid);
            exit(0);	    

	 } else {
	    setpgid(pid,0); //turn child into new process group
	    tcsetpgrp(0,pid); //child into foreground
            push(pid,1,command);
	    waitpid(pid,&status,WUNTRACED); 
	    tcsetpgrp(0,yashPid);

	    if(WIFEXITED(status) || WIFSIGNALED(status)){
	      // printf("exited:%d\n",status);
	      // printf("signaled:%d\n",status);
               free(command);	       
	       pop();	 
            } else if(WIFSTOPPED(status)){  	
              // printf("stopped:%d\n",status);
               //keep stopped process in stack
            }
	 }

      } else{
         // 6. Other commands for job stuff
         executeJobs(command,tokens); 
         free(command);	       
      }

      freeParseCommand(tokens,tokenNum);

printStack();
//printf("yashPid:%d\n",yashPid);
   }

}

