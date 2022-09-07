//jobs

typedef struct jobStack { 
    pid_t pgid; 
    int status;
    int jobNum;
    char *command;
    struct jobStack *next;

} jobStack;

jobStack *makeStack();
void push(pid_t pgid,int status,char *command);
jobStack pop();
int getStackSize();
int getHighestJobNum();
void printStack();

int jobsCommandCheck(char *command);
void executeJobs(char *command,char **tokens,jobStack *stack);


