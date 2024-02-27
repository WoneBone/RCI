/*Este ficheiro serve para conter as funções que tratam dos comandos recebidos pelo utilizador*/
#include "header.h"
extern int errcode;

int join(int ring, int id, struct addrinfo *res){
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	char s[500];
	if(fd == -1) exit(1); //erro
						  //
	sprintf(s,"NODES %d\n", ring);					  
	errcode = sendto(fd, s, strlen(s),0, res->ai_addr, res->ai_addrlen);
	if(errcode == -1) exit(1);//error
							
	errcode = recvfrom(fd, s, 500,0, NULL, NULL);
	if(errcode == -1) exit(1); /*error*/

	puts(s);
}
