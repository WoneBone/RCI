/*Este ficheiro serve para conter as funções que tratam dos comandos recebidos pelo utilizador*/
#include "header.h"
extern int errcode;

int join(int ring, int id, struct addrinfo *res){
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	char s[1000], *t;
	int rID;
	char ip[50], port[50];
	
	if(fd == -1) exit(1); //erro
						  //
	sprintf(s,"NODES %03d\n", ring);					  
	errcode = sendto(fd, s, strlen(s),0, res->ai_addr, res->ai_addrlen);
	if(errcode == -1) exit(1);//error
							
	errcode = recvfrom(fd, s, 1000,0, NULL, NULL);
	if(errcode == -1) exit(1); /*error*/

	puts(s);
	t = strtok(s, "\n");
	if(t == NULL) return id;
	while(sscanf(t, "%d %s %s", &rID, ip, port) == 3){
		if(id == rID){
			id = join(ring, (id + 1)%100, res);
			break;
		}
		t = strtok(NULL, "\n");
		if(t == NULL) break;
	}

	return id;
}
