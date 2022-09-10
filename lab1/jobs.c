#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "jobs.h"
#include "parse.h"

#define MAXSIZE 20
/*
typedef struct jobStack { 
    pid_t pgid; 
    int status;
    int jobNum;
    string *command;
    struct jobStack *next;

} jobStack;
*/
jobStack *MyStack;

//Initializes MyStack pointer to empty stack
//Only call once
jobStack *makeStack(){
   MyStack = NULL;
   return MyStack;
}

void freeStack(){

   while(MyStack!=NULL){
      pop();
   }

   free(MyStack);

}

void push(pid_t pgid,int status,char *command){
   jobStack *stack = (jobStack *)malloc(sizeof(jobStack));
   stack->next = MyStack;
   stack->pgid = pgid;

   if(getStackSize()==0){ //empty stack first job in stack
      stack->jobNum = 1;
   } else { 
      stack->jobNum = getHighestJobNum();  
   }

   stack->command = command;

   MyStack = stack;
   
}


//return node at top
jobStack peekTop(){


}

//Return a jobStack node
jobStack *pop(){

   jobStack *poppedNodePtr = MyStack;
   jobStack poppedNode;

   if(MyStack != NULL){
      MyStack = MyStack->next;
   }

   if(poppedNodePtr != NULL){      
      jobStack poppedNode = *poppedNodePtr;
      free(poppedNodePtr);
   }
   return poppedNodePtr;

} 
int getStackSize(){

   int count = 0;
   jobStack *ptr = MyStack;

   while(ptr != NULL && ptr->next != NULL){
      ptr = ptr->next;
      count++;
   }

   return count;

}

int getHighestJobNum(){
   int highest = 1;
   
   jobStack *ptr = MyStack;
   while(ptr->next != NULL){
      if(ptr->jobNum >= highest){
         highest = ptr->jobNum + 1;
      }
      ptr = ptr->next;
   }

   return highest;
}

void printStack(){

   if(MyStack == NULL){
      return;
   }

   jobStack *ptr = pop();
   printStack();
   printf("[%d]                     %s\n",ptr->jobNum,ptr->command);
   push(ptr->pgid,ptr->status,ptr->command);
}

void printFinishedJobs(){
   if(MyStack == NULL){
      return;
   }

   jobStack *ptr = pop();
   printFinishedJobs();

   //if done print job
   if(ptr->status == -1){
      printf("[%d]                     %s\n",ptr->jobNum,ptr->command);
      MyStack = ptr->next;
   }else{
      push(ptr->pgid,ptr->status,ptr->command);
   }

}


//commands are jobs,&,fg,bg
int jobsCommandCheck(char *command,char **tokens){
   int jobFlag = 0;

   //user just entered \n character
   if(sizeOfArray(tokens) == 0){
      return jobFlag;
   }


   if(strcmp(command,"jobs") == 0 ||
      strcmp(command,"fg") == 0  || 
      strcmp(tokens[sizeOfArray(tokens)-1],"&") == 0 || 
      strcmp(command,"bg") == 0){

      jobFlag = 1;
   }
//printf("jobsFlag is %d\n",jobFlag);
   return jobFlag;
			   
}

void executeJobs(char *command,char **tokens,jobStack *stack){

   //user just entered \n character
   if(sizeOfArray(tokens) == 0){
      return;
   }

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
 
   if(sizeOfArray(tokens) > 0){
      if(strcmp(tokens[sizeOfArray(tokens)-1],"&") == 0){
         printf("this contains &\n");
      }
   }

}
