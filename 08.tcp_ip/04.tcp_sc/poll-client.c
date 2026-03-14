// tcp-client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 5000
#define BUF_SIZE    1024

int main(void) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUF_SIZE];

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(1);
    }

    printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);

    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;   // keyboard input
    fds[0].events = POLLIN;
    fds[1].fd = sockfd;         // socket
    fds[1].events = POLLIN;

    while (1) {
        int ret = poll(fds, 2, -1); // wait indefinitely
        if (ret < 0) {
            perror("poll");
            break;
        }

        // Check keyboard input
        if (fds[0].revents & POLLIN) {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                printf("EOF on stdin, exiting.\n");
                break;
            }
            send(sockfd, buffer, strlen(buffer), 0);
        }

        // Check server messages
        if (fds[1].revents & POLLIN) {
            int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
            if (n <= 0) {
                printf("Server closed connection.\n");
                break;
            }
            buffer[n] = '\0';
            printf("Server: %s", buffer);
        }
    }

    close(sockfd);
    return 0;
}
