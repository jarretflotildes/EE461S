#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "jobs.h"

#define TRUE 1
#define FALSE 0

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
   MyStack = (jobStack *)malloc(sizeof(jobStack));
   MyStack->next = NULL;
   return MyStack;
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

//Return a jobStack node
jobStack pop(){

   jobStack *poppedNodePtr = MyStack;
   jobStack poppedNode = *poppedNodePtr;   

   MyStack = MyStack->next;
   
   free(poppedNodePtr);
   
   //decrement jobnum of everything in stack
//   jobStack *ptr = MyStack;
//   while(ptr->next != NULL){
//      ptr->jobNum = ptr->jobNum--;
//      ptr = ptr->next;
 //  }


   return poppedNode;

} 
int getStackSize(){

   int count = 0;
   jobStack *ptr = MyStack;
   while(ptr->next != NULL){
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

   jobStack *ptr = MyStack;
   while(ptr->next != NULL){
      //printf("stack position is at %d with pgid %d with status %d\n",ptr->jobNum,ptr->pgid,ptr->status);
      //TODO add +/- and if running/stopped/etc
      printf("[%d]                     %s\n",ptr->jobNum,ptr->command);
      ptr = ptr->next;
   }

}


//commands are jobs,&,fg,bg
int jobsCommandCheck(char *command){
   int jobFlag = FALSE;
   if(strcmp(command,"jobs") == 0 ||
      strcmp(command,"fg") == 0  || 
    //  strstr(command,"&") || 
      strcmp(command,"bg") == 0){

      jobFlag = TRUE;
   }
//printf("jobsFlag is %d\n",jobFlag);
   return jobFlag;
			   
}

void executeJobs(char *command,char **tokens,jobStack *stack){


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

   //need better way to check for &
   if(strstr(command,"&")){
      printf("this contains &\n");
   }

}
