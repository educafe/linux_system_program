#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void* udp_server(void *arg);
void change_port(int *port);

int main() {
    srand(time(NULL)); // Seed random number generator

    int port = 5000 + rand() % 1000; // Initial port number between 5000 and 5999
    pthread_t thread_id;

    while (1) {
        printf("Starting server on port %d\n", port);
        pthread_create(&thread_id, NULL, udp_server, &port);
        
        sleep(10); // Change port every 10 seconds
        
        // Attempt to gracefully shut down the running server thread
        pthread_cancel(thread_id);
        pthread_join(thread_id, NULL);
        
        change_port(&port); // Change the port number for the next iteration
    }

    return 0;
}

void* udp_server(void *arg) {
    int port = *((int*)arg);
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        char buffer[BUF_SIZE];
        unsigned int len = sizeof(cliaddr);
        
        int n = recvfrom(sockfd, (char *)buffer, BUF_SIZE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';
        printf("Client : %s\n", buffer);
    }

    close(sockfd);
    return NULL;
}

void change_port(int *port) {
    *port = 5000 + rand() % 1000; // Change to a new port number between 5000 and 5999
}
