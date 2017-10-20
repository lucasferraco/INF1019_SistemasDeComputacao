#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
  int fd[2], nEscr, nLido;
  char msgEscr[] = "Teste de Pipe";
  char msgLida[sizeof msgEscr];

  if (pipe(fd) < 0) {
    printf("Erro ao abrir os pipes\n");
    exit(-1);
  }

  if (fork() != 0) { // Processo pai
    nLido = read(fd[0], msgLida, sizeof(msgLida));
    printf("Mensagem lida    (tam = %d): %s\n", nLido, msgLida);
  }
  else { // Processo filho
    nEscr = write(fd[1], msgEscr, strlen(msgEscr) + 1);
    printf("Mensagem escrita (tam = %d)\n", nEscr);
  }

  close(fd[0]);
  close(fd[1]);

  return 0;
}
