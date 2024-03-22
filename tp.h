#ifndef tp_header
#define tp_header
#include "header.h"

typedef struct PATH{
	unsigned char size;
	unsigned char route[16];
}path;

extern path RTable[100][100];
extern path SPTable[100];
extern unsigned char ETable[100];



extern path RTable[100][100];
extern path SPTable[100];
extern unsigned char ETable[100];

extern int mid;

/*Função de inicialização da RTable
 * define-se como caminho vazio um caminho de tamanho <=0*/
void initRT();

void initSPT();

void initET();
/*Retoma o destino do path ou -1 se tamanho <=0*/
unsigned char dest(path p);
/*Retoma o source do path ou -1 se tamanho <=0*/
unsigned char source(path p);
/*Função de comparação de paths
 * Retorna -1 se p1 < p2
 * retorna 0 se p1 = p2
 * retorna 1 se p1 > p2
 * retorna 2 se tamanho igual mas caminho diferente*/
int patheq(path p1, path p2);
/*Atualiza a RTable com o path. Verifica se é necessário atualizar a SP*/
void updateRT(path p);
/*Recalcula o elemento row da SPTable
 * Se mudar envia Route*/
void updateSP(unsigned char row);

void adj_route(path p);

void sendRoute(path p, int fd);
/*Função específica para colocar paths vazios na RT.
 * MUITO CUIDADO A USAR
 * Perceber muito bem antes de usar.
 * Assume que um path vazio tem src no indice 0 e dest no indice 1
 * MUITO CUITADO A USAR*/
void updateRT_empty(path);
/*Função de remoção de coluna*/
void removeCol(unsigned char id);
//Envia todos os caminhos da SPTable de tamanho > 0
void routall(int fd);
void prtRoute();
#endif
