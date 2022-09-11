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

   signal(SIGTTOU,SIG_IGN);
 /*
   if(signal(SIGTTOU,sig_ttou) == SIG_ERR){
      printf("signal (SIGTTOU) error");
   }   
*/

}

void saveYash(pid_t yash){
   YashPid = yash;
}

void setSignalJobStack(jobStack *myStack){
   myStackPtr = myStack;
}

//signal handlers

//Ctrl-c must quit current foreground process (if one exists) and not the shell
//and should not print the process (unlike bash)
static void sig_int(int signo) {
 
    if(tcgetpgrp(STDIN_FILENO) == YashPid){
       char *newLine = "\n# ";
       int size = 4;
       write(0,newLine,size);
    } else {
       tcsetpgrp(STDIN_FILENO,YashPid);     
       kill(-1*tcgetpgrp(STDIN_FILENO),signo); 
    }

}

//Ctrl-z must send SIGTSTP to the current foreground process and should not
//print the process (unlike bash)
//The shell will not be stopped on SIGTSTP
static void sig_stp(int signo){
   if(tcgetpgrp(STDIN_FILENO) == YashPid){
       char *newLine = "\n# ";
       int size = 4;
       write(0,newLine,size);
   } else {  
         
   }

}

static void sig_chld(int signo){
//
}




