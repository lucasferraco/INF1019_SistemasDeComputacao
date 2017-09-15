#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	int i, x = 0;

	if (fork() != 0) { // Parent Code
		for(i = 0; i < 100; i++) x += 5;

		printf("Pai %d - %d\n", getpid(), x);
	}
	else { // Child Code
		for(i = 0; i < 100; i++) x += 10;

		printf("Filho %d - %d\n", getpid(), x);
	}

	return 0;
}
