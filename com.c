/*Este ficheiro serve para conter as funções que tratam dos comandos recebidos pelo utilizador*/
#include "header.h"
#include "tp.h"
int join(int ring, int id, struct addrinfo *res){
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	char s[1000], *t;
	int rID;
	char ip[50], port[50];
	int ids[100];
	//verificação de socket
	if(fd == -1) exit(1); //erro
				
	memset(ids, 0, sizeof(ids));
	memset(s, '\0', sizeof(s));
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
		mRing = ring;
		return id;
}

	while(sscanf(t, "%d %s %s", &rID, ip, port) == 3){
		printf("%d %s %s\n",rID,ip,port);
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
	mRing = ring;
	return id;
}

void d_join(int id,int sucid,char * sucIP, char *sucTCP){
	ssize_t n;
	char send[1000],rec[1000],trash[100];
	//Atribição dos valores de succ
	strcpy(succ.ip,sucIP);
	strcpy(succ.port,sucTCP);
	succ.id=sucid;
	succ.fd=tcp_client(sucIP,sucTCP);

	//Envio de entry
	sprintf(send,"ENTRY %02d %s %s\n",id,mIP,mTCP);
	n=write(succ.fd,send,strlen(send));
	if(n==-1)/*error*/ exit(1);
  	
	mid = id;
	routall(succ.fd);
	/* n=read(fd,rec,strlen(rec));
	if(n==-1)    exit(1);
	
	sscanf(rec,"%s %d %s %s",trash,&sucsuc.id,sucsuc.ip,sucsuc.port); */
	return;

}

void leave(int id, struct addrinfo *res){
	//UDP for UNREG
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	char s[1000], *t;
	
	//verificação de socket
	if(fd == -1) exit(1); //erro

	//UNREG
	sprintf(s,"UNREG %03d %02d\n", mRing, id);					  
	errcode = sendto(fd, s, strlen(s),0, res->ai_addr, res->ai_addrlen);
	if(errcode == -1) exit(1);//error
	
	//OKUNREG
	errcode = recvfrom(fd, s, 1000, 0, NULL, NULL);
	if(errcode == -1) exit(-1); /*error*/
	puts(s);
	
	/*insert close chords here*/

	//close succ
	if(succ.id != -1){
		close(succ.fd);
		succ.id = -1;
	}
	//close pred
	if(pred.id != -1){
		close(pred.fd);
		pred.id = -1;
	}
	sucsuc.id = -1; 
	sucsuc.fd = -1;

	initET();
	initRT();
	initSPT();

}
int ctt(int org, int dst,int fd,char *carta){
	char envelope[500];
	int n;
	if (strlen(carta)>128){
		printf("Mensagem não enviada pois tamanho maior que 128\n");
		return -1;
	}
	
	sprintf(envelope,"CHAT %02d %02d %s\n",org,dst,carta);
	write(fd,envelope,strlen(envelope));
	if(n==-1)/*error*/ exit(1);
	return 0;
}
int check_serv(struct addrinfo *res,int id,struct node chord){
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	char s[1000], *t;
	int rID;
	char ip_serv[50], port_serv[50];
	int ids[100];
	//verificação de socket
	if(fd == -1) exit(1); //erro
	if (mRing<=0){
		printf("Não me encontro dentro de um anel\n");
		return 1;
	}
	sprintf(s,"NODES %03d\n", mRing);
	errcode = sendto(fd, s, strlen(s),0, res->ai_addr, res->ai_addrlen);
	if(errcode == -1) exit(1);//error
					
	//receber NODES LIST
	errcode = recvfrom(fd, s, 1000,0, NULL, NULL);
	if(errcode == -1) exit(1); /*error*/

	puts(s);
	t = strtok(s, "\n");
	
	//ERRO, Não recebi NODESLIST<LF>
	if(t == NULL) return -1;
	
	t = strtok(NULL, "\n");
	if(t==NULL){
		printf("Anel vazio não é possivel formar corda\n");
		return 1;
	}
	while(sscanf(t, "%d %s %s", &rID, ip_serv, port_serv) == 3){
		printf("%d %s %s\n",rID,ip_serv,port_serv);
		if(rID==id){
			chord.id=id;
			strcpy(chord.ip,ip_serv);
			strcpy(chord.port,port_serv);
			return 0;
		}
		//nova linha da lista
		t = strtok(NULL, "\n");
		if(t == NULL) break;
	}
	printf("Não foi possivel encontrar o nó dentro do anel\n");
	return 1;

}
