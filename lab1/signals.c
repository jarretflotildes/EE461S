#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

#include "signals.h"
#include "jobs.h"

/*Signals (SIGINT, SIGTSTP, SIGCHLD)
	   -  Ctrl-c must quit current foreground process (if one exists) and not the shell
	   and should not print the process (unlike bash)
	   -  Ctrl-z must send SIGTSTP to the current foreground process and should not
	   print the process (unlike bash)
	   - The shell will not be stopped on SIGTSTP
*/

pid_t YashPid;

void registerSignals(){

   //^C
   if(signal(SIGINT,sig_int) == SIG_ERR){
      printf("signal(SIGINT) error");
   }	 
   
   //^Z
   if(signal(SIGTSTP,sig_stp) == SIG_ERR){
      printf("signal(SIGSTP) error");
   }	

}

void saveYash(pid_t yash){
   YashPid = yash;
}


//signal handlers

//-  Ctrl-c must quit current foreground process (if one exists) and not the shell
//and should not print the process (unlike bash)
static void sig_int(int signo) {
 
}
//- Ctrl-z must send SIGTSTP to the current foreground process and should not
//print the process (unlike bash)
//- The shell will not be stopped on SIGTSTP
static void sig_stp(int signo){
  //stop current foreground process and add to job stack
  
  pause();  
}


