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

int getStackSize(){
   return JobIndex;
}

void checkKilledPids(){

   for(int i = 0;i<JobIndex;i++){
      int killStatus = kill(-1*JobList[i]->pgid,0);
      if(killStatus == -1){
         changeRunStatePos(JobList[i]->pgid,2); //2 indicates finished job
      }
   }

}

void changeRunStatePos(int pgid,int newRunState){

   for(int i = 0;i<JobIndex;i++){
      if(JobList[i]->pgid == pgid){
         JobList[i]->runState = newRunState;
      }
   }

}

void changeRunState(int stackNum,int newRunState){

   for(int i = 0;i<JobIndex;i++){
      if(i == stackNum){
         JobList[i]->runState = newRunState;
      }
   }
}



void printNode(job node){
   if(JobList[Top]->pgid == node.pgid){
      printf("[%d]+ Done              %s\n",node.jobNum,node.command);
   } else {
      printf("[%d]- Done              %s\n",node.jobNum,node.command);
   }

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
         if(i == Top){ //TOP + 
         printf("[%d]+ %s                 %s\n",JobList[i]->jobNum,runState[JobList[i]->runState],JobList[i]->command);
            } else { //NOT TOP -
            printf("[%d]- %s                 %s\n",JobList[i]->jobNum,runState[JobList[i]->runState],JobList[i]->command);
         }
      }
   }

   cleanJobs();

}


//remove jobs that are finished from array
void cleanJobs(){
   job *newJobList[30] = {NULL};

   int newTop = 0;
   int newJobIndex = 0;

   int j = 0;

   for(int i = 0;i<JobIndex;i++){
      if(JobList[i]->runState != 2){
         newJobList[j] = JobList[i];
         j++;
	 newJobIndex++;
	 newTop = newJobIndex - 1;	 
      }
   }

   *JobList = *newJobList;
   Top = newTop;
   JobIndex = newJobIndex;
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
      if(getStackSize()>0){
         job node = pop(); //get info and immediately return it to stack as running
	 push(node.pgid,1,node.command);

        	      
	 tcsetpgrp(0,node.pgid); //child into foreground
	 kill(-1*node.pgid,SIGCONT);
	 waitpid(node.pgid,&status,WUNTRACED); 
	 tcsetpgrp(0,yashPid);

	 if(WIFEXITED(status) || WIFSIGNALED(status)){
           changeRunState(peek(),2); //change top of stack to done
         } else if(WIFSTOPPED(status)){  	
           changeRunState(peek(),0); //change top of stack to stopped
         }
      }
   }

   if(strcmp(command,"bg") == 0){
      if(getStackSize()>0){
         job node = pop(); //get info and immediately return it to stack as running
	 push(node.pgid,1,node.command);
	 kill(-1*node.pgid,SIGCONT);
         waitpid(-1,&status,WNOHANG); //main parent keeps going 
      }
   }
 
   if(sizeOfArray(tokens) > 1){ //ignore if only character is just &
      if(strcmp(tokens[sizeOfArray(tokens)-1],"&") == 0){
         //printf("this contains &\n");
         pid_t pid = fork(); //Child block act as process control parent 
	
	 if(pid == 0){
            pid_t childPid = getpid();
      	    setpgid(0,0);    //turn child into new process group

            push(pid,1,command); 
            signal(SIGINT,SIG_DFL);  //set disposition back to default
            signal(SIGTSTP,SIG_DFL);
            
	    // 5. execute commands using execvp or execlp   
            char **removeAnd = chopArray(tokens,sizeOfArray(tokens),0,sizeOfArray(tokens)-1);
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
