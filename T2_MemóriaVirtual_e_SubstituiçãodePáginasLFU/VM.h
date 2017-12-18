#define MB 256

/**
 Estrutura que descreve uma pagina da memoria virtual.
 */
typedef struct page {
    int index;
    int proc;
    int offset;
    char type;
	
	int M;
} Page;

/**
 Estrutura que descreve um page frame da memoria virtual.
 */
typedef struct pageFrame {
    int accesses;
    int index;
	
    Page page;
} PageFrame;

/**
 Estrutura que descreve uma tabela de paginas da memoria virtual.
 */
typedef struct pageTable {
    Page page;
    PageFrame frame;
} PageTable;

/**
 Estrutura que descreve a fila de paginas atualmente nos page frames.
 */
typedef struct pageQueue {
    Page pages[4];
	
    int first;
    int next;
} PageQueue;

/**
 Cria a memória compartilhada para todas as page frames.
 
 @return Vetor de PageFrames.
 */
PageFrame * createPageFrames(void);

/**
 Cria a memória compartilhada para a PageTable do processo especificada.
 
 @param pnumber Numero do processo para qual PageTable vai ser criada.
 @return Ponteiro para a PageTable criada.
 */
PageTable * createPageTable(int pnumber);

/**
 Cria a memória compartilhada para a PageQueue, a qual descreve a fila de paginas atualmente
 ocupando os PageFrames.
 
 @return Ponteiro para a PageQueue criada.
 */
PageQueue * createPageQueue(void);

/**
 Traduz um endereco virtual para o endereco fisico correspondente.
 Caso a pagina especificada ocupe um PageFrame atualmente, conta a referencia.
 Caso contrario, emite um sinal SIGUSR1 avisando que tal pagina.
 
 @param pnumber Numero do processo requisitando a pagina.
 @param i Endereco da pagina.
 @param o Offset do endereco na pagina.
 @param rw Modo de acesso a pagina ('R' para leitura e 'W' para escrita).
 */
void trans(int pnumber, int i, unsigned int o, char rw);
