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
    struct jobStack *next;

} jobStack;
*/
jobStack *MyStack;

//Gives pointer to empty stack
jobStack *makeStack(){
   MyStack = (jobStack *)malloc(sizeof(jobStack));
   MyStack->next = NULL;
   return MyStack;
}

void push(pid_t pgid,int status){
   jobStack *stack = (jobStack *)malloc(sizeof(jobStack));
   stack->next = MyStack;
   stack->pgid = pgid;
   stack->jobNum = getStackSize() + 1;
  
   
   MyStack = stack;
   
}

//Return a jobStack node
jobStack pop(){

   jobStack *poppedNodePtr = MyStack;
   jobStack poppedNode = *poppedNodePtr;   

   MyStack = MyStack->next;
   
   free(poppedNodePtr);
   
   //decrement jobnum of everything in stack
   jobStack *ptr = MyStack;
   while(ptr->next != NULL){
      ptr->jobNum = ptr->jobNum--;
      ptr = ptr->next;
   }


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

void printStack(){
   jobStack *ptr = MyStack;
   while(ptr->next != NULL){
      printf("stack position is at %d with pgid %d with status %d\n",ptr->jobNum,ptr->pgid,ptr->status);
      ptr = ptr->next;
   }

}



