#ifndef Queue_h
#define Queue_h

#include <stdio.h>
#include <stdlib.h>

#define MAXFILA 8

typedef struct queue Queue;
struct queue {
	struct node * first;
	struct node * last;

	int size;
};

Queue * q_create (void);

void q_enqueue (int item, Queue * queue);

int q_dequeue (Queue * queue);

int q_isFull(Queue * queue);

int q_isEmpty (Queue * queue);

void q_free (Queue * queue);

#endif /* Queue_h */
