#include "Queue.h"

typedef struct node Node;
struct node {
	int value;
	Node * next;
};

Queue * q_create() {
	Queue * q = (Queue *) malloc(sizeof(Queue));

	q->first = NULL;
	q->last = NULL;
	q->size = 0;

	return q;
}

void q_enqueue(int item, Queue * queue) {
	Node * newNode = (Node *) malloc(sizeof(Node));

	newNode->value = item;
	newNode->next = NULL;

	if (queue->first == NULL) {
		queue->first = newNode;
	}
	else {
		queue->last->next = newNode;
	}
	queue->last = newNode;

	(queue->size)++;
}

int q_dequeue(Queue * queue) {
	Node * removingNode = queue->first;
	int value;

	if (removingNode == NULL) {
		return -1;
	}

	value = removingNode->value;

	queue->first = removingNode->next;
	(queue->size)--;
	free(removingNode);

	return value;
}

int q_isFull(Queue * queue) {
	return queue->size == MAXFILA;
}

int q_isEmpty(Queue * queue) {
	return queue->first == NULL;
}

void q_free(Queue * queue) {
	while (queue->first != NULL) {
		q_dequeue(queue);
	}

	free(queue);
}
