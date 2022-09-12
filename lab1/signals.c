#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h> 

#include "signals.h"


/*Signals (SIGINT, SIGTSTP, SIGCHLD)
	   -  Ctrl-c must quit current foreground process (if one exists) and not the shell
	   and should not print the process (unlike bash)
	   -  Ctrl-z must send SIGTSTP to the current foreground process and should not
	   print the process (unlike bash)
	   - The shell will not be stopped on SIGTSTP
*/

pid_t YashPid;
job **StackPtr;

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
}

void registerJobStack(job **stackPtr){
   StackPtr = stackPtr;
}
void saveYash(pid_t yash){
   YashPid = yash;
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
       job node = pop();
       if(node.jobNum > 0 ){
          kill(-1*node.pgid,SIGINT); 
          push(node.pgid,node.runState,node.command);
       }
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
   } 


}

static void sig_chld(int signo){
   int status = 0;
   waitpid(-1,&status,WUNTRACED | WNOHANG);
 
   if(getpid() == YashPid){
      checkKilledPids(); 
   }
}
