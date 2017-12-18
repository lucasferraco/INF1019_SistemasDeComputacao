#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <time.h>

#include "VM.h"

#define TRUE 1
#define FALSE 0

// Vetor de PageFrames
PageFrame * pageFrames;

// Vetor de PageTables dos processos
PageTable * pageTables[4];
// Vetor de pids dos processos
pid_t pids_procs[4];
// Pid do GM
pid_t pid_gm;

// Fila de paginas atualmente nos PageFrames.
PageQueue * pageQueue;

// Total swaps
int totalSwaps = 0;
// Total Page Faults
int totalPageFaults = 0;

// Timer
time_t start;

void pageFaultHandler(int signal);
void lostFrameHandler(int signal);
void quitHandler(int signal);

// Achar indice e offset de um endereco na memoria virtual
unsigned pageIndex(unsigned addr) {
	unsigned page = addr >> 16;
	return page;
}

unsigned pageOffset(unsigned addr) {
	unsigned offset = addr & 0x0000FFFF;
	return offset;
}

int main() {
	int i;
	unsigned addr;
	char rw;

	// Registra os tratadores de cada sinal
	signal(SIGUSR1, pageFaultHandler);
	signal(SIGINT, quitHandler);
	signal(SIGQUIT, quitHandler);
	
	// Inicializacao
	FILE * compressor = fopen("compressor.log", "r");
	FILE * simulador = fopen("simulador.log", "r");
	FILE * matriz = fopen("matriz.log", "r");
	FILE * compilador = fopen("compilador.log", "r");
	
	FILE * allFiles[4] = {compressor, simulador, matriz, compilador};
	pageFrames = createPageFrames();
	pageQueue = createPageQueue();
	pid_gm = getpid();
	start = time(0);

	// Cria os processos
	for (i = 0; i < 4; i++) {
		pageTables[i] = createPageTable(i);

		if ((pids_procs[i] = fork()) == 0) {
			sleep(1); // Espera pelo GM entrar em seu loop infinito
			
			PageTable * currentTable;
			currentTable = pageTables[i];
			signal(SIGUSR2, lostFrameHandler);

			while(fscanf(allFiles[i], "%x %c", &addr, &rw) > 0){
				int index = pageIndex(addr);
				int offset = pageOffset(addr);

				currentTable[index].page.offset = offset;
				currentTable[index].page.type = rw;

				trans(i, index, offset, rw);
			}
		}
	}

	while(TRUE);

	return 0;
}

void pageFaultHandler(int signal) {
	int i, mostAccesses, hasFreeFrame = FALSE;
	Page pageIn, pageOut;
	PageFrame frameToChange;
	
	// Retira a primeira pagina da fila
	pageIn = pageQueue->pages[pageQueue->first];
	kill(pids_procs[pageIn.proc], SIGSTOP);
	totalPageFaults++;
	
	pageQueue->first = (pageQueue->first + 1) % 4;
	
	/* LFU */
	mostAccesses = pageFrames[0].accesses;
	frameToChange = pageFrames[0];
	
	// Acha o PageFrame a ser substituido
	for (i = 0; i < MB; i++) {
		// Verifica se o frame esta vazio
		if (pageFrames[i].accesses == 0) {
			frameToChange = pageFrames[i];
			hasFreeFrame = TRUE;
			break;
		}
		
		// Verifica o numero total de acessos
		if (pageFrames[i].accesses < mostAccesses) {
			mostAccesses = pageFrames[i].accesses;
			frameToChange = pageFrames[i];
		}
		// Desempata pelo byte de pagina modificada
		else if (pageFrames[i].accesses == mostAccesses &&
				 (pageFrames[i].page.M && !frameToChange.page.M)) {
			mostAccesses = pageFrames[i].accesses;
			frameToChange = pageFrames[i];
		}
	}
	/* fim LFU */
	
	// Verifica se ocorreu Page out
	if (!hasFreeFrame) {
		pageOut = frameToChange.page;
		printf("-> Page out: P%d, %04x%04x, %c\n", pageOut.proc + 1, pageOut.index, pageOut.offset, pageOut.type);
		
		kill(pids_procs[pageOut.proc], SIGUSR2);
	}
	
	// Zera os acessos a pagina que esta saindo do PageFrame
	pageTables[frameToChange.page.proc][frameToChange.page.index].frame.accesses = -1;
	pageTables[frameToChange.page.proc][frameToChange.page.index].frame.index = -1;
	
	// Atualiza PageFrame que esta sendo alterado para a nova pagina
	frameToChange.accesses = 1;
	frameToChange.page.index = pageIn.index;
	frameToChange.page.proc = pageIn.proc;
	frameToChange.page.offset = pageIn.offset;
	frameToChange.page.type = pageIn.type;
	
	pageTables[pageIn.proc][pageIn.index].frame.accesses = frameToChange.accesses;
	pageTables[pageIn.proc][pageIn.index].frame.index = frameToChange.index;
	pageTables[pageIn.proc][pageIn.index].frame.page.index = frameToChange.page.index;
	pageTables[pageIn.proc][pageIn.index].frame.page.proc = frameToChange.page.proc;
	pageTables[pageIn.proc][pageIn.index].frame.page.offset = frameToChange.page.offset;
	pageTables[pageIn.proc][pageIn.index].frame.page.type = frameToChange.page.type;
	
	if (frameToChange.page.type == 'W') {
		frameToChange.page.M = 1;
		
		totalSwaps++;
		sleep(1);
	}
	sleep(1);
	
	kill(pids_procs[pageIn.proc], SIGCONT);
}

void lostFrameHandler(int signal) {
	int i;
	pid_t currentProc = getpid();
	
	for (i = 0; i < 4; i++)
		if (pids_procs[i] == currentProc)
			break;
			
	printf("-> P%d lost page frame\n", i);
}

void quitHandler(int signal) {
	// Calcula tempo de execucao ate a interrupcao
	time_t totalTime = time(0) - start;
	
	if (getpid() == pid_gm) {
		printf("\n-- Encerrou --\n");
		printf("Tempo: %lu\n", totalTime);
		printf("Page Faults: %d\n", totalPageFaults);
		printf("Swaps: %d\n", totalSwaps);
	}
	
	exit (0);
}
