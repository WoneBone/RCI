#include "header.h"

#define MAX_CLIENTS 17
#define MAX(a,b) ((a)>(b)?(a):(b))
int errcode,mid;
int mRing;
char *mIP;
char *mTCP;
struct node succ, sucsuc, pred;



int main(int argc, char *argv[]){
	char std_in[500],tcp_rec[500],tcp_clit[500];
	char *regIP;
	char *regUDP;
	succ.id=-1;
	sucsuc.id=-1;
	pred.id=-1;
	int sTCP, sUDP, fd_ret, cTCP,maxfd=0,client_fds[MAX_CLIENTS],i,newfd;
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
			
			//cTCP = tcp_client(mIP,mTCP);
			while (1) {
        		FD_ZERO(&filhas); //reset filhas
				FD_SET(0,&filhas);
				FD_SET(sTCP, &filhas); //filhas inicializado com stdin e sTCP
	
				if(succ.id!=-1){ //atualizar filhas com fd's adjacentes
					FD_SET(pred.fd,&filhas);
					maxfd = MAX(maxfd,pred.fd);
					FD_SET(succ.fd,&filhas);
					maxfd = MAX(maxfd,succ.fd); 
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
						succ.id = -1;
					
						
						
					}
					else{
						if(n==-1)/*error*/ exit(1);
						n=what_clit(succ.fd,tcp_clit); //interpreta msg recebida
						if (n==0){
							printf("%s - message meaning identified\n",tcp_clit);
						}
						else{
							printf("%s - cannot identifie message meaning\n",tcp_clit);
						}
					}
				}
				if ((FD_ISSET(pred.fd,&filhas))&& pred.id!=-1){ //same for pred
					printf("Message received from client %s:%s\n", pred.ip, pred.port);
					n=read(pred.fd,tcp_rec,sizeof(tcp_rec));
					if (n==0) //if conection with succ broken (succ left)
					{
						FD_CLR(pred.fd, &filhas);
						close(pred.fd);
						pred.id = -1;
					
						
						
					}
					else {
					if(n==-1)/*error*/ exit(1);
					n=what_clit(pred.fd,tcp_rec); //DUVIDA!! NAO E SUPOSTO ISTO SER WHAT SERV????
					if (n==0){
						printf("%s - message meaning identified\n",tcp_rec);
					}else{
						printf("%s - cannot identifie message meaning\n",tcp_rec);
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

					n=what_serv(newfd,tcp_rec); //meu server TCP interpreta

					if (n==0){
						printf("%s - message meaning identified\n",tcp_rec);
					}else{
						printf("%s - cannot identifie message meaning\n",tcp_rec);
					}
				}
				
				if (FD_ISSET(0,&filhas)){ //escrevemos algo no terminal
					
					fgets(std_in,500,stdin);
					
					i=what_std(std_in,resUDP);//interpreta consola
					
				}
			}
			
		

			
			join(1, 68, resUDP);
			freeaddrinfo(resUDP);
			close(sUDP);

			break;

		default:
			printf("The number of arguments inputed are invalid");
			exit(1);
			break;
	
}
}

