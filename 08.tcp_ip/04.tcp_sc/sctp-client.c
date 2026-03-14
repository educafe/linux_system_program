// sctp-client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#define SERVER_IP   "127.0.0.1"   // change if server is remote
#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

int main(void) {
    int sock;
    struct sockaddr_in servaddr;
    const char *msg = "Hello, SCTP!";
    uint16_t stream = 0;          // use stream 0

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) != 1) {
        perror("inet_pton");
        close(sock);
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("connect");
        close(sock);
        exit(1);
    }

    for (int i = 0; i < 100; i++) {
        ssize_t n = sctp_sendmsg(
            sock,                // connected socket
            msg, strlen(msg),    // payload
            NULL, 0,             // no addr for connected socket
            0, 0,                // ppid, flags
            stream,              // stream number
            0, 0                 // timetolive, context
        );
        if (n < 0) {
            perror("sctp_sendmsg");
        } else {
            printf("Message sent (%zd bytes): %s\n", n, msg);
        }
        sleep(1);
    }

    close(sock);
    return 0;
}
