//jobs

typedef struct jobStack { 
    pid_t pgid; 
    int status;
    int jobNum;
    char *command;
    struct jobStack *next;

} jobStack;

jobStack *makeStack();
void freeStack();
void push(pid_t pgid,int status,char *command);
jobStack pop();
int getStackSize();
int getHighestJobNum();
void printStack();
void printFinishedJobs();

int jobsCommandCheck(char *command,char **tokens);
void executeJobs(char *command,char **tokens,jobStack *stack);


