#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(void){
    
    char *aux = NULL;
    char *parmList[3] = {"", aux, NULL};
    
    char entrada[100];
    char comando[10];
    char arquivo[100];
    
    int status;
    char raiz[100];
    
    while(1){
        
        strcpy(entrada, "");
        strcpy(arquivo, "");
        strcpy(comando, "");
        strcpy(raiz, "");
        strcpy(raiz, "/bin/");
        
        printf("Shell >> ");
        scanf(" %[^\n]", entrada);
        
        char *token;
        char *search = " \"";

        
        // Token will point to first word.
        token = strtok(entrada, search);
        strcpy(comando, token);
        
        token = strtok(NULL, search);
        while (token != NULL){
            strcat(arquivo, token);
            strcat(arquivo, " ");
            token = strtok(NULL, search);
            if (strlen(arquivo) != 1){
                parmList[1] = (char *) malloc(sizeof(arquivo) * sizeof(char));
                strcpy(parmList[1], arquivo);
            }
            else {
                parmList[1] = NULL;
            }
        }
        
        //junto o comando + "/bin/"
        strcat(raiz,comando);

        
        if (fork() != 0){
            
            printf("\nProcesso do pai iniciado \n");
            waitpid(-1, &status, 0);
            parmList[1] = aux;
            printf("\n Processo do pai finalizado \n");
        }
        else {
            printf("\nProcesso do filho iniciado \n");
            execv(raiz, parmList);
            printf("\nProcesso do filho finalizado \n");
        }
        
    }
    
    return 0;
}