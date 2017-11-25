#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "Queue.h"

#define NUMTHREADS 2
#define MAXELEMENTOS 64

Queue * q;
int notBusy = 1;

void * produtor(void * threadId);
void * consumidor(void * threadId);

int main(int argc, char const *argv[]) {
	int i;
	pthread_t threads[NUMTHREADS];

	q = q_create();

	for (i = 0; i < NUMTHREADS; i++)
		pthread_create(&threads[i], NULL, produtor, (void *)i);

	for (i = 0; i < NUMTHREADS; i++)
		pthread_join(threads[i], NULL);

	q_free(q);

	return 0;
}

void * produtor(void * threadId) {
	int i, r;

	for (i = 0; i < MAXELEMENTOS;) {
		sleep(1);

		if(!q_isFull(q) && notBusy) {
			notBusy = 0;

			r = rand()%40;
			q_enqueue(r, q);

			printf("P  -  adicionou %do. numero (%d)\n", i + 1, r);

			notBusy = 1;
			i++;
		}
		else {
			printf("P  -  Fila cheia/ocupada\n");
		}
	}

	pthread_exit(NULL);
}

void * consumidor(void * threadId) {
	int i, r;

	for (i = 0; i < MAXELEMENTOS;) {
		sleep(2);

		if(!q_isEmpty(q) && notBusy) {
			notBusy = 0;

			r = q_dequeue(q);

			printf("C  -  consumiu %do. numero (%d)\n", i + 1, r);

			notBusy = 1;
			i++;
		}
		else {
			printf("C  -  Fila vazia/ocupada\n");
		}
	}

	pthread_exit(NULL);
}
