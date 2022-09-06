#include <stdlib.h>

#include <string.h>
#include <stdio.h>
#include "parse.h"

#define stringLength 30


char **parseCommand(char *command){ 

      char **tokens;
      char *token;
      char *savePtr;
      char *commandCopy; 
      char *toFree;
      
      commandCopy = toFree = strdup(command);


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

int sizeOfArray(char **array){
    int i = 0;
    while(array[i]!=NULL){
       i++;
    }
    return i;
}

void printArray(char **array){
    int i = 0;
    while(array[i]!=NULL){
       printf("Array[%d] = %s\n",i,array[i]);
       i++;
    }
}

//returns -1 if string not in array
int getLocationOfStringArray(char **array,char *string){

    int location = -1;
    int i = 0;
    while(array[i]!=NULL){
       if(strstr(array[i],string)){
          location = i;
	  break;
       }
       i++;
    }
    return location;

}

char **chopArray(char **originalArray,int size,int start,int end){

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

//find character of next redirection if there is one
// < > 2> | 
//Output: character of next redirection 
char *findNextFileRedirection(char **array){

   int redirection = sizeOfArray(array); 
   char *string = "Nothing found"; 

   for(int i = 0;i<sizeOfArray(array);i++){
      if(strcmp(array[i],"2>") == 0){
          string = "2>";
	  redirection = getTokenLocation(array,string);
          break;  
       }
   }


   for(int i = 0;i<sizeOfArray(array);i++){
      if(strcmp(array[i],"<") == 0){
	 if(redirection > getTokenLocation(array,"<")){
	    string = "<";
	    redirection = getTokenLocation(array,string);
	 }
         break;
      }
   }

   for(int i = 0;i<sizeOfArray(array);i++){
      if(strcmp(array[i],">") == 0){
	 if(redirection > getTokenLocation(array,">")){
	    string = ">";
	    redirection = getTokenLocation(array,string);
	 }
         break;
      }		      
   }

   //pipes take priority
   for(int i = 0;i<sizeOfArray(array);i++){
       if(strcmp(array[i],"|") == 0){
           string = "|";
	   break;  
       }
   }

   return string;

}


char **mergeArray(char **leftSide, char **rightSide){

   int leftSize = sizeOfArray(leftSide);
   int rightSize = sizeOfArray(rightSide);
   char **newArray = malloc(sizeof(char*) * (leftSize + rightSize));
 
   int i = 0;
//   if(leftSize > 0){
      for(i = 0;i<leftSize;i++){
         newArray[i] = malloc(sizeof(char) * stringLength);
         newArray[i] = leftSide[i];
  //    }
   }

//   if(rightSize > 0){

      for(int j = 0;j<rightSize;j++){
         newArray[i] = malloc(sizeof(char) * stringLength);
         newArray[i] = rightSide[j];
         i++;
      }
  // }

   newArray[i] = NULL;

   return newArray;

}



