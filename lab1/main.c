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
#include "jobs.h"

#define FALSE 0

int main(){
	
   //printf("Hello World\n");

   char *command;  

   pid_t pid = -1;

   int status = 0;
   int pipefd[2] = {0};
	
   int fd = 0;

   jobStack *stack = makeStack();
/*push(1,9,"asfd");
push(2,1,"sdfasdf");
push(3,2,"asfd");

printStack();

jobStack popped = pop();
printf("popped values are jobNum:%d,status:%d,commnad:%s\n",popped.jobNum,popped.status,popped.command);
printStack();
*/
   // 0. Register Signal Handlers
   while(1){

      // 1. Print prompt
      command = readline("# "); 

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);
      //3. Check for job control Token
      
      if(jobsCommandCheck(command) == FALSE){
         //4. Determine number of children processes to create (# times to call fork()) 
      
         char *redirection = findNextFileRedirection(tokens);
      
         //TODO add child block act as process control parent 
      
         if(strcmp(redirection,"|") == 0){
               int location = getTokenLocation(tokens,"|");        
               pipeCommand(tokens,tokenNum,status,location);		
         } else if(strcmp(redirection,"2>") == 0){
               int location = getTokenLocation(tokens,"2>");
               stdErrNextToken(tokens,tokenNum,location);
         } else if(strcmp(redirection,"<") == 0){
               int location = getTokenLocation(tokens,"<");        
	       stdInNextToken(tokens,tokenNum,location);
         } else if(strcmp(redirection,">") == 0){
               int location = getTokenLocation(tokens,">");        
   	       stdOutNextToken(tokens,tokenNum,location);
         } else  {
   	     //normal exec
             pid = fork();
             // 5. execute commands using execvp or execlp   
             if(pid == 0){
                execvp(tokens[0],tokens);// first in array is always command
	        exit(0);
             } 
            wait((int*)NULL); //wait for any child
         }
         
      } else {

         // 6. Other commands for job stuff
         if(strcmp(command,"jobs") == 0){
            printf("these are the jobs\n");
            printStack();
         }    

         if(strcmp(command,"fg") == 0){
            printf("this is fg\n");
	 }

	 if(strcmp(command,"bg") == 0){
            printf("this is bg\n");
	 }

	 if(strstr(command,"&")){
            printf("this contains &\n");
	 }
      }

      freeParseCommand(tokens,tokenNum);
      free(command);

   }

}

