#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFSIZE 1024
#define BIN "/bin/"

void printArgs(char *str[], int n) {
	int i = 0;
	printf("--argumentos (size: %d):  \n", n);
	while (i<n) {
		printf("--%s \n", str[i]);
		i++;
	}
	printf("\n");
}

void cleanArgs(char *args[], int size){
	while (size >= 0){
		free(args[size]);
		size --;
	}

}
int main () {
	char *w, str[BUFSIZE], *argv[100], path[100];
	int status, size, i = 0;

	//recebe comando do prompt
	while(1) {

		printf("> ");
		strcpy(path, BIN);

		scanf("%[^\n]%*c", str);

		//separa os parametros
		w = strtok(str," \"");
		
		while(w != NULL) {
			argv[i] = w;
			i++;
			w = strtok(NULL," \"");
		}

		//adiciona o null ao final do vetor de parametros
		argv[i] = NULL;
		size = i + 1;

		// concatena o comando com "/bin/"
		strcat(path, argv[0]);

		printArgs(argv, size);

		if (fork() != 0) {

			waitpid(-1,&status,0);

			//"zera" os argumentos
			i = 0;

			printf("\n--fim do processo filho \n");

		} else {
			/*Child code*/
			execv(path,argv);
		}

	}

	return 0;
}
