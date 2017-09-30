#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1

int delay;

int main () {
	pid_t pid1,pid2,pid3,trc;

	if ((pid1 = fork()) < 0){ // Processo pai
		fprintf(stderr, "Erro ao criar Processo filho 1.\n");
		exit(-1);
	}

	if (pid1 == 0) {
		execv("prog1", NULL);
	}
	else {
		if ((pid2 = fork()) < 0){
			fprintf(stderr, "Erro ao criar Processo filho 2.\n");
			exit(-1);
		}

		if (pid2 == 0) {
			execv("prog2", NULL);
		}
		else {
			if ((pid3 = fork()) < 0){
				fprintf(stderr, "Erro ao criar Processo filho 3.\n");
				exit(-1);
			}

			if (pid3 == 0) {
				execv("prog3", NULL);
			}
			else {
				kill(pid1, SIGSTOP);
				kill(pid2, SIGSTOP);
				kill(pid3, SIGSTOP);

				while(TRUE) {
					kill(pid1, SIGCONT);

					sleep(1);
					kill(pid1, SIGSTOP);
					kill(pid2, SIGCONT);

					sleep(2);
					kill(pid2, SIGSTOP);
					kill(pid3, SIGCONT);

					sleep(2);
					kill(pid3, SIGSTOP);
				}
			}
		}
	}
	return 0;
}
