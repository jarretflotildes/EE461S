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

        pid_t pid = fork();              //child takes care of piping children

        pipe(pipefd); 	

        if(pid == 0){
	  
 	   pid_t pid = fork();  	   
	  
	   if(pid == 0){
	      close(pipefd[0]);              //close unused read end 
              dup2(pipefd[1],STDOUT_FILENO); //output goes to right end ok pipe token	      
	      char **pipeLeft = chopArray(tokens,location,0,location);
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

	else{		
           waitpid(pid,&status,0); //parent waits for child in charge of piping children	
        }

}

//> will replace stdout with the file that is the next token
void stdInNextToken(char **tokens,int tokenNum,int location){
        int fd = 0;
 
	int status = 0;
	pid_t pid = fork();

	if(pid == 0){
	   char **leftSide = chopArray(tokens,location,0,location);
	   char **rightSide = chopArray(tokens,tokenNum,location+1,tokenNum);
	   char **mergedArray = mergeArray(leftSide,rightSide);
//printf("size of left array is %d\n",sizeOfArray(leftSide));
//printf("size of right array is %d\n",sizeOfArray(rightSide));
//printf("size of merged array is %d\n",sizeOfArray(mergedArray));

//printf("commandv args are \n");
//  	   printArray(mergedArray); 
//	  printArray(rightSide);
	 //printArray(leftSide);

	   fd = open(rightSide[0],O_RDWR);        
           dup2(fd,1);

	   //check right side for any other redirections or pipes 
/*	   if(strstr(*rightSide,"|")){
  	      int location = getTokenLocation(tokens,"|");        
	      pipeCommand(mergedArray,sizeOfArray(mergedArray),0,location);

      	   } else{
*/
           execvp(tokens[0],rightSide);

	
//	   }
	
           exit(0);	 
	}

        else { 
           waitpid(pid,&status,0);	
 	}	

}



