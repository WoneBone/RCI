#include "header.h"

#define REGIP "193.136.138.142"
#define REGUDP "59000"


int main(int argc, char *argv[]){
	char *IP, *TCP,buffer[10000];
	char *regIP;
	char *regUDP;
	int sTCP, sUDP, fd_ret;
	ssize_t n;
	socklen_t addrlen;
	fd_set filhas;
	FD_ZERO(&filhas);
	FD_SET(0,&filhas);

	struct addrinfo *resUDP;
	struct sockaddr_in addr;


	switch (argc) {
		//4 argumentos
		case 5:
			printf("There are 5 arguments\n");

			//Verificar tamanho do valor IP
			if(strlen(argv[3]) > 16){
				puts("Invalid IP address");
				exit(1);
			}
			else
				regIP = argv[3];
			
			//atribuições
			regUDP = argv[4];
			IP = argv[1]; 
			TCP = argv[2];

			//Criar sockets de servidor TCP e cliente UDP
			sTCP = tcp_connect(TCP);
			sUDP = udp_connect(regIP, regUDP, &resUDP);

			
			break;

		case 3:
			printf("There are 3 arguments\n");
			//Atribuições
			IP = argv[1]; 
			TCP = argv[2];

			//Criar sockets de servidor TCP e cliente UDP
			sTCP = tcp_connect(TCP);
			sUDP = udp_connect(REGIP, REGUDP, &resUDP);

			n=sendto(sUDP,"LST\n",4,0,resUDP->ai_addr,resUDP->ai_addrlen);
			if(n==-1) 
				exit(1);

			addrlen=sizeof(addr);
			n=recvfrom(sUDP,buffer,10000,0,(struct sockaddr*)&addr,&addrlen);
			if(n==-1) 
				exit(1);

			write(1,"echo: ",6);
			write(1,buffer,n);

			freeaddrinfo(resUDP);
			close(sUDP);

			break;

		default:
			printf("The number of arguments inputed are invalid");
			exit(1);
			break;
	
}
}

