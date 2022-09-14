void executeCommand(char **tokens,int tokenNum,int status,pid_t pid);
void blockingWait(pid_t pid);
void pipeCommand(char **tokens,int tokenNum,int status,int location);	
void redirectionChecks(char **tokens); 	  
void stdInNextToken(char **tokens,int tokenNum,int location);
void stdOutNextToken(char **tokens,int tokenNum,int location);
void stdErrNextToken(char **tokens,int tokenNum,int location);
