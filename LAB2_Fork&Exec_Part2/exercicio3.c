//
//  exercicio3.c
//  INF1019
//
//  Created by Lucas Ferraço on 31/08/17.
//  Copyright © 2017 Lucas Ferraço. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1

int main (void) {
	int status, i = 0;
	char * aux;
	char * params[] = {"", NULL, NULL};
	char path[100], entry[100], command[10], parameter[1000];
	
	while (TRUE) {
		i = 0;
		strcpy(parameter, "");
		strcpy(path, "/bin/");
		
		printf("> ");
		scanf(" %[^\n]%*c", entry);
		
		aux = strtok(entry, " \"");
		while (aux != NULL) {
    		printf ("AUX: %s\n",aux);

    		if (i == 0) {
    			strcpy(command, aux);
    		}
    		else {
    			strcat(parameter, aux);
    			strcat(parameter, " ");
    		}

    		aux = strtok (NULL, " \"");
    		i++;
  		}
		strcat(path, command);

		if (i < 2) {
			params[1] = NULL;
		}
		else {
			params[1] = (char *) malloc(sizeof(char *));
			strcpy(params[1], parameter);
		}
		
		if (fork() != 0) {
			waitpid(-1, &status, 0);
			sleep(1);
		}
		else {
			printf("ENTRY: %s", entry);
			printf("\nCOMMAND: %s", command);
			printf("\nPATH: %s", path);
			printf("\nPARAMETER: %s", parameter);
			for (int j = 0; j < 3; ++j) {
				printf("\nPARAMS[%d]: %s", j, params[j]);
			}
			printf("\n");
			execvp(path, params);
		}
		
		printf("\n");
	}
	
	return 0;
}