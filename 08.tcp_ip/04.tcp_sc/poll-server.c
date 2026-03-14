#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define PORT 5000
#define MAX_CLIENTS 10

int main() {
    int listen_fd, conn_fd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cli_len;
    char buffer[1024];
    struct pollfd fds[MAX_CLIENTS+1];
    int nfds = 1;

    // Create listening socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listen_fd, 5);

    // Set up poll for listening socket
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;

    printf("Server running on port %d...\n", PORT);

    while (1) {
        int ret = poll(fds, nfds, -1);  // wait forever
        if (ret < 0) { perror("poll"); exit(1); }

        // Check listening socket
        if (fds[0].revents & POLLIN) {
            cli_len = sizeof(cliaddr);
            conn_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &cli_len);
            printf("New client connected: fd=%d\n", conn_fd);

            fds[nfds].fd = conn_fd;
            fds[nfds].events = POLLIN;
            nfds++;
        }

        // Check client sockets
        for (int i = 1; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                int n = read(fds[i].fd, buffer, sizeof(buffer));
                if (n <= 0) {
                    printf("Client disconnected: fd=%d\n", fds[i].fd);
                    close(fds[i].fd);
                    fds[i] = fds[nfds-1];  // compact array
                    nfds--;
                    i--;  // recheck moved entry
                } else {
                    buffer[n] = '\0';
                    printf("Client %d says: %s\n", fds[i].fd, buffer);
                    write(fds[i].fd, buffer, n); // echo
                }
            }
        }
    }
}
