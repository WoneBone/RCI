/*Este ficheiro serve para conter as funções que tratam dos comandos recebidos pelo utilizador*/
#include "header.h"
extern int errcode,mid;
extern char *mIP, *mTCP;
extern struct node succ, sucsuc, pred;

int join(int ring, int id, struct addrinfo *res){
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	char s[1000], *t;
	int rID;
	char ip[50], port[50];
	int ids[100];
	//verificação de socket
	if(fd == -1) exit(1); //erro
				
	memset(ids, 0, sizeof(ids));

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
		ids[rID] = 1;
		//nova linha da lista
		t = strtok(NULL, "\n");
		if(t == NULL) break;
	}
	
	while(ids[id] == 1) id = (id+1)%100;
	
	//REG
	sprintf(s,"REG %03d %02d %s %s\n", ring, id, mIP, mTCP);					  
	errcode = sendto(fd, s, strlen(s),0, res->ai_addr, res->ai_addrlen);
	if(errcode == -1) exit(1);//error
	
	//OKREG
	errcode = recvfrom(fd, s, 1000, 0, NULL, NULL);
	if(errcode == -1) exit(-1); /*error*/
	puts(s);
	
	//Direct join com o ultimo ip obtido
	d_join(id, rID, ip, port);
	return id;
}

void d_join(int id,int sucid,char * sucIP, char *sucTCP){
	int fd;
	ssize_t n;
	char send[1000],rec[1000],trash[100];
	strcpy(succ.ip,sucIP);
	strcpy(succ.port,sucTCP);
	succ.id=id;

	fd=tcp_client(sucIP,sucTCP);
    succ.fd=fd;
	sprintf(send,"ENTRY %02d %s %s\n",id,mIP,mTCP);
	n=write(fd,send,strlen(send));
	if(n==-1)/*error*/ exit(1);
    
	/* n=read(fd,rec,strlen(rec));
	if(n==-1)    exit(1);
	
	sscanf(rec,"%s %d %s %s",trash,&sucsuc.id,sucsuc.ip,sucsuc.port); */
	return;

}
