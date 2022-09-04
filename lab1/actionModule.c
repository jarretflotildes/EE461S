#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "parse.h"
#include "actionModule.h"

#define mode S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH

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
              redirectionChecks(pipeLeft);
              execvp(pipeLeft[0],pipeLeft); 
	      exit(0);
	   }

	   pid = fork();

	   if(pid == 0){
              close(pipefd[1]);              //close unused write end
              dup2(pipefd[0],STDIN_FILENO);
	      char **pipeRight = chopArray(tokens,tokenNum,location+1,tokenNum);
              redirectionChecks(pipeRight);
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

//ONLY USE INSIDE CHILD PROCESS
void redirectionChecks(char **tokens){

    //2> has to be checked before >
    if(getLocationOfStringArray(tokens,"2>") != -1){
        int location = getTokenLocation(tokens,"2>");        
	stdErrNextToken(tokens,sizeOfArray(tokens),location);
	exit(0);
    }

    if(getLocationOfStringArray(tokens,">") != -1){
        int location = getTokenLocation(tokens,">");        
	stdOutNextToken(tokens,sizeOfArray(tokens),location);
	exit(0);
    }
 
    if(getLocationOfStringArray(tokens,"<") != -1){
        int location = getTokenLocation(tokens,"<");        
	stdInNextToken(tokens,sizeOfArray(tokens),location);
	exit(0);
    }
}

void pipeCheck(char **tokens){
 

      if(getLocationOfStringArray(tokens,"|") != -1){
            int location = getTokenLocation(tokens,"|");
            int status = 0;	    
            pipeCommand(tokens,sizeOfArray(tokens),status,location);		
      } 
}


//< will replace stdin with the file that is the next token
void stdInNextToken(char **tokens,int tokenNum,int location){
        int fd = 0;
 
	int status = 0;
	pid_t pid = fork();

	if(pid == 0){
	   char **leftSide = chopArray(tokens,location,0,location);
	   char **rightSide = chopArray(tokens,tokenNum,location+2,tokenNum); //ignore < and file.txt
	   char **mergedArray = mergeArray(leftSide,rightSide);

	   fd = open(tokens[location+1],O_RDWR,mode); //file is right after >         
           dup2(fd,0);
	   
	   redirectionChecks(mergedArray);
      	   
           execvp(tokens[0],mergedArray);	
           exit(0);	 
	}

        else { 
           waitpid(pid,&status,0);	
 	}	

}
	 
//> will replace stdout with the file that is the next token
void stdOutNextToken(char **tokens,int tokenNum,int location){
        int fd = 0;
 
	int status = 0;
	pid_t pid = fork();

	if(pid == 0){
	   char **leftSide = chopArray(tokens,location,0,location);
	   char **rightSide = chopArray(tokens,tokenNum,location+2,tokenNum); //ignore > and file.txt
	   char **mergedArray = mergeArray(leftSide,rightSide);

	   fd = open(tokens[location+1],O_RDWR,mode); //file is right after >         
           dup2(fd,1);
 	    
	   redirectionChecks(mergedArray);
      	   
	   execvp(tokens[0],mergedArray);	
           exit(0);	 
	}

        else { 
           waitpid(pid,&status,0);	
 	}	

}

//2> will replace stderr with the file that is the next token
void stdErrNextToken(char **tokens,int tokenNum,int location){
        int fd = 0;
 
	int status = 0;
	pid_t pid = fork();

	if(pid == 0){
	   char **leftSide = chopArray(tokens,location,0,location);
	   char **rightSide = chopArray(tokens,tokenNum,location+2,tokenNum); //ignore 2> and file.txt
	   char **mergedArray = mergeArray(leftSide,rightSide);
	   fd = open(tokens[location+1],O_RDWR,mode); //file is right after 2>         
           dup2(fd,2);
 	    
	   redirectionChecks(mergedArray);
      	   
	   execvp(tokens[0],mergedArray);	
           exit(0);	 
	}

        else { 
           waitpid(pid,&status,0);	
 	}	

}	


