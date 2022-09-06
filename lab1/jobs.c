#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct jobStack { 
    pid_t pgid; 
    int status;
    struct jobStack *next;

} jobStack;

//Gives pointer to empty stack
jobStack *makeStack(){
   jobStack *stack = (jobStack *)malloc(sizeof(jobStack));
   stack->next = NULL;
   return stack;
}

void push(pid_t pgid,int status,jobStack *s){
   jobStack *stack = (jobStack *)malloc(sizeof(jobStack));

   stack->next = s;
   stack->pgid = pgid;

}
	
jobStack *pop(jobStack *s){
   jobStack *poppedNode;


   poppedNode->pgid = s->pgid;
   poppedNode->status = s->status;
   poppedNode->next = NULL;

   s = s->next;
   free(s);
   return poppedNode;

} 

void printStack(jobStack *s){
   jobStack *ptr = s;

   while(ptr->next != NULL){
      printf("stack ptr is at pgid %d with status %d",ptr->pgid,ptr->status);
      ptr = ptr->next;
   }

}



