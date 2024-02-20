#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define REGIP "193.136.138.142"
#define REGUDP "59000"

/*******************************************************************
 * Função de inicialização de servidor TCP
 *
 * Cria um servidor TCP com usando IP e TCP como IP e PORT, respetivamente
 * 
 * Retoma o file descriptor da socket criada
 * ****************************************************************/
int tcp_connect(char *IP, char *TCP);

/*******************************************************************
 * Função de inicialização de cliente UDP
 *
 * Cria um cliente UDP com usando regIP e regUDP como IP e PORT, respetivamente
 * 
 * Retoma o file descriptor da socket criada
 * ****************************************************************/
int udp_connect(char *regIP, char*regUDP);

int main(int argc, char *argv[]){
	char *IP, *TCP;
	char *regIP;
	char *regUDP;
	int sTCP, sUDP;


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
			sTCP = tcp_connect(IP, TCP);
			sUDP = udp_connect(regIP, regUDP);
			break;

		case 3:
			printf("There are 3 arguments\n");
			//Atribuições
			IP = argv[1]; 
			TCP = argv[2];

			//Criar sockets de servidor TCP e cliente UDP
			sTCP = tcp_connect(IP, TCP);
			sUDP = udp_connect(REGIP, REGUDP);
			break;

		default:
			printf("The number of arguments inputed are invalid");
			exit(1);
			break;
	}
}

