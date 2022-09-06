char **parseCommand(char *command); 
void freeParseCommand(char **tokens,int tokenNum);
int getNumberOfTokens(char *command); 
int getLocationOfStringArray(char **array,char *string);
int getTokenLocation(char *tokens[],char *string);
int sizeOfArray(char **array); 
void printArray(char **array); 
char **chopArray(char **originalArray,int size,int start,int end);
char *findNextFileRedirection(char **array); 
char **mergeArray(char **leftSide, char **rightSide);

