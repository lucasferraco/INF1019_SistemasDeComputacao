#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void errorHandler(int signal);

int main() {
	int int1, int2;
	int somInt, subInt, multInt, divisInt;
	float num1, num2;
	float soma, sub, mult, divis;

	signal(SIGFPE, errorHandler);

	printf("Entre dois numeros:\n");
	scanf("%f %f", &num1, &num2);

  printf("\nOperacoes com real\n");
  soma = num1 + num2;
	sub = num1 - num2;
	mult = num1 * num2;
	divis = num1 / num2;

	printf("soma:          %f \n", soma);
	printf("subtracao:     %f \n", sub);
	printf("multiplicacao: %f \n", mult);
	printf("divisao:       %f \n", divis);

  printf("\nOperacoes com inteiro\n");
  int1 = (int) num1;
  int2 = (int) num2;
  somInt = int1 + int2;
  subInt = int1 - int2;
  multInt = int1 * int2;
  divisInt = int1 / int2;

  printf("soma:          %d \n", somInt);
  printf("subtracao:     %d \n", subInt);
  printf("multiplicacao: %d \n", multInt);
  printf("divisao:       %d \n", divisInt);

	return 0;
}

void errorHandler(int signal) {
	printf("Tentativa de divisão por 0\n");
	exit(1);
}
