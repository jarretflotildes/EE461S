#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "parse.h"
#include "actionModule.h"

void pipeCommand(char **tokens,int tokenNum,int status,int pipefd[],int location){
	pid_t pid = fork();  	   
	int stringLength = 30;

	if(pid == 0){
	   close(pipefd[0]);              //close unused read end 
           dup2(pipefd[1],STDOUT_FILENO); //output goes to right end ok pipe token
	      
	   //get left side of pipe
	   char **pipeLeft = malloc(sizeof(char*) * location);
           int i;
	   for(i = 0;i<location;i++){
	      pipeLeft[i] = malloc(sizeof(char) * stringLength);
	      pipeLeft[i] = tokens[i];
           }
	   pipeLeft[i] = NULL;
	   //check for file redirection
           execvp(pipeLeft[0],pipeLeft); 
	   exit(0);
	}

	pid = fork();
	if(pid == 0){
           close(pipefd[1]);              //close unused write end
           dup2(pipefd[0],STDIN_FILENO);
	   //get right side of pipe
           char **pipeRight = malloc(sizeof(char*) * (tokenNum-location));
	   int i = location;
	   i++;
	   while(tokens[i]!=NULL){
	      pipeRight[i] = malloc(sizeof(char) * stringLength);    
              pipeRight[i] = tokens[i];
              i++;
	   }
	   pipeRight[i] = NULL;
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
