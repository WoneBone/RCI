#ifndef header
#define header
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

struct node{
	int id ;
	char ip[100];
	char port[100];
	int fd;	
};
/*******************************************************************
 * Função de inicialização de servidor TCP
 *
 * Cria um servidor TCP com usando IP e TCP como IP e PORT, respetivamente
 * 
 * Retoma o file descriptor da socket criada
 * ****************************************************************/
int tcp_server(char *TCP);

/******************************************************************
 * Função de inicialização de cliente TCP
 *
 * Cria um cliente TCP ligado ao endereço ipTCP na port pTCP
 * 
 * Retoma o fd correspondente à ligação
 * ****************************************************************/
int tcp_client(char *ipTCP, char *pTCP);

/*******************************************************************
 * Função de inicialização de cliente UDP
 *
 * Cria um cliente UDP com usando regIP e regUDP como IP e PORT, respetivamente
 * 
 * Retoma o file descriptor da socket criada
 * ****************************************************************/
int udp_connect(char *regIP, char *regUDP, struct addrinfo **res);

int join(int ring, int id, struct addrinfo *res);

void d_join(int id,int sucid,char * sucIP, char *sucTCP);

int what_serv(int fd, char *mess);

int what_std(char *std_in,struct addrinfo *res);
#endif
