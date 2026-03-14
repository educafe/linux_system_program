#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/sctp.h>

#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int listenSock, connSock, ret;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];
    struct sctp_sndrcvinfo sndrcvinfo;
    int flags;

    listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (listenSock == -1) {
        perror("socket");
        exit(1);
    }

    bzero((void *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    ret = bind(listenSock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret == -1) {
        perror("bind");
        exit(1);
    }

    ret = listen(listenSock, 5);
    if (ret == -1) {
        perror("listen");
        exit(1);
    }

    printf("SCTP server listening...\n");

    while (1) {
        bzero(buffer, sizeof(buffer));
        flags = 0;
        connSock = accept(listenSock, (struct sockaddr *)NULL, (int *)NULL);
        if (connSock < 0) {
            perror("accept");
            continue;
        }

        ret = sctp_recvmsg(connSock, (void *)buffer, sizeof(buffer),
                           (struct sockaddr *)NULL, 0, &sndrcvinfo, &flags);
        if (ret > 0) {
            printf("Received message: %s\n", buffer);
        }

        close(connSock);
    }

    return 0;
}
