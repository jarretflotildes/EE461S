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

#define STACKSIZE 30
/*
typedef struct job { 
    pid_t pgid; 
    int runState;
    int jobNum;
    string *command;

} job;
*/
job *JobList[30];

int JobIndex;
int Top; //jobIndex - 1 

job **makeStack(){
   JobIndex = 0;
   Top = 0;
   for(int i = 0;i<STACKSIZE;i++){
      JobList[i] = NULL;
   }
   return JobList;
}

void push(pid_t pgid,int runState,char *command){
   job *node = (job *)malloc(sizeof(job));
   node->pgid = pgid;
   node->runState = runState;

   node->command = command;

   if(JobIndex == 0){
      node->jobNum = 1;
   } else {
      node->jobNum = getHighestJobNum() + 1;
   }
   JobList[JobIndex] = node;

   JobIndex++;
   Top = JobIndex - 1;
}

job pop(){
   
   job value;
   value.jobNum = -1;
	   
   if(JobIndex>0){
      job *node = JobList[Top];

      free(JobList[Top]);
      JobIndex--;
      Top--;
   }

   return value;

}

void printNode(job node){
   printf("node[%d] command is %s\n",node.jobNum,node.command);
}

void freeStack(){
   for(int i = 0;i<JobIndex;i++){
      free(JobList[i]);
   }
   JobIndex = 0;
   Top = 0;
}

int getHighestJobNum(){

   int highest = 0;

   for(int i = 0;i<JobIndex;i++){
      if(JobList[i]->jobNum > highest){
         highest = JobList[i]->jobNum;
      }
   }

   return highest;
}

void printStack(){

   char *runState[2] = {"-","+"};

   for(int i = 0;i<JobIndex;i++){
      printf("[%d] %s               %s\n",JobList[i]->jobNum,runState[JobList[i]->runState],JobList[i]->command);
      printf("pid is %d\n",JobList[i]->pgid);
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

void executeJobs(char *command,char **tokens){

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
