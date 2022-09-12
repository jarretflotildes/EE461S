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
#include "actionModule.h"

#define STACKSIZE 30
/*
typedef struct job { 
    pid_t pgid; 
    int runState; 0 - STOPPED, 1 - RUNNING, 2 - DONE
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

   node->command = malloc(sizeof(char*) * strlen(command));
   strcpy(node->command,command);

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
      value = *node;
      free(JobList[Top]);
      JobIndex--;
      Top--;
   }

   return value;

}

int peek(){
   return Top;
}

void changeRunState(int stackNum,int newRunState){

   for(int i = 0;i<JobIndex;i++){
      if(i == stackNum){
         JobList[i]->runState = newRunState;
      }
   }
}

//do later 
void writeNode(job node){
   //printf("[%d] Done          %s\n",node.jobNum,node.command);
   //char *string = "[";
   //write(0,newLine,sie);
}

void printNode(job node){
   printf("[%d] Done          %s\n",node.jobNum,node.command);
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

//int runState; 0 - STOPPED, 1 - RUNNING, 2 - DONE
void printStack(){

   char *runState[3] = {"Stopped","Running","Done"};

   for(int i = 0;i<JobIndex;i++){ 
      if(i == Top){ //TOP + 
         printf("[%d]+ %s              %s\n",JobList[i]->jobNum,runState[JobList[i]->runState],JobList[i]->command);
      } else { //NOT TOP -
         printf("[%d]- %s              %s\n",JobList[i]->jobNum,runState[JobList[i]->runState],JobList[i]->command);
      }
   }
}

//int runState; 0 - STOPPED, 1 - RUNNING, 2 - DONE
void printFinishedJobs(){

   char *runState[3] = {"Stopped","Running","Done"};

   for(int i = 0;i<JobIndex;i++){ 
      if(JobList[i]->runState == 2) { //NOT TOP -
         printf("[%d]-  %s               %s\n",JobList[i]->jobNum,runState[JobList[i]->runState],JobList[i]->command);
      }
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

void executeJobs(char *command,char **tokens,int yashPid){


   //user just entered \n character
   if(sizeOfArray(tokens) == 0){
      return;
   }

   int status = 0;

   if(strcmp(command,"jobs") == 0){
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
         //printf("this contains &\n");
         pid_t pid = fork(); //Child block act as process control parent 
	
	 if(pid == 0){
            pid_t childPid = getpid();
      	    setpgid(0,0);

            push(pid,1,command); 
            signal(SIGINT,SIG_DFL);  //set disposition back to default
            signal(SIGTSTP,SIG_DFL);
            
	    // 5. execute commands using execvp or execlp   
            char **removeAnd =chopArray(tokens,sizeOfArray(tokens),0,sizeOfArray(tokens)-1);
	    executeCommand(removeAnd,sizeOfArray(removeAnd),status,childPid);
       	    exit(0);	    

	 } else {
	    setpgid(pid,0); //turn child into new process group
            push(pid,1,command); 
            waitpid(-1,&status,WNOHANG); //main parent keeps going 
         }
      }
   } 
}
