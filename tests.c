#include "header.h"
#define PORT "59000"
int errcode;
int main(){
	int fd, newfd;
	char buff[128];
	fd = tcp_server(PORT);
	newfd = accept(fd, NULL, NULL);
	int c = newfd;
	while(1){

		read(c, buff, 128);
		printf("read: %s", buff);
	}

}
