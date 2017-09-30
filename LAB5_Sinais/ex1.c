#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define TRUE 1

int main() {
  int i;
	pid_t pid1, pid2, pid_aux;

	if((pid1 = fork()) < 0) { // Processo pai
		fprintf(stderr, "Erro ao criar filho\n");
		exit(-1);
	}
	else if(pid1 == 0) { // Processo filho 1
		printf("Processo filho 1 inicializado.\n");

		while(TRUE)
			printf("PF1 - ");

    printf("\n");
	}
	else {
		if((pid2 = fork()) < 0) { // Processo pai
			fprintf(stderr, "Erro ao criar filho\n");
			exit(-1);
		}

		if(pid2 == 0) { // Processo filho 2
			printf("Processo filho 2 inicializado.\n");

			while(TRUE)
				printf("PF2 - ");

      printf("\n");
		}
		else {
			kill(pid1, SIGSTOP);
			kill(pid2, SIGSTOP);

			for(i = 0; i < 10; i++) {
				if(i != 0) {
          printf("\n\n--------- Processo pai alterna os filhos ---------\n\n");
          kill(pid1, SIGSTOP);
        }

				sleep(1);
				kill(pid2, SIGCONT);
				sleep(1);

				pid_aux = pid1;
				pid1 = pid2;
				pid2 = pid_aux;
			}

			kill(pid1, SIGKILL);
			kill(pid2, SIGKILL);

			exit(1);
			}
		}

	return 0;
}
