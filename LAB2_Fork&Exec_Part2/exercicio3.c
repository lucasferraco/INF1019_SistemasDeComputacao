#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1

int main (void) {
	int status, i = 0;
	char * parameters[100], * aux;
	char entry[100];
	
	while (TRUE) {
        strcpy(entry, "");
		
		printf("Shell> ");
		scanf(" %[^\n]", entry);
		
		aux = strtok(entry, " \"");
		while (aux != NULL) {
            parameters[i] = aux;
    		aux = strtok (NULL, " \"");
    		i++;
  		}
        
        parameters[i] = NULL;
        
		if (fork() != 0) {
			waitpid(-1, &status, 0);
			i = 0;
		}
		else {
			execvp(parameters[0], parameters);
		}
		
		printf("\n");
	}
	
	return 0;
}
