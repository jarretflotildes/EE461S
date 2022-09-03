#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "parse.h"
#include "actionModule.h"

void pipeCommand(char **tokens,int tokenNum,int status,int pipefd[],int location){

	int stringLength = 30;
	
	pid_t pid = fork();  	   

	if(pid == 0){
	   close(pipefd[0]);              //close unused read end 
           dup2(pipefd[1],STDOUT_FILENO); //output goes to right end ok pipe token	      
	   char **pipeLeft = chopArray(tokens,location,0,location);
	   //check for file redirection        
	   execvp(pipeLeft[0],pipeLeft); 
	   exit(0);
	}

	if(pid == 0){
           close(pipefd[1]);              //close unused write end
           dup2(pipefd[0],STDIN_FILENO);
	   int end = tokenNum - location;
	   char **pipeRight = chopArray(tokens,end,location,end);
	   //check for file redirection	
	   execvp(pipeRight[0],pipeRight);
	   exit(0);
	}

	pid = fork();

	close(pipefd[0]);
	close(pipefd[1]);
        waitpid(-1,&status,0);
        waitpid(-1,&status,0);
	exit(0);

}


