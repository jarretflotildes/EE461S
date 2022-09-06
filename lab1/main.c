/*
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

int main(){
	
   printf("Hello World\n");

   char *command;  

   pid_t pid = -1;

   int status = 0;
   int pipefd[2] = {0};
	
   int fd = 0;
   // 0. Register Signal Handlers


   while(1){

      // 1. Print prompt
      command = readline("# "); 

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);
      //3. Check for job control Token
      //4. Determine number of children processes to create (# times to call fork()) 

      
      
      /*      char *redirection = findNextFileRedirection(tokens);
     
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
*/
      // 6. Other commands for job stuff
      
      
      freeParseCommand(tokens,tokenNum);
      free(command);

   }

}

