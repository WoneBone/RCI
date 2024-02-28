#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_CLIENTS 10 // Maximum number of clients that can be handled simultaneously
#define MAX(a,b) ((a)>(b)?(a):(b))

int main(void) {
    struct addrinfo hints, *res;
    int listen_fd, client_fds[MAX_CLIENTS], errcode, maxfd, i;
    ssize_t n, nw;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char *ptr, buffer[128];
    fd_set rfds;

    memset(client_fds, -1, sizeof(client_fds)); // Initialize client_fds array to -1

    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) exit(1); // error

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;

    if ((errcode = getaddrinfo(NULL, "58000", &hints, &res)) != 0) exit(1); // error

    if (bind(listen_fd, res->ai_addr, res->ai_addrlen) == -1) exit(1); // error

    if (listen(listen_fd, 5) == -1) exit(1); // error

    while (1) {
        FD_ZERO(&rfds);
        FD_SET(0,&rfds);
        FD_SET(listen_fd, &rfds);
        maxfd = listen_fd;

        // Add active client_fds to rfds and update maxfd
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] > 0) {
                FD_SET(client_fds[i], &rfds);
                maxfd = MAX(maxfd, client_fds[i]);
            }
        }

        if (select(maxfd + 1, &rfds, NULL, NULL, NULL) <= 0) exit(1); // error

        // Check for new connections
        if (FD_ISSET(listen_fd, &rfds)) {
            addrlen = sizeof(addr);
            int newfd = accept(listen_fd, (struct sockaddr *)&addr, &addrlen);
            if (newfd == -1) exit(1); // error

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(addr.sin_addr), clientIP, INET_ADDRSTRLEN);
            printf("Connection established from %s:%d\n", clientIP, ntohs(addr.sin_port));

            // Find a slot for the new connection
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_fds[i] < 0) {
                    client_fds[i] = newfd;
                    break;
                }
            }

            if (i == MAX_CLIENTS) {
                printf("Max clients reached. Rejecting new connection from %s:%d\n", clientIP, ntohs(addr.sin_port));
                close(newfd);
            }
        }

        // Check each client for data
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] > 0 && FD_ISSET(client_fds[i], &rfds)) {
                n = read(client_fds[i], buffer, 128);
                if (n <= 0) { // error or connection closed
                    printf("Connection lost from client %d\n", i);
                    close(client_fds[i]);
                    client_fds[i] = -1; // Mark as available
                } else {
                    printf("Message from client %d: %s", i, buffer);
                    // Echo back the message
                    ptr = &buffer[0];
                    while (n > 0) {
                        if ((nw = write(client_fds[i], ptr, n)) <= 0) exit(1); // error
                        n -= nw;
                        ptr += nw;
                    }
                }
            }
        }
    }

    freeaddrinfo(res);
    close(listen_fd);
    exit(0);
}
