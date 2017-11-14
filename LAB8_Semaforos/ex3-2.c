#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>

// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);

int main (int argc, char * argv[]) {
  int segmento, * soma, semId;

  // Aloca a memória compartilhada
  segmento = shmget (8180, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
  semId = semget (8765  , 1, 0666 | IPC_CREAT);
  setSemValue(semId);

  // Associa a memória compartilhada ao processo
  soma = (int*) shmat (segmento, 0, 0);
  *soma = 0;

  while (1) {
    semaforoP(semId);

    *soma = (*soma) + 5;
    printf("Soma + 5: %d\n", *soma);

    sleep(rand()%3);
    semaforoV(semId);
    sleep(rand()%2);
  }

  shmdt (soma);
  shmctl (segmento, IPC_RMID, 0);
  delSemValue(semId);

  return 0;
}

int setSemValue(int semId) {
  union semun semUnion;
  semUnion.val = 1;
  return semctl(semId, 0, SETVAL, semUnion);
}

void delSemValue(int semId) {
  union semun semUnion;
  semctl(semId, 0, IPC_RMID, semUnion);
}

int semaforoP(int semId) {
  struct sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = -1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, &semB, 1);
  return 0;
}

int semaforoV(int semId) {
  struct sembuf semB;

  semB.sem_num = 0;
  semB.sem_op = 1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, &semB, 1);

  return 0;
}
