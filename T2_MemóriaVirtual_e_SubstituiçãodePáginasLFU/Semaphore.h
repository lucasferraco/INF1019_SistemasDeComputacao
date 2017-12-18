/**
 Inicializa o valor do semaforo.

 @param semId ID do semaforo.
 @return Valor definido do semaforo ou -1, em caso de falha.
 */
int setSemValue(int semId);

/**
 Remove o semaforo.

 @param semId ID do semaforo.
 */
void delSemValue(int semId);

/**
 Operacao P.

 @param semId ID do semaforo.
 @return 0, se bem sucedida, ou 1, em caso de erro.
 */
int down(int semId);

/**
 Operacao V.

 @param semId ID do semaforo.
 @return 0, se bem sucedida, ou 1, em caso de erro.
 */
int up(int semId);
