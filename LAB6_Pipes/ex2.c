#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (void) {
	int fd, retorno, retorno2;

	if ((fd = open("teste.txt", O_RDWR|O_CREAT|O_TRUNC, 0666)) == -1) {
		printf("Erro open()\n");
		return -1;
	}

	close(0);

	if ((retorno = dup(fd)) == -1) {
		printf("Erro dup()\n");
		return -2;
	}

	if ((retorno2 = dup2(fd, 1)) == -1) {
		printf("Erro dup2()\n");
		return -3;
	}

	printf("Retorno de dup():  %d\n", retorno);
	printf("Retorno de dup2(): %d\n", retorno2);

	return 0;
}
