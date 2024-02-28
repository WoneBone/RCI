#include "header.h"

#define REGIP "193.136.138.142"
#define REGUDP "59000"
#define MAX_CLIENTS 17
#define MAX(a,b) ((a)>(b)?(a):(b))
int errcode;
char *mIP;
char *mTCP;

struct node succ, sucsuc, pred;



int main(int argc, char *argv[]){
	char buffer[10000],std_in[500];
	char *regIP;
	char *regUDP;
	int sTCP, sUDP, fd_ret, cTCP,maxfd,client_fds[MAX_CLIENTS],i;
	ssize_t n;
	socklen_t addrlen;
	fd_set filhas;
	FD_ZERO(&filhas);
	

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
				maxfd = sTCP;

				// Add active client_fds to filhas and update maxfd
				for (i = 0; i < MAX_CLIENTS; i++) {
					if (client_fds[i] > 0) {
						FD_SET(client_fds[i], &filhas);
						maxfd = MAX(maxfd, client_fds[i]);
					}
				}

				if (select(maxfd + 1, &filhas, NULL, NULL, NULL) <= 0) exit(1); // error
				
				
				if (FD_ISSET(0,&filhas)){
					fgets(std_in,500,stdin);
					
				}
				
				// Check for new connections
				if (FD_ISSET(sTCP, &filhas)) {
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

