#include "header.h"
#define IP "127.0.0.1"
#define PORT "59000"
int errcode;
int main(){
	int fd, newfd;
	char buff[128];
	fd = tcp_client(IP, PORT);

	while(1){
		write(fd, "Pilinha\n", sizeof("Pilinha\n"));
		read(0, buff, 128);
		printf("read: %s", buff);
	}

}
