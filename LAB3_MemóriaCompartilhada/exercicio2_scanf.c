#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main () {
    int segmento;
    char * mensagem;

    // Aloca a memória compartilhada
    segmento = shmget(8180, 500*sizeof (char), IPC_CREAT | IPC_EXCL | S_IRUSR |  S_IWUSR);
    mensagem = (char*) shmat(segmento, 0, 0);

    printf("Digite a mensagem do dia:\n");
    scanf(" %[^\n]", mensagem);
    
    return 0;
}
