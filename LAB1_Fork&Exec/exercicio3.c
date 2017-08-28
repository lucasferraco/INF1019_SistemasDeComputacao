//
//  exercicio3.c
//  
//
//  Created by Lucas Ferraço on 24/08/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	int i, status1, status2, pidPai = getpid(), pidFilho = fork();

	if (pidFilho != 0) { // Parent Code
		for (i = 0; i < 100; i++) {
			printf("\npid %d - %d\n", pidPai, i);
			sleep(1);
		}
		printf("\nProcesso pai vai finalizar\n");

		waitpid(-1, &status1, 0);
		printf("\nProcesso pai finalizado\n");
	}
	else if (fork() != 0) { // Child Code
		printf("\nFilho foi criado\n");

		for (i = 100; i < 200; i++) {
			printf("\npid %d, pid pai %d - %d\n", getpid(), getppid(), i);
			sleep(2);
		}
		printf("\nProcesso filho vai finalizar\n");

		waitpid(-1, &status2, 0);
		printf("\nProcesso filho finalizado\n");
		exit(1);
	}
	else { // Grandchild Code
		printf("\nNeto foi criado - %d\n", getpid());

		for (i = 200; i < 300; i++) {
			printf("\npid %d, pid pai %d - %d\n", getpid(), getppid(), i);
			sleep(3);
		}
		printf("\nProcesso neto vai finalizar\n");
		printf("\nProcesso neto finalizado\n");
		exit(1);
	}

    return 0;
}
