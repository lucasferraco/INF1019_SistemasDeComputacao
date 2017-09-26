#include <stdio.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAXFILA 8
#define MAXITENS 64

/// Fila Circular - interface
typedef struct filaCircular {
	int fila[MAXFILA];
	int ini;
	int fim;
	int tam;
} FilaCircular;
void inicializa(FilaCircular * f);
void insereNa(FilaCircular * f, int valor);
int removeNa(FilaCircular * f);
int estaCheia(FilaCircular * f);
int estaVazia(FilaCircular * f);

void produz(FilaCircular * fila, int pidProd, int pidCons);
void consome(FilaCircular * fila, int pidCons, int pidProd);

int main (void) {
	int segmento1, segmento2, status;
	int pidProd, pidCons, * pidConsCompartilhado;
	FilaCircular * fila;
	
	// Aloca a memória compartilhada
	segmento1 = shmget (IPC_PRIVATE, sizeof(FilaCircular), IPC_CREAT | IPC_EXCL | S_IRUSR |  S_IWUSR);
	segmento2 = shmget (IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR |  S_IWUSR);
	
	// Associa a memória compartilhada ao processo
	fila = (FilaCircular *) shmat (segmento1, 0, 0);
	pidConsCompartilhado = (int *) shmat (segmento2, 0, 0);
	
	inicializa(fila);
	*pidConsCompartilhado = 0;
	
	pidProd = fork();
	
	if (pidProd != 0) { // Processo pai
		pidCons = fork();
		
		if (pidCons != 0) { // Processo pai
			*pidConsCompartilhado = pidCons;
			shmdt(pidConsCompartilhado);
			
			waitpid (-1, &status, 0);
			waitpid (-1, &status, 0);
			
			printf("Programa finalizado\n");
			
			shmdt(fila);
			shmctl(segmento1, IPC_RMID, 0);
		}
		else { // Processo consumidor
			printf("C -> processo inicializado\n");
			
			shmdt(pidConsCompartilhado);
			pidCons = getpid();
			
			consome(fila, pidCons, pidProd);
			
			shmdt(fila);
			exit(1);
		}
	}
	else { // Processo produtor
		while (*pidConsCompartilhado == 0);
		printf("P -> processo inicializado\n");
		
		pidCons = *pidConsCompartilhado;
		shmctl(segmento2, IPC_RMID, 0);
		pidProd = getpid();
		
		produz(fila, pidProd, pidCons);
		
		shmdt(fila);
		exit(1);
	}
	
	return 0;
}

void produz(FilaCircular * fila, int pidProd, int pidCons) {
	int i, item;
	
	for (i = 0; i < MAXITENS; i++) {
		// Gero item diferente de 0
		item = rand()%100 + 1;
		
		// Recebo sinal de lotada
		if (estaCheia(fila))
			kill(pidProd, SIGSTOP);
		
		// Insiro item
		insereNa(fila, item);
		printf("P -> %d-esimo item produzido, valor %d\n", i + 1, item);
		
		// Mando sinal de nao vazia
		if (fila->tam == 1)
			kill(pidCons, SIGCONT);
		
		sleep(1);
	}
}

void consome(FilaCircular * fila, int pidCons, int pidProd) {
	int i, item;
	
	for (i = 0; i < MAXITENS; i++) {
		// Recebo sinal de vazia
		if (estaVazia(fila))
			kill (pidCons, SIGSTOP);
		
		// Consumo item
		item = removeNa(fila);
		
		// Mando sinal de nao lotada
		if (fila->tam == MAXFILA - 1)
			kill(pidProd, SIGCONT);
		
		printf("C -> %d-esimo item consumido, valor %d\n", i + 1, item);
		sleep(2);
	}
}

/// Fila Circular - implementacao
void inicializa(FilaCircular * f) {
	int i;
	
	for (i = 0; i < MAXFILA; i++) {
		f->fila[i] = 0;
	}
	
	f->ini = 0;
	f->fim = 0;
	f->tam = 0;
}

void insereNa(FilaCircular * f, int valor) {
	f->fila[f->fim] = valor;
	f->fim = (f->fim + 1) % MAXFILA;
	(f->tam)++;
}

int removeNa(FilaCircular * f) {
	int valor;
	valor = f->fila[f->ini];
	
	f->fila[f->ini] = 0;
	f->ini = (f->ini + 1) % MAXFILA;
	(f->tam)--;
	
	return valor;
}

int estaCheia(FilaCircular * f) {
	return f->tam == MAXFILA;
}

int estaVazia(FilaCircular * f) {
	return f->tam == 0;
}

