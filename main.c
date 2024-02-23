#include "header.h"

#define REGIP "193.136.138.142"
#define REGUDP "59000"


int main(int argc, char *argv[]){
	char *IP, *TCP;
	char *regIP;
	char *regUDP;
	int sTCP, sUDP, fd_ret;
	fd_set filhas;
	FD_ZERO(&filhas);
	FD_SET(0,&filhas);

	struct addrinfo *resUDP;


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
			break;

		default:
			printf("The number of arguments inputed are invalid");
			exit(1);
			break;
	
}
}

