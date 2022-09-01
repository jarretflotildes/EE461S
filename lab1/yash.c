/*
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
void freeParseCommand(char **tokens);

int getTokenLocation(char *tokens[],char *string);
char **removeElem(char *tokens[],int location);
  
static void sig_int(int signo) {
   printf("caught SIGINT\n");
}

int main(){
	
   printf("Hello World\n");
   char *command;    
   pid_t pid = -1;

   mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

   // 0. Register Signal Handlers
   /*Signals (SIGINT, SIGTSTP, SIGCHLD)
	   - Ctrl-c must quit current foreground process (if one exists) and not the shell
	   and should not print the process (unlike bash)
	   - Ctrl-z must send SIGTSTP to the current foreground process and should not
	   print the process (unlike bash)
	   - The shell will not be stopped on SIGTSTP

   if (signal(SIGINT, sig_int) == SIG_ERR){
      printf("signal(SIGINT) error");
   }
   */

   while(1){

      // 1. Print prompt
      command = readline("# "); 

      // 2. Grab and parse input - remove newLine modifier (\n)  
      //    each token will be no more than 30 characters 
      char *tokens[64] = {NULL};
      char *token;
      char *savePtr;
      char *commandCopy = strdup(command);
     
      int i = 0;
      tokens[i] = strtok_r(commandCopy," ",&savePtr);
      while(strtok_r(NULL," ", &savePtr)){
         i++; 
         tokens[i] = token; 
      } 
  
      //3. Check for job control Token
/*
      if(strstr(command,">")){
         //get location of token
	 int location = getTokenLocation(tokens,">");
	 int fd = open(tokens[location + 1],O_RDWR,mode); //file name is next value in array
	 dup2(fd,1);
	 //remove > from array
      }*/

     

      //4. Determine number of children processes to create (# times to call fork()) 
      pid = fork();
 
      // 5. execute commands using execvo or execlp   
      if(pid == 0){
         execvp(tokens[0],tokens);// first in array is always command
	 exit(0);
      } 
    
      // 6. Other commands for job stuff

      free(command);
      wait((int*)NULL);
 
   }

}

char **parseCommand(char *command){ 
      char **tokens;
      char *token;
      char *savePtr;
      char *commandCopy = strdup(command);
 
      int stringSize = 30;          //each token will be no more than 30 characters
     
      int i = 0;
      strtok_r(commandCopy," ",&savePtr);
      while(strtok_r(NULL," ", &savePtr)){
         i++; 
      }

      i++;
      tokens = malloc(i * sizeof(char*));        


      i = 0;
      commandCopy = strdup(command);

      tokens[i] = malloc(stringSize * sizeof(char));  
      tokens[i] = strtok_r(commandCopy," ",&savePtr);
      
      while((token = strtok_r(NULL," ", &savePtr))){
         i++;
	 tokens[i] = (char*)malloc(stringSize + sizeof(char));
         tokens[i] = token;
      }
     
      return tokens;
}


void freeParseCommand(char **tokens){
   int i = 0;
   free(tokens[0]);
   /*
   while(tokens[i] != NULL){
     free(tokens[i]);
     i++;
   }*/
   free(tokens);
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

char **removeElem(char *tokens[],int location){

   int i = location;	 
   int j = location + 1;
   while(tokens[j] != NULL){
      tokens[i] = tokens[j];
      i++; 
      j++;
   }
   tokens[j] = NULL; 

   return tokens;   
}



