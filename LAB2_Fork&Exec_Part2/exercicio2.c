//
//  exercicio2.c
//  INF1019
//
//  Created by Lucas Ferraço on 31/08/17.
//  Copyright © 2017 Lucas Ferraço. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int i, j;
	int M1[3][3], M2[3][3];
	
	printf("M1:\n");
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			M1[i][j] = 2 + i;
			printf("%d\t", M1[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	printf("M2:\n");
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			M2[i][j] = 2 + j*i;
			printf("%d\t", M2[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	printf("M3:\n");
	for (i = 0; i < 3; i++) {
		sleep(i);
		if (fork() == 0) {
			printf("Linha %d:\t", i);
			
			for (j = 0; j < 3; j++) {
				printf("%d\t", M1[i][j] + M2[i][j]);
			}
			printf("\n");
			exit(0);
		}
	}
	
	return 0;
}
