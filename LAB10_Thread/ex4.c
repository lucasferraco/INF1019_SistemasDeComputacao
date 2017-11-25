#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "Queue.h"

#define NUMTHREADS 4
#define MAXELEMENTOS 20

Queue* q;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t production_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t consume_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t product = PTHREAD_COND_INITIALIZER;
pthread_cond_t consume = PTHREAD_COND_INITIALIZER;

void * produtor(void * threadId);
void * consumidor(void * threadId);

int main(int argc, char const *argv[]) {
  int i;
	pthread_t threads[NUMTHREADS];

  q = q_create();

	pthread_create(&threads[0], NULL, produtor, (void *)0);
	pthread_create(&threads[1], NULL, produtor, (void *)1);
	pthread_create(&threads[2], NULL, consumidor, (void *)2);
	pthread_create(&threads[3], NULL, consumidor, (void *)3);

	for (int i = 0; i < NUMTHREADS; i++)
		pthread_join(threads[i], NULL);

	return 0;
}

void * produtor(void * threadId) {
  int i, r;

	for (int i = 0; i < MAXELEMENTOS;) {
		sleep(1);

		if(!q_isFull(q)) {
			pthread_mutex_lock(&mutex);

			r = rand()%40;
			q_enqueue(r, q);

      printf("P  -  adicionou %do. numero (%d) - thread %d\n", i + 1, r, (int)threadId);

			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&consume);
			i++;
		}
		else {
      printf("P  -  Fila cheia - para thread %d\n", (int)threadId);

			pthread_mutex_lock(&production_mutex);
			pthread_cond_wait(&product, &production_mutex);
			pthread_mutex_unlock(&production_mutex);

      printf("P  -  Desbloqueado produtor de thread %d\n", (int)threadId);
		}
	}

	pthread_exit(NULL);
}

void * consumidor(void * threadId) {
	for (int i = 0; i < MAXELEMENTOS;) {
		sleep(2);

		if(!q_isEmpty(q)) {
			pthread_mutex_lock(&mutex);

			int r = q_dequeue(q);

      printf("C  -  consumiu %do. numero (%d) - thread %d\n", i + 1, r, (int)threadId);

			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&product);
			i++;
		}
		else {
      printf("C  -  Fila vazia, para thread %d!\n", (int)threadId);

			pthread_mutex_lock(&consume_mutex);
			pthread_cond_wait(&consume, &consume_mutex);
			pthread_mutex_unlock(&consume_mutex);

      printf("C  -  Desbloqueado consumidor de thread %d\n", (int)threadId);
		}
	}

	pthread_exit(NULL);
}
