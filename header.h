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
#include "LinkedList.h"
#define MAX_CLIENTS 17
#define REGIP "193.136.138.142"
#define REGUDP "59000"
struct node{
	int id ;
	char ip[100];
	char port[100];
	int fd;	
};
extern int errcode,mid;
extern int mRing;
extern char *mIP, *mTCP;
extern struct node succ, sucsuc, pred,my_chord;
extern struct LinkedListStruct *Fire_Link;
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
 * Função que manda mensagem 
 *
 * 
 * ****************************************************************/
int ctt(int org, int dst,int fd,char *carta);

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

int check_serv(struct addrinfo *res,int id,struct node chord);

void dummieFunc(Item); //This function does not concern you. Look away. These are not the funcs you're looking for
#endif
