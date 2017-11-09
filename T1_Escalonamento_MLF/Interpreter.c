#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>

#define BUFFER_SIZE 200
#define NAME_SIZE 20
#define IO_SIZE 200
#define TRUE 1

int main(int argc, char const *argv[]) {
	char programName[NAME_SIZE];
	char io[IO_SIZE];
	char schedule[IO_SIZE];
	char * buffer[BUFFER_SIZE];
	int j = 1; // Buffer index

	printf("Digite o comando 'exec <nomedoprograma> <sequenciadetempo>'\n");
	printf("Comandos:\n");
	printf("  - pronto : escalona os processos passados;\n");
	printf("  - fim    : termina o programa.\n\n");

	while(scanf(" exec %s %s", programName, io) == 2){
			 buffer[j] = (char*)malloc(strlen(programName)+1);
			 strcpy(buffer[j], programName);
			 j++;
			 buffer[j] = (char*)malloc(strlen(io)+1);
			 strcpy(buffer[j], io);
			 j++;
	 }
	 buffer[0] = "SchedulerMLF";
	 execv("SchedulerMLF", buffer);

	 return 0;
}
