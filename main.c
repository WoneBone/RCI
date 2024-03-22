#include "header.h"

#define MAX_CLIENTS 17
#define MAX(a,b) ((a)>(b)?(a):(b))
int errcode,mid;
int mRing=0;
char *mIP;
char *mTCP;
struct node succ, sucsuc, pred,my_chord;

int main(int argc, char *argv[]){
	char std_in[500],tcp_rec[500],tcp_clit[500],trash[500];
	char *regIP, *regUDP;
	char *tok, *aux;
	succ.id=-1;
	sucsuc.id=-1;
	pred.id=-1;
	my_chord.id=-1;
	int sTCP, sUDP, fd_ret, cTCP,maxfd=0,client_fds[MAX_CLIENTS],i,newfd;
	int p_ = 0; 
	ssize_t n,nw;
	socklen_t addrlen;
	fd_set filhas;
	FD_ZERO(&filhas);
	mid=-1;

	struct addrinfo *resUDP;
	struct sockaddr_in addr;

	memset(std_in, 0, 500);
	memset(tcp_rec, 0, 500);
	memset(tcp_clit, 0, 500);

	initEXP();
	initRT();
	initSPT();
	initmapIndices();
	initinvIndices();

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
			mIP = argv[1]; 
			mTCP = argv[2];

			//Criar sockets de servidor TCP e cliente UDP
			sTCP = tcp_server(mTCP);
			sUDP = udp_connect(regIP, regUDP, &resUDP);
			
			
			break;

		case 3:
			printf("There are 3 arguments\n");
			//Atribuições
			mIP = argv[1]; 
			mTCP = argv[2];
			
			//Criar sockets de servidor TCP e cliente UDP
			sTCP = tcp_server(mTCP);
			sUDP = udp_connect(REGIP, REGUDP, &resUDP);
			
			break;

		default:
			printf("The number of arguments inputed are invalid");
			exit(1);
			break;
}
	//cTCP = tcp_client(mIP,mTCP);
	while (1) {
		FD_ZERO(&filhas); //reset filhas
		FD_SET(0,&filhas);
		FD_SET(sTCP, &filhas); //filhas inicializado com stdin e sTCP
		
		if(succ.id > 0 || pred.id > 0){
			if(succ.id > 0){ //atualizar filhas com fd's adjacentes
				FD_SET(succ.fd,&filhas);
				maxfd = MAX(maxfd,succ.fd); 
			}
			if (pred.id != -1){
				FD_SET(pred.fd,&filhas);
				maxfd = MAX(maxfd,pred.fd);
			}
		}else{
			maxfd = sTCP;}
		
		errcode = select(maxfd+1 , &filhas, NULL, NULL, NULL);
		if ( errcode <= 0) exit(errno); // error
		
		
		
		if ((FD_ISSET(succ.fd,&filhas))&& succ.id!=-1){ //succ.fd mudou (e existe), entao read
			printf("Message received from server %s:%s\n", succ.ip, succ.port);
			n=read(succ.fd,tcp_clit,sizeof(tcp_clit)); //succ.fd = nossa TCP client conectada com o TCP server do succ
			if (n==0) //if conection with succ broken (succ left)
			{
				FD_CLR(succ.fd, &filhas);
				close(succ.fd);
				
				if (sucsuc.id != mid) //IF IM NOT ALONE
				{
					p_ = succ.id;
					succ.id = -1;
					succ.fd = tcp_client(sucsuc.ip, sucsuc.port);
					removeNodeCol(p_);
					succ.id = sucsuc.id;
					strcpy(succ.ip, sucsuc.ip);
					strcpy(succ.port, sucsuc.port);
					
					sprintf(trash,"PRED %d\n",mid);
					n=write(succ.fd,trash,strlen(trash));//I TELL NEXT GUY IM BEHIND HIM
					if(n==-1)/*error*/ exit(1);

					sprintf(trash,"SUCC %d %s %s\n",succ.id,succ.ip,succ.port);
            
					n=write(pred.fd,trash,strlen(trash));//I TELL MY PRED HIS NEW SUCCSUCC
					if(n==-1)/*error*/ exit(1);

					routall(succ.fd);

				}else{
					p_=succ.id;
					succ.id=-1;
					sucsuc.id = -1;
					removeNodeCol(p_);
				}
			
			}
			else{
				if(n==-1)/*error*/ exit(1);
				if((tok = strchr(tcp_clit, '\n')) == NULL){
					printf("%s - Message does not include <LF>\n",tcp_clit);
				}
				else{
					aux = tcp_clit;
				 	do{

						*(tok++) = '\0';
						n=what_clit(succ.fd,aux); //interpreta msg recebida
						if (n==0){
							printf("%s - message meaning identified\n",aux);
						}
						else{
							printf("%s - cannot identifie message meaning\n",aux);
						}
						aux = tok;
						tok = strchr(aux, '\n');
					}while(tok != NULL);
				}
			}
		}
		if ((FD_ISSET(pred.fd,&filhas))&& pred.id!=-1){ //same for pred
			printf("Message received from client %s:%s\n", pred.ip, pred.port);
			n=read(pred.fd,tcp_rec,sizeof(tcp_rec));
			if (n==0) //if conection with succ broken (succ left)
			{
				p_ = pred.id;
				pred.id = -1;
				FD_CLR(pred.fd, &filhas);
				close(pred.fd);
				removeNodeCol(p_);
			}
			else {
				if(n==-1)/*error*/ exit(1);
				if((tok = strchr(tcp_rec, '\n')) == NULL){
						printf("%s - Message does not include <LF>\n",tcp_rec);
					}
				else{
					aux = tcp_rec;
					do{
						*(tok++) = '\0';
						n=what_serv(pred.fd,aux); 
						if (n==0){
							printf("%s - message meaning identified\n",aux);
						}else{
							printf("%s - cannot identifie message meaning\n",aux);
						}
						aux = tok;
						tok = strchr(aux , '\n');

					}while(tok != NULL);
				}
		} }
		// Check for new connections
		if (FD_ISSET(sTCP, &filhas) ) {					
			
			addrlen = sizeof(addr);
			newfd = accept(sTCP, (struct sockaddr *)&addr, &addrlen); //newfd = Our TCP server socket connected to a client
			if (newfd == -1) exit(1); // error

			char clientIP[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(addr.sin_addr), clientIP, INET_ADDRSTRLEN); //where's the clit?
			printf("Message received from client %s:%d\n", clientIP, ntohs(addr.sin_port));
			n=read(newfd,tcp_rec,sizeof(tcp_rec));
			if(n==-1)/*error*/ exit(1);
			if((tok = strchr(tcp_rec, '\n')) == NULL){
				printf("%s - Message does not include <LF>\n",tcp_rec);
			}
			else{
				aux = tcp_rec;
				do{
					*(tok++) = '\0';
					n=what_serv(newfd, aux); //meu server TCP interpreta
					if (n==0){
						printf("%s - message meaning identified\n",aux);
					}else{
						printf("%s - cannot identifie message meaning\n",aux);
					}
					aux = tok;
					tok = strchr(aux, '\n');

				}while(tok!= NULL);
			}
		}
		
		if (FD_ISSET(0,&filhas)){ //escrevemos algo no terminal
			
			fgets(std_in,500,stdin);
			
			i=what_std(std_in,resUDP);//interpreta consola
			
		}
	}			
	freeaddrinfo(resUDP);
	close(sUDP);
}

