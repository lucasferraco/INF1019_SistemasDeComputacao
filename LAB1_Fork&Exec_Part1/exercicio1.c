#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	int status, pidPai = getpid(), pidFilho = fork();

	if (pidFilho != 0) { // Parent Code
		printf("Processo pai iniciado\n");

		printf("pid: %d\n", pidPai);
		printf("pid filho: %d\n", pidFilho);

		waitpid(-1, &status, 0);
		printf("Processo pai finalizado\n");
	}
	else { // Child Code
		printf("Processo filho iniciado\n");

		pidFilho = getpid();
		printf("pid: %d\n", pidFilho);
		printf("pid pai: %d\n", pidPai);

		printf("Processo filho finalizado\n");
		exit(0);
	}

    return 0;
}
