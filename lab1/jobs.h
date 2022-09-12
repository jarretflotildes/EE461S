//jobs

typedef struct job { 
    pid_t pgid; 
    int runState;
    int jobNum;
    char *command;
} job;

job **makeStack();
void push(pid_t pgid,int runState,char *command);
job pop();
int peek();
int getStackSize();
int getMostRecentStopped();

void checkKilledPids();
void changeRunStatePos(int pgid,int newRunState);
void changeRunState(int stackNum,int newRunState);
void freeStack();
int getHighestJobNum();

void removePosition(int nodeNum);
void cleanJobs();
void printStack();
void printNode(job node);
void printFinishedJobs();

int jobsCommandCheck(char *command,char **tokens);
void executeJobs(char *command,char **tokens,int yashPid);


