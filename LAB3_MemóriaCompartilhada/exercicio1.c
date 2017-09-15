#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int main () {
    int i, segmento1, segmento2, segmento3, id, status;
    int * mat1, * mat2, * mat3, * bas1, * bas2, * bas3;

    // Aloca a memória compartilhada
    segmento1 = shmget(IPC_PRIVATE, 9 * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR |  S_IWUSR);
    segmento2 = shmget(IPC_PRIVATE, 9 * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR |  S_IWUSR);
    segmento3 = shmget(IPC_PRIVATE, 9 * sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR |  S_IWUSR);

    // associa a memória compartilhada ao processo
    mat1 = (int *) shmat(segmento1, 0, 0);
    mat2 = (int *) shmat(segmento2, 0, 0);
    mat3 = (int *) shmat(segmento3, 0, 0);

    bas1 = mat1;
    bas2 = mat2;
    bas3 = mat3;

    for (i = 0; i < 8; i++) {
        *mat1 = 1;
        mat1 += 4;

        *mat2 = 1;
        mat2 += 4;
    }
    *mat1 = 1;
    *mat2 = 1;
    mat2 += 4;

    mat1 = bas1;    
    mat2 = bas2;

    if ((id = fork()) < 0) { // Processo pai
            puts ("Erro na criação do novo processo");
            exit (-2);
    }
    else if (id == 0) { // Processo filho 1
            for (i = 0; i < 8; i++) {
                *mat3 = *mat1 + *mat2;
                mat2 += 4;
                mat1 += 4;
                mat3 += 4;
            }
            id = fork();

            if (id == 0) { // Processo filho 2
                for (i = 0; i < 8; i++) {
                    *mat3 = *mat1 + *mat2;
                    mat2 += 4;
                    mat1 += 4;
                    mat3 += 4;
                }
                id = fork();

                if (id == 0) { // Processo filho 3
                    *mat3 = *mat1 + *mat2;
                    mat2 += 4;
                    mat1 += 4;
                    mat3 += 4;
                    *mat3 = *mat1 + *mat2;
                    mat2 += 4;
                    mat1 += 4;
                    mat3 += 4;
                    *mat3 = *mat1 + *mat2;
                }
            }
    }
    else { // Processo pai
        waitpid(-1, &status, 0);

        mat3 = bas3;    
        printf ("matriz resultante:\n");

        printf ("%d ", *mat3);
        mat3 += 4;
        printf ("%d ", *mat3);
        mat3 += 4;
        printf ("%d\n", *mat3);
        mat3 += 4;
        printf ("%d ", *mat3);
        mat3 += 4;
        printf ("%d ", *mat3);
        mat3 += 4;
        printf ("%d\n", *mat3);
        mat3 += 4;
        printf ("%d ", *mat3);
        mat3 += 4;
        printf ("%d ", *mat3);
        mat3 += 4;
        printf ("%d\n", *mat3);
    }

    // Libera a memória compartilhada das matrizes
    shmdt (mat1);
    shmdt (mat2);
    shmdt (mat3);

    // Libera a memória compartilhada
    shmctl (segmento1, IPC_RMID, 0);
    shmctl (segmento2, IPC_RMID, 0);
    shmctl (segmento3, IPC_RMID, 0);

    return 0;
}