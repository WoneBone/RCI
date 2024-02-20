#include "header.h"

int tcp_connect(char *TCP){
	int fd, n;
	struct addrinfo hints,*res;	


	fd=socket(AF_INET,SOCK_STREAM,0);
	if (fd==-1) exit(1); //error
	
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_STREAM; //TCP socket
	hints.ai_flags=AI_PASSIVE;

	errcode=getaddrinfo(NULL,TCP,&hints,&res);
	if((errcode)!=0)/*error*/
		exit(1);

	n=bind(fd,res->ai_addr,res->ai_addrlen);
	if(n==-1) /*error*/ 
		exit(2);
	if(listen(fd,5)==-1)/*error*/
		exit(3);
	
	freeaddrinfo(res);

	return fd;	
}

int udp_connect(char *regIP, char *regUDP, struct addrinfo **res){
	int fd;
	struct addrinfo hints;	

	fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
	if(fd==-1) /*error*/exit(1);
	
	memset(&hints,0,sizeof hints);
	hints.ai_family=AF_INET; //IPv4
	hints.ai_socktype=SOCK_DGRAM; //UDP socket

	errcode=getaddrinfo(regIP, regUDP, &hints, res);
	if(errcode!=0) /*error*/ exit(1);
	
	return fd;
	
}
