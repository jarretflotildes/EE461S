#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

#include "signals.h"

/*Signals (SIGINT, SIGTSTP, SIGCHLD)
	   -  Ctrl-c must quit current foreground process (if one exists) and not the shell
	   and should not print the process (unlike bash)
	   -  Ctrl-z must send SIGTSTP to the current foreground process and should not
	   print the process (unlike bash)
	   - The shell will not be stopped on SIGTSTP
*/

pid_t YashPid;
jobStack *myStackPtr;

void registerSignals(){

   //^C
   if(signal(SIGINT,sig_int) == SIG_ERR){
      printf("signal(SIGINT) error");
   }	 
   
   //^Z
   if(signal(SIGTSTP,sig_stp) == SIG_ERR){
      printf("signal(SIGSTP) error");
   }

   if(signal(SIGCHLD,sig_chld) == SIG_ERR){
      printf("signal (SIGCHLD) error");
   }   

   if(signal(SIGKILL,sig_kill) == SIG_ERR){
  //    printf("signal(SIGKILL) error");
   }

}

void saveYash(pid_t yash){
   YashPid = yash;
}

void setSignalJobStack(jobStack *myStack){
   myStackPtr = myStack;
}

//signal handlers

//-  Ctrl-c must quit current foreground process (if one exists) and not the shell
//and should not print the process (unlike bash)
static void sig_int(int signo) {
    if(getpgid(STDIN_FILENO) == YashPid && getStackSize() == 0){
 	printf("\n# ");
    } else if(getpgid(STDIN_FILENO) == YashPid){
	jobStack node = pop();
	kill(-1*node.pgid,signo);
    }
}
//- Ctrl-z must send SIGTSTP to the current foreground process and should not
//print the process (unlike bash)
//- The shell will not be stopped on SIGTSTP
static void sig_stp(int signo){
  if(tcgetpgrp(STDIN_FILENO) == YashPid){
     printf("\n# "); //don't do anything 
  } else {
     //stop current foreground process and add to job stack
     printf("ehl");
 }

}

static void sig_chld(int signo){
//printf("child gone\n");
}

static void sig_kill(int signo){
printf("kill\n");

}





