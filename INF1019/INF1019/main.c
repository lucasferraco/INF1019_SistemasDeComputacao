//
//  main.c
//  INF1019
//
//  Created by Lucas Ferraço on 24/08/17.
//  Copyright © 2017 Lucas Ferraço. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//// QUESTAO 3
//int main(int argc, const char * argv[]) {
//	int i, status1, status2, pidPai = getpid(), pidFilho = fork();
//
//	if (pidFilho != 0) { // Parent Code
//		for (i = 0; i < 3; i++) {
//			printf("\npid %d - %d\n", pidPai, i);
//			sleep(1);
//		}
//		printf("\nProcesso pai vai finalizar\n");
//		
//		waitpid(-1, &status1, 0);
//		printf("\nProcesso pai finalizado\n");
//	}
//	else if (fork() != 0) { // Child Code
//		printf("\nFilho foi criado\n");
//		
//		for (i = 3; i < 6; i++) {
//			printf("\npid %d, pid pai %d - %d\n", getpid(), getppid(), i);
//			sleep(2);
//		}
//		printf("\nProcesso filho vai finalizar\n");
//		
//		waitpid(-1, &status2, 0);
//		printf("\nProcesso filho finalizado\n");
//		exit(1);
//	}
//	else { // Grandchild Code
//		printf("\nNeto foi criado - %d\n", getpid());
//		
//		for (i = 6; i < 9; i++) {
//			printf("\npid %d, pid pai %d - %d\n", getpid(), getppid(), i);
//			sleep(3);
//		}
//		printf("\nProcesso neto vai finalizar\n");
//		printf("\nProcesso neto finalizado\n");
//		exit(1);
//	}
//
//    return 0;
//}

// QUESTAO 4
//int main(int argc, const char * argv[]) {
//	int status;
//
//	if (fork() != 0) { // Parent Code
//		waitpid(-1, &status, 0);
//	}
//	else { // Child Code
//		execv("alomundo.c", NULL);
//		exit(1);
//	}
//
//    return 0;
//}
