//jobs

typedef struct job { 
    pid_t pgid; 
    int runState;
    int jobNum;
    char *command;
} job;

job **makeStack();
void push(pid_t pgid,int runState,char *command);
void freeStack();
int getHighestJobNum();
void printStack();
void printNode(job node);
job pop();

int jobsCommandCheck(char *command,char **tokens);
void executeJobs(char *command,char **tokens);


