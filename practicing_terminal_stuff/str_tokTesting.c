#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **parseCommand(char *command);
int getTokenSize(char *command);

int main(int argc, const char *argv[]){
       	
	char *command = "wow,look,at,these,comma-,separated,tokens";
	char **tokens = parseCommand(command);
     	       
	for(int i = 0;i<getTokenSize(command);i++){
	   printf("Token[%d] is %s\n",i,tokens[i]);
           free(tokens[i]);
	}

	free(tokens);
	
	return 0;

}

int getTokenSize(char *command){
	char *cl_copy, *to_free, *token, *save_ptr;
	int i;

        cl_copy = to_free = strdup(command);

	i = 0;
	while ((strtok_r(cl_copy, ",", &save_ptr))) {
    	   i++;
	   /* First argument to strtok_r must be NULL after the first iteration. */
	   cl_copy = NULL;
	}

	free(to_free);
	
        return i;
}

char **parseCommand(char *command){
	
	char **tokens;
	char *cl_copy, *to_free, *token, *save_ptr;
	int i;

        cl_copy = to_free = strdup(command);

	tokens = malloc(sizeof(char*) * (getTokenSize(command)+1));

        i = 0;
	while ((token = strtok_r(cl_copy, ",", &save_ptr))) {
	   tokens[i] = malloc(sizeof(char) * 30);
	   strcpy(tokens[i],token);
    	   i++;
	   /* First argument to strtok_r must be NULL after the first iteration. */
	   cl_copy = NULL;
	}

	free(to_free);

	return tokens;
}
