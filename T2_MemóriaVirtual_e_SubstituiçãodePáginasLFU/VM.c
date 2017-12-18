#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <unistd.h>

#include "Semaphore.h"
#include "VM.h"

#define SIZE 65536

// ID da memoria compartilhada onde estao armazenados os PageFrames
int segpf;
// Vetor de PageFrames
PageFrame * pageFrames;

// ID da memoria compartilhada onde estao armazenados os PageTables de cada processo
int seg[4];
// Vetor de PageTables dos processos
PageTable * pageTables[4];

// ID da memoria compartilhada onde esta armazenado o semaforo
int semaphore;

// ID da memoria compartilhada onde estao armazenados as paginas que atualmente ocupam os PageFrames
int segQueue;
// Fila de paginas atualmente nos PageFrames.
PageQueue * pageQueue;

/**
 Cria a memória compartilhada para todas as PageFrames.

 @return Vetor de PageFrames.
 */
PageFrame * createPageFrames() {
	int i;
	
	segpf = shmget(IPC_PRIVATE, MB * sizeof(PageFrame), IPC_CREAT | IPC_EXCL | S_IRWXU);
	pageFrames = (PageFrame *) shmat(segpf, 0, 0);
	
	for(i = 0; i < MB; i++) {
		pageFrames[i].accesses = 0;
		pageFrames[i].index = i;
	}
	
	return pageFrames;
}

/**
 Cria a memória compartilhada para a PageTable do processo especificada.

 @param pnumber Numero do processo para qual PageTable vai ser criada.
 @return Ponteiro para a PageTable criada.
 */
PageTable * createPageTable(int pnumber) {
	int i;
	
	seg[pnumber] = shmget(IPC_PRIVATE, SIZE * sizeof(PageTable), IPC_CREAT | IPC_EXCL | S_IRWXU);
	pageTables[pnumber] = (PageTable *) shmat(seg[pnumber], 0, 0);
	
	for(i = 0; i < SIZE; i++) {
		pageTables[pnumber][i].page.index = i;
		pageTables[pnumber][i].page.proc = pnumber;
		pageTables[pnumber][i].page.M = 0;
		
		pageTables[pnumber][i].frame.accesses = -1;
		pageTables[pnumber][i].frame.index = -1;
	}
	
	return pageTables[pnumber];
}

/**
 Cria a memória compartilhada para a PageQueue, a qual descreve a fila de paginas atualmente
 ocupando os PageFrames.

 @return Ponteiro para a PageQueue criada.
 */
PageQueue * createPageQueue() {
	segQueue = shmget(IPC_PRIVATE, sizeof(PageQueue), IPC_CREAT | IPC_EXCL | S_IRWXU);
	semaphore = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	
	// Define o valor de semaforo para a area de memoria da fila,
	// indicando que a mesma e a regiao critica.
	setSemValue(semaphore);
	
	pageQueue = (PageQueue *) shmat(segQueue, 0, 0);
	pageQueue->first = 0;
	pageQueue->next = 0;
	
	return pageQueue;
}

/**
 Traduz um endereco virtual para o endereco fisico correspondente.
 Caso a pagina especificada ocupe um PageFrame atualmente, conta a referencia.
 Caso contrario, emite um sinal SIGUSR1 avisando que tal pagina.

 @param proc_number Numero do processo requisitando a pagina.
 @param index Endereco da pagina.
 @param offset Offset do endereco na pagina.
 @param rw Modo de acesso a pagina ('R' para leitura e 'W' para escrita).
 */
void trans(int proc_number, int index, unsigned int offset, char rw) {
	int frameIndex = pageTables[proc_number][index].frame.index;
	int accesses = pageTables[proc_number][index].frame.accesses;
	
	// Entra na regiao critica
	down(semaphore);
	
	// Verifica se a pagina referenciada ocupa um PageFrame
	if (frameIndex >= 0 && accesses >= 0) {
		pageTables[proc_number][index].frame.accesses++;
		pageFrames[pageTables[proc_number][index].frame.index].accesses++;
		
		printf("P%d, %04x%04x, %c\n", proc_number + 1, frameIndex, offset, rw);
	}
	else { // Page Fault
		
		// Bota pagina na fila
		pageQueue->pages[pageQueue->next].index = index;
		pageQueue->pages[pageQueue->next].proc = proc_number;
		pageQueue->pages[pageQueue->next].offset = offset;
		pageQueue->pages[pageQueue->next].type = rw;
		pageQueue->next = (pageQueue->next + 1) % 4;
		
		// Avisa ao GM que ocorreu um Page Fault
		kill(getppid(), SIGUSR1);
		sleep(1); // Garante que o sinal foi corretamente tratado
		
		printf("P%d, %04x%04x, %c\n", proc_number + 1, pageTables[proc_number][index].frame.index, offset, rw);
	}
	
	// Sai da regiao critica
	up(semaphore);
}
