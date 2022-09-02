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

char **parseCommand(char *command); 
void freeParseCommand(char **tokens,int tokenNum);

int getNumberOfTokens(char *command); 
int getTokenLocation(char *tokens[],char *string);

int main(){
	
   printf("Hello World\n");

   char *command;    
   pid_t pid = -1;

   int pipefd[2] = {0};
   int status = 0;
   int done = 0;

   mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

   // 0. Register Signal Handlers

   while(1){

      // 1. Print prompt
      command = readline("# "); 

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);
       //3. Check for job control Token
/*
      //4. Determine number of children processes to create (# times to call fork()) 
      pid = fork();
 
      // 5. execute commands using execvp or execlp   
      if(pid == 0){
         execvp(tokens[0],tokens);// first in array is always command
	 exit(0);
      } 
      
      wait((int*)NULL); //wait for any child
      // 6. Other commands for job stuff
  */  
      freeParseCommand(tokens,tokenNum);
      free(command);

   }

}

char **parseCommand(char *command){ 

      char **tokens;
      char *token;
      char *savePtr;
      char *commandCopy; 
      char *toFree;
      
      commandCopy = toFree = strdup(command);

      int stringLength = 30;          //each token will be no more than 30 characters

      tokens = malloc((getNumberOfTokens(command)) * sizeof(char*));   

      int i = 0;

      while((token = strtok_r(commandCopy," ", &savePtr))){
        tokens[i] = malloc(stringLength + sizeof(char));
        strcpy(tokens[i],token);
	commandCopy = NULL;
        i++;
      }

      free(toFree);
 
      tokens[i] = NULL;

     
      return tokens;
}

int getNumberOfTokens(char *command){
      
      char *savePtr;
      char *commandCopy;
      char *toFree;
      commandCopy = toFree = strdup(command);
 
      int i = 0;
      while(strtok_r(commandCopy," ", &savePtr)){
         i++; 
	 commandCopy = NULL;
      }

      free(toFree);

      return i+1; //+1 for NULL terminator

}


void freeParseCommand(char **tokens,int tokenNum){
   
   for(int i = 0;i<tokenNum;i++){
      free(tokens[i]);
   }

   free(tokens);
   tokens = NULL;
}


//get location of token
//preconditions: Character must be in array of tokens
int getTokenLocation(char *tokens[],char *string){

    int i = 0;
    int location = -1;

    while(tokens[i] != NULL){
       if(strstr(tokens[i],string)){
	  location = i;
          break;
       }    
       i++;   
    }

    return location;

}

