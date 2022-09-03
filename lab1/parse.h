char **parseCommand(char *command); 
void freeParseCommand(char **tokens,int tokenNum);
int getNumberOfTokens(char *command); 
int getTokenLocation(char *tokens[],char *string);
char **chopArray(char **originalArray,int size,int start,int end);
