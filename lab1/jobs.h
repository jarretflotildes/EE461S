//jobs
struct jobStack;

jobStack *makeStack();

void push(pid_t pgid,int status,jobStack *s);

jobStack *pop(jobStack *s);

void printStack(jobStack *s);

