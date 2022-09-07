//jobs

typedef struct jobStack { 
    pid_t pgid; 
    int status;
    int jobNum;
    struct jobStack *next;

} jobStack;

jobStack *makeStack();

void push(pid_t pgid,int status);
jobStack pop();
int getStackSize();
void printStack();

