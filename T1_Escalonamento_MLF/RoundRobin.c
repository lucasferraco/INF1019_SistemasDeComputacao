#include "RoundRobin.h"

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

RRScheduler * rr_create (int timeSlice) {
	RRScheduler * scheduler = (RRScheduler *) malloc(sizeof(RRScheduler));

	scheduler->inExec = NULL;
	scheduler->readyQueue = q_create();
	scheduler->timeSlice = timeSlice;

	return scheduler;
}

void rr_runNextProcess(RRScheduler * scheduler) {
	if (q_isEmpty(scheduler->readyQueue)) {
		return;
	}

	Process * proc2Run = NULL;
	proc2Run = q_dequeue(scheduler->readyQueue);
	printf("rr_runNextProcess dequeued program %s\n", proc2Run->name);
	int nextSlice = p_findNextExeTime(proc2Run);

	scheduler->inExec = proc2Run;

	if (nextSlice > scheduler->timeSlice) {
		proc2Run->runningTime += scheduler->timeSlice;
	}
	else {
		proc2Run->runningTime += nextSlice;
	}
}

int rr_isEmpty(RRScheduler * scheduler) {
	return q_isEmpty(scheduler->readyQueue) &&
		   scheduler->inExec == NULL;
}

void rr_free (RRScheduler * scheduler){
	q_free(scheduler->readyQueue);

	free(scheduler);
}

// int main() {
// 	RRScheduler * test = rr_create(1);
// 	Process * proc = p_create(1);
// 	strcpy(proc->name, "Nome do processo");
// 	q_enqueue(proc, test->readyQueue);
// 	printf("ta vazio %d\nFila:", rr_isEmpty(test));
// 	q_print(test->readyQueue);
// 	rr_runNextProcess(test);
// 	printf("ta vazio %d\nFila:", rr_isEmpty(test));
// 	q_print(test->readyQueue);
// 	rr_free(test);
// 	printf("cabo\n");
// }
