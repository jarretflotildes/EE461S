#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "parse.h"
#include "actionModule.h"

void pipeCommand(char **tokens,int tokenNum,int status,int location){

	int stringLength = 30;

        int pipefd[2] = {0};	
	pipe(pipefd);
 	
	pid_t pid = fork();  	   
	if(pid == 0){
	   close(pipefd[0]);              //close unused read end 
           dup2(pipefd[1],STDOUT_FILENO); //output goes to right end ok pipe token	      
	   char **pipeLeft = chopArray(tokens,location,0,location);

	   int i = 0;

	   //check for file redirection        
	   execvp(pipeLeft[0],pipeLeft); 
	   exit(0);
	}

	pid = fork();

	if(pid == 0){
           close(pipefd[1]);              //close unused write end
           dup2(pipefd[0],STDIN_FILENO);

	   char **pipeRight = chopArray(tokens,tokenNum,location+1,tokenNum);

	   //check for file redirection	
	   execvp(pipeRight[0],pipeRight);
	   exit(0);
	}

	close(pipefd[0]);
	close(pipefd[1]);
        waitpid(-1,&status,0);
        waitpid(-1,&status,0);
	exit(0);

}


