#include "header.h"

#define REGIP "193.136.138.142"
#define REGUDP "59000"
#define MAX_CLIENTS 17
#define MAX(a,b) ((a)>(b)?(a):(b))
int errcode,mid;
char *mIP;
char *mTCP;
struct node succ, sucsuc, pred;



int main(int argc, char *argv[]){
	char buffer[10000],std_in[500],tcp_rec[500],tcp_clit[500];
	char *regIP;
	char *regUDP;
	succ.id=-1;
	sucsuc.id=-1;
	pred.id=-1;
	int sTCP, sUDP, fd_ret, cTCP,maxfd,client_fds[MAX_CLIENTS],i,newfd;
	ssize_t n,nw;
	socklen_t addrlen;
	fd_set filhas;
	FD_ZERO(&filhas);
	mid=-1;

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
        		FD_ZERO(&filhas);
				FD_SET(0,&filhas);
				FD_SET(sTCP, &filhas);
				
				if(succ.id!=-1){
					FD_SET(succ.fd,&filhas);
				}
				
				maxfd = sTCP;

				if (select(maxfd+1 , &filhas, NULL, NULL, NULL) <= 0) exit(1); // error
				
				
				if (FD_ISSET(0,&filhas)){
					
					fgets(std_in,500,stdin);
					
					i=what_std(std_in,resUDP);
					
				}
				if (FD_ISSET(succ.fd,&filhas)){
					printf("Message received from %s:%s\n", succ.ip, succ.port);
					n=read(succ.fd,tcp_clit,sizeof(tcp_clit));
					if(n==-1)/*error*/ exit(1);
					n=what_serv(succ.fd,tcp_clit);
					if (n==0){
						printf("%s - message meaning identified",tcp_rec);
					}else{
						printf("%s - cannot identifie message meaning",tcp_rec);
					}
				}
				
				// Check for new connections
				if (FD_ISSET(sTCP, &filhas)) {					
					
					addrlen = sizeof(addr);
					newfd = accept(sTCP, (struct sockaddr *)&addr, &addrlen);
					if (newfd == -1) exit(1); // error

					char clientIP[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &(addr.sin_addr), clientIP, INET_ADDRSTRLEN);
					printf("Message received from %s:%d\n", clientIP, ntohs(addr.sin_port));
					n=read(newfd,tcp_rec,sizeof(tcp_rec));
					if(n==-1)/*error*/ exit(1);

					n=what_serv(newfd,tcp_rec);

					if (n==0){
						printf("%s - message meaning identified",tcp_rec);
					}else{
						printf("%s - cannot identifie message meaning",tcp_rec);
					}
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

