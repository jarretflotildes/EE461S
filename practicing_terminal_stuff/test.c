#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	const char *command = "wow,look,at,these,comma-,separated,tokens";
	
	char *tokens[64] = {NULL};
	char *cl_copy, *to_free, *token, *save_ptr;
	int i;

        cl_copy = to_free = strdup(command);

	i = 0;
	while ((token = strtok_r(cl_copy, ",", &save_ptr))) {
	   tokens[i] = token;
    	   i++;
	   /* First argument to strtok_r must be NULL after the first iteration. */
	   cl_copy = NULL;
	 }

	i = 0;
	while (tokens[i]) {
	    printf("token[%d] is %s\n", i, tokens[i]);
            i++;									}
	    free(to_free);
        }
	return 0;
}

