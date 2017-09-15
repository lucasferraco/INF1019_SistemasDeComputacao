#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int i, j;
	int n = 13, v[13] = {31,9,4,5,6,1,3,0,5,7,8,2,12}, val = 9;
	
	for (i = 0; i < 4; i++) {
		if (fork() == 0) {
			
			for (j = i; j < n; j += 4) {
				if (v[j] == val) {
					printf("\nFilho %d Encontrou na posicao %d do vetor.\n", getpid(), j);
					exit(0);
				}
			}
			
			printf("\nFilho %d Nao encontrou ocorrencia do valor.\n", getpid());
			exit(0);
		}
	}
	
	return 0;
}
