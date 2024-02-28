/*Este ficheiro serve para conter as funções que tratam dos comandos recebidos pelo utilizador*/
#include "header.h"
extern int errcode;
extern char *mIP, *mTCP;

int join(int ring, int id, struct addrinfo *res){
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	char s[1000], *t;
	int rID;
	char ip[50], port[50];
	//verificação de socket
	if(fd == -1) exit(1); //erro
					

	ip[0] = '\0'; port[0] = '\0';
	//Eviar NODES
	sprintf(s,"NODES %03d\n", ring);					  
	errcode = sendto(fd, s, strlen(s),0, res->ai_addr, res->ai_addrlen);
	if(errcode == -1) exit(1);//error
					
	//receber NODES LIST
	errcode = recvfrom(fd, s, 1000,0, NULL, NULL);
	if(errcode == -1) exit(1); /*error*/

	puts(s);
	t = strtok(s, "\n");
	
	//ERRO, Não recebi NODESLIST<LF>
	if(t == NULL) return -1;
	
	//Primeira Linha
	t = strtok(NULL, "\n");

	//Anel vazio
	if(t == NULL){	
		sprintf(s,"REG %03d %02d %s %s\n", ring, id, mIP, mTCP);					  
		errcode = sendto(fd, s, strlen(s),0, res->ai_addr, res->ai_addrlen);
		if(errcode == -1) exit(1);//error
								  
		errcode = recvfrom(fd, s, 1000, 0, NULL, NULL);
		if(errcode == -1) exit(-1); /*error*/
		puts(s);
		return id;
}

	while(sscanf(t, "%d %s %s", &rID, ip, port) == 3){
		//O id existe no anel
		if(id == rID){
			//Chamada de join com o novo id
			id = join(ring, (id + 1)%100, res);
			//devolver o novo id. Ligação é orientada pelo join recursivo
			return id;
		}
		//nova linha da lista
		t = strtok(NULL, "\n");
		if(t == NULL) break;
	}
	
	//REG
	sprintf(s,"REG %03d %02d %s %s\n", ring, id, mIP, mTCP);					  
	errcode = sendto(fd, s, strlen(s),0, res->ai_addr, res->ai_addrlen);
	if(errcode == -1) exit(1);//error
	
	//OKREG
	errcode = recvfrom(fd, s, 1000, 0, NULL, NULL);
	if(errcode == -1) exit(-1); /*error*/
	puts(s);
	
	//Direct join com o ultimo ip obtido
	d_join();
	return id;
}
