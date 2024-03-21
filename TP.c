typedef struct PATH{
	unsigned char size;
	unsigned char route[16];
}path;

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

void updateRT(path p){
	
}













