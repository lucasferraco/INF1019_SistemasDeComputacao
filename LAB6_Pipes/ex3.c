#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char * argv[]) {
	int fd[2], retorno;

	if(pipe(fd) < 0) {
		printf("Erro ao abrir os pipes\n");
		exit(-1);
	}

	if (fork() != 0) { // Processo Pai
		if ((retorno = dup2(fd[0], 0)) == -1) {
			printf("Erro dup2()\n");
			return -3;
		}
		close(fd[1]);

		argv[0] = "grep";
		argv[1] = "e";
    argv[2] = NULL;

		execvp(argv[0], argv);
	}
	else { // Processo Filho
		if ((retorno = dup2(fd[1], 1)) == -1) {
			perror("Error dup2()");
			return -3;
		}

		argv[0] = "ls";
    argv[1] = NULL;

		execvp(argv[0], argv);
	}

	return 0;
}
