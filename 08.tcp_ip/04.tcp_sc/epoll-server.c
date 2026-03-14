// ============================
// epoll-chat-server.c
// A minimal multi-client chat server using epoll (level-triggered)
// - Accepts many TCP clients
// - Broadcasts any line received from one client to all others
// - Displays all messages received and broadcast
// ============================

#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT 5000
#define MAX_EVENTS  64
#define MAX_CLIENTS 1024
#define BUF_SIZE    4096

static volatile int keep_running = 1;

static void on_sigint(int sig) {
    (void)sig;
    keep_running = 0;
}

static int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static void broadcast(int sender_fd, int *clients, int nclients, const char *data, size_t len) {
    for (int i = 0; i < nclients; i++) {
        int fd = clients[i];
        if (fd <= 0 || fd == sender_fd) continue;
        ssize_t sent = send(fd, data, len, 0);
        if (sent < 0) {
            if (errno == EPIPE || errno == ECONNRESET) {
                // ignore, client will be cleaned up later
            }
        }
    }
}

int main(void) {
    signal(SIGINT, on_sigint);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { 
		perror("socket"); 
		exit(1); 
	}

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#ifdef SO_REUSEPORT
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
#endif

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) { 
		perror("bind"); 
		exit(1); 
	}
    if (listen(listen_fd, SOMAXCONN) < 0) { 
		perror("listen"); 
		exit(1); 
	}
    if (set_nonblocking(listen_fd) < 0) { 
		perror("nonblock listen"); 
		exit(1); 
	}

    int epfd = epoll_create1(0);
    if (epfd < 0) { 
		perror("epoll_create1"); 
		exit(1); 
	}

    struct epoll_event ev;
    ev.events = EPOLLIN; // level-triggered read on listening socket
    ev.data.fd = listen_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) { 
		perror("epoll_ctl listen"); 
		exit(1); 
	}

    struct epoll_event events[MAX_EVENTS];

    int clients[MAX_CLIENTS];
    int nclients = 0;
    memset(clients, 0, sizeof(clients));

    printf("epoll chat server listening on port %d\n", SERVER_PORT);

    while (keep_running) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, 1000);
        if (n < 0) {
            if (errno == EINTR) 
				continue;
            perror("epoll_wait");
            break;
        }
        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;
            uint32_t re = events[i].events;

            if (fd == listen_fd) {
                // accept all pending
                while (1) {
                    struct sockaddr_in cli; socklen_t clilen = sizeof(cli);
                    int cfd = accept(listen_fd, (struct sockaddr *)&cli, &clilen);
                    if (cfd < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) 
							break;
                        perror("accept"); 
						break;
                    }
                    set_nonblocking(cfd);
                    struct epoll_event cev = {0};
                    cev.events = EPOLLIN | EPOLLRDHUP;
                    cev.data.fd = cfd;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &cev) < 0) {
                        perror("epoll_ctl add client");
                        close(cfd);
                        continue;
                    }
                    if (nclients < MAX_CLIENTS) {
                        clients[nclients++] = cfd;
                    } else {
                        const char *full = "Server full\n";
                        send(cfd, full, strlen(full), 0);
                        close(cfd);
                    }
                    char ip[64];
                    inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof(ip));
                    printf("+ client %s:%d (fd=%d)\n", ip, ntohs(cli.sin_port), cfd);
                }
            } else {
                if (re & (EPOLLHUP | EPOLLRDHUP | EPOLLERR)) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                    for (int k = 0; k < nclients; k++) if (clients[k] == fd) { clients[k] = clients[nclients-1]; clients[nclients-1] = 0; nclients--; break; }
                    printf("- client fd=%d\n", fd);
                    continue;
                }
                if (re & EPOLLIN) {
                    char buf[BUF_SIZE];
                    while (1) {
                        ssize_t rn = recv(fd, buf, sizeof(buf)-1, 0);
                        if (rn > 0) {
                            buf[rn] = '\0';
                            printf("[fd=%d] %s", fd, buf); // show incoming message
                            broadcast(fd, clients, nclients, buf, (size_t)rn);
                        } else if (rn == 0) {
                            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                            close(fd);
                            for (int k = 0; k < nclients; k++) if (clients[k] == fd) { 
								clients[k] = clients[nclients-1]; 
								clients[nclients-1] = 0; 
								nclients--; 
								break; }
                            printf("- client fd=%d (EOF)\n", fd);
                            break;
                        } else {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) 
								break;
                            perror("recv");
                            break;
                        }
                    }
                }
            }
        }
    }

    close(listen_fd);
    for (int i = 0; i < nclients; i++) if (clients[i] > 0) close(clients[i]);
    close(epfd);
    printf("server exited\n");
    return 0;
}
