#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "signals.h"


/*Signals (SIGINT, SIGTSTP, SIGCHLD)
	   -  Ctrl-c must quit current foreground process (if one exists) and not the shell
	   and should not print the process (unlike bash)
	   -  Ctrl-z must send SIGTSTP to the current foreground process and should not
	   print the process (unlike bash)
	   - The shell will not be stopped on SIGTSTP
*/
void registerSignals(){
 
   //Ctrl-c
//   if(Signal(SIGINT, sig_int) == SIG_ERR){

 //  }
 	
  

}
