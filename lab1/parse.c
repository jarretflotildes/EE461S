#include <stdlib.h>
#include <string.h>


#include <stdio.h>
#include "parse.h"

char **parseCommand(char *command){ 

      char **tokens;
      char *token;
      char *savePtr;
      char *commandCopy; 
      char *toFree;
      
      commandCopy = toFree = strdup(command);

      int stringLength = 30;          //each token will be no more than 30 characters

      tokens = malloc((getNumberOfTokens(command)+1) * sizeof(char*));   //+1 for null

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

void freeParseCommand(char **tokens,int tokenNum){

      for(int i = 0;i<tokenNum;i++){
         free(tokens[i]);
      }

      free(tokens);


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

      return i; 

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

char **chopArray(char **originalArray,int size,int start,int end){

   int stringLength = 30;

   char **newArray = malloc(sizeof(char*) * size);
   int i = 0; 
 
   for(int j = start;j<end;j++){
      newArray[i] = malloc(sizeof(char) * stringLength);
      newArray[i] = originalArray[j];
      i++;
   }
 
   newArray[i] = NULL;

   return newArray;
}


