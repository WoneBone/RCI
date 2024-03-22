#include "header.h"
#include "tp.h"

path RTable[100][100];
path SPTable[100];
unsigned char ETable[100];

extern int mid;

/*Função de inicialização da RTable
 * define-se como caminho vazio um caminho de tamanho <=0*/
void initRT(){
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 100; j++){
			RTable[i][j].size = -1;
			RTable[i][j].route[0] = -1;	
		}
	}
}

void initSPT(){
	for(int i = 0; i < 100; i++){
		SPTable[i].size = -1;
		SPTable[i].route[0] = -1;
	}
}

void initET(){
	for(int i = 0; i < 100; i++){
		ETable[i] = -1;
	}
}

unsigned char dest(path p){
	if (p.size > 0) {
        return p.route[p.size - 1]; 
    }
    return -1; 
}

unsigned char source(path p){
	if(p.size >0){
		return p.route[0];
	}
	return -1;
}
/*Função de comparação de paths
 * Retorna -1 se p1 < p2
 * retorna 0 se p1 = p2
 * retorna 1 se p1 > p2
 * renorna 2 se p1 e p2 tiverem o mesmo tamanho mas forem caminhos diferentes*/
int patheq(path p1, path p2){
	if(p1.size > p2.size) return 1;
	else if(p1.size < p2.size) return -1;
	else if(p1.size == p2.size){
		for(int i = 0; i < p1.size; i++)
			if(p1.route[i] != p2.route[i]) return 2;
	}
	return 0;
}

void updateRT(path p){
	unsigned char dst = dest(p);
	unsigned char src = source(p);

	if(p.size <= 0 || dst == (unsigned char) -1 || src == (unsigned char) -1){
		updateRT_empty(p);
		return;
	}

	RTable[dst][src] = p;

	if(source(SPTable[dst]) == src || (SPTable[dst].size > 0 && p.size < SPTable[dst].size))
		updateSP(dst);

}

void updateSP(unsigned char row){
	path p = SPTable[row];
	unsigned char src = source(p);
	unsigned char f = 0;

	for(int i = 0; i < 100; i++){
		//Disparidade entre caminho na SP e na RT. Mesma Source, caminho diferente;
		if(src == i && patheq(SPTable[row], RTable[row][i]) != 0){
			SPTable[row] = RTable[row][i];
			i = 0;
			f = 1;
		}
		//Caminho Válido na RT e SP com caminho vazio
		else if (RTable[row][i].size > 0 && SPTable[row].size <= 0) {
			SPTable[row] = RTable[row][i];
			src = i;
			f = 1;
		}
		//Caminho válido na RT de tamanho inferior ao na RT
		else if( RTable[row][i].size < SPTable[row].size && RTable[row][i].size > 0){
			SPTable[row] = RTable[row][i];
			src = i;
			f = 1;
		}
	}
	
	if(f == 1)
		adj_route(SPTable[row]);
}

void adj_route(path p){
	if(succ.id >= 0){
		sendRoute(p, succ.fd);
	}
	if(pred.id >= 0){
		sendRoute(p, pred.fd);
	}
}

void sendRoute(path p, int fd){
	char buffer[200], element[4];
	unsigned char src = source(p);
	unsigned char dst = dest(p);
	int n = 0;
	if(p.size <= 0){
		sprintf(buffer, "ROUTE %d %d\n", mid, dst);
		n =  write(fd, buffer, strlen(buffer));
		if(n == -1)/*error*/ exit(1);
		return;
	}

	sprintf(buffer, "ROUTE %d %d %d", mid, dst, mid);
	for(int i = 0; i < p.size; i++){
		sprintf(element, "-%02d", p.route[i]);
		strcat(buffer, element);
	}
	strcat(buffer, "\n");
	n = write(fd, buffer, strlen(buffer));
	if(n == -1) /*exit*/ exit(1);
}

void updateRT_empty(path p){
	
	RTable[p.route[1]][p.route[0]] = p;
	if(p.route[0] == source(SPTable[p.route[1]])){
		SPTable[p.route[1]] = p;
		updateSP(p.route[1]);
		
		if(SPTable[p.route[1]].size <= 0){
			adj_route(SPTable[p.route[1]]);
		}
	}
}

void removeCol(unsigned char id){
	path p;
	for(int i = 0; i < 100; i++){
		//Path vazio
		p.size = -1;
		p.route[0] = id;
		p.route[1] = i;
		//Põe na tabela
		updateRT_empty(p);
	}
}
