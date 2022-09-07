#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "jobs.h"


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

   return poppedNode;

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

   if(MyStack->next == NULL){
      return;
   }

   jobStack ptr = pop();
 
   printStack();
  
   printf("[%d]                     %s\n",ptr.jobNum,ptr.command);

   push(ptr.pgid,ptr.status,ptr.command);


}


//commands are jobs,&,fg,bg
int jobsCommandCheck(char *command){
   int jobFlag = 0;
   if(strcmp(command,"jobs") == 0 ||
      strcmp(command,"fg") == 0  || 
    //  strstr(command,"&") || 
      strcmp(command,"bg") == 0){

      jobFlag = 1;
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
