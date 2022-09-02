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

char **parseCommand(char *command); 
void freeParseCommand(char **tokens,int tokenNum);

int getNumberOfTokens(char *command); 
int getTokenLocation(char *tokens[],char *string);

char **choppedArray(char *newArray[],char **originalArray,int start,int end);



static void sig_int(int signo) {
   printf("caught SIGINT\n");
}

int main(){
	
   printf("Hello World\n");

   char *command;    
   pid_t pid = -1;

   int pipefd[2] = {0};
   int status = 0;
   int done = 0;

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
      char **tokens = parseCommand(command);
      int tokenNum = getNumberOfTokens(command);
char *stdIn[getTokenLocation(tokens,"|")];
stdIn = *choppedArray(stdIn,tokens,0,1);
for(int i = 0;i<tokenNum;i++){
printf("tokens[%d] = %s\n",i,tokens[i]);
}
      //3. Check for job control Token

      //modify tokens for file direction
      if(strstr(command,">")){
        //get location of token
	int location = getTokenLocation(tokens,">");
	int fd = open(tokens[location + 1],O_RDWR,mode); //file name is next value in array
	dup2(fd,1);
	//remove > from array
      }

      //User wants to Pipe
      if(strstr(command,"|")){
	pid = fork(); //child takes care of piping children
	if(pid == 0){
           pid = fork();  	   
	   if(pid == 0){
	      close(pipefd[0]);              //close unused read end 
              dup2(pipefd[1],STDOUT_FILENO); //output goes to right end ok pipe toke
              execvp(tokens[0],tokens); 
	      exit(0);
	   }

	   pid = fork();
	   if(pid == 0){
	      close(pipefd[1]);              //close unused write end
              dup2(pipefd[0],STDIN_FILENO);
	      execvp(tokens[2],tokens);
	      exit(0);
	   }

	   close(pipefd[0]);
	   close(pipefd[1]);
           waitpid(-1,&status,0);
	   waitpid(-1,&status,0);
	   exit(0);
	} else{
           waitpid(pid,&status,0);       //parent waits for child in charge of piping children
	}
           
      } else { //User does not pipe

         //4. Determine number of children processes to create (# times to call fork()) 
         pid = fork();
 
         // 5. execute commands using execvp or execlp   
         if(pid == 0){
            execvp(tokens[0],tokens);// first in array is always command
	    exit(0);
         } 
            wait((int*)NULL); //wait for any child
      }
 
    // 6. Other commands for job stuff
    
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

//Function returns subset of array
char **choppedArray(char *newArray[],char **originalArray,int start,int end){

    for(int i = start;i<end;i++){
       newArray[i] = originalArray[i];
printf("stdIn[%d] = %s\n",i,newArray[i]);
    }

    return newArray;


}



