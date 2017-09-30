#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1

int valor, time;
int flag = 0;

void iniciarChamada(int sinal);
void terminarChamada(int sinal);

int main () {
	signal(SIGUSR1, iniciarChamada);
	signal(SIGUSR2, terminarChamada);

	while(TRUE) {
		printf("Pronto para realizar ligacoes.\n");
		pause();
	}

	return 0;
}

void iniciarChamada(int sinal) {
	valor = 0;
	time = 0;

	printf ("Ligacao iniciada.\n");

	while(TRUE) {
		if(flag == 1){
			flag = 0;
			break;
		}

		sleep(1);
		time++;

		if (time <= 60)
			valor += 2;
		else
			valor += 1;
	}

	return;
}

void terminarChamada(int sinal) {
  printf ("Ligacao encerrada.\n");
	printf("Sua ligacao durou %d segundos e custou R$%.2f\n", time, valor/100.0);
	flag = 1;

	return;
}
