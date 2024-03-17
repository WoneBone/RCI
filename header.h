#ifndef header
#define header
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#define MAX_CLIENTS 17
#define REGIP "193.136.138.142"
#define REGUDP "59000"
struct node{
	int id ;
	char ip[100];
	char port[100];
	int fd;	
};
struct Path {                        
    int route[MAX_CLIENTS];         // The list of node IDs representing the path
    int size;                       // The current number of nodes in the route
};

extern struct Path routingTable[MAX_CLIENTS-1][MAX_CLIENTS-1];

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
/******************************************************************
 * Função que trata da rotina join
 *
 * Comunica com o servidor UDP identificado por res e tenta ligar-se ao anel ring com o id id
 *
 * Devolve o id efetivamente ligado
 *
 * introduz o programa no anel ring e coloca a variável mRing = ring
 * ****************************************************************/
int join(int ring, int id, struct addrinfo *res);

/******************************************************************
 * Função que trata da rotina direct join
 *
 * Liga-se ao nó identificado por succid, localizado em succIP e succTCP
 * Não retoma nada mas define a variável global succ
 * ****************************************************************/
void d_join(int id,int sucid,char * sucIP, char *sucTCP);
/******************************************************************
 * Função que trata da rotina leave
 *
 * Tira o nó id do anel ring registado no servidor de nós res
 * ****************************************************************/

void leave(int id, struct addrinfo *res);

/******************************************************************
 * Função de identificação de comunicação feita a este servidor TCP
 * ****************************************************************/
int what_serv(int fd, char *mess);

/******************************************************************
 * Função de identificação de comunicação feita pelo utilizador pelo std in
 * ****************************************************************/
int what_std(char *std_in,struct addrinfo *res);

/******************************************************************
 * Função de identificação de comunicação feita por um servidor a que este cliente está ligado
 * ****************************************************************/
int what_clit(int fd, char *mess);


/*Useless struct Path initPath();*/

int dest(struct Path path);

int source(struct Path path);

/* WHAT THIS DO struct Path addNode*/

void initRT();
void initSPT();
void initEXP();
void initmapIndices();
void getIndexLists();
int findOrAssignIndex(int nodeID);
void updateRT(struct Path path);
void updateSPT();
void updateEXP();

void send_route(struct Path path, int fd);
#endif
