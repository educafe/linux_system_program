// ============================
// epoll-chat-client.c
// A minimal chat client using epoll to multiplex stdin + socket
// Type lines and see broadcasts from the server & other clients
// ============================

#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 5000
#define MAX_EVENTS  4
#define BUF_SIZE    4096

static int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) 
		return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { 
		perror("socket"); 
		return 1; 
	}

    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv.sin_addr) != 1) { 
		perror("inet_pton"); 
		return 1; 
	}

    if (connect(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("connect");
        return 1;
    }

    set_nonblocking(sockfd);
    set_nonblocking(STDIN_FILENO);

    int epfd = epoll_create1(0);
    if (epfd < 0) { 
		perror("epoll_create1"); 
		return 1; 
	}

    struct epoll_event ev, events[MAX_EVENTS];

    ev.events = EPOLLIN | EPOLLRDHUP; ev.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) < 0) { 
		perror("epoll_ctl sock"); 
		return 1; 
	}

    ev.events = EPOLLIN; ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) < 0) { 
		perror("epoll_ctl stdin"); 
		return 1; 
	}

    printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);
    printf("Type messages and press Enter. Ctrl+C to quit.\n");

    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }
        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;
            uint32_t re = events[i].events;
            if (fd == sockfd) {
                if (re & (EPOLLHUP | EPOLLRDHUP | EPOLLERR)) {
                    printf("Server closed connection.\n");
                    goto out;
                }
                if (re & EPOLLIN) {
                    char buf[BUF_SIZE];
                    while (1) {
                        ssize_t rn = recv(sockfd, buf, sizeof(buf), 0);
                        if (rn > 0) {
                            fwrite(buf, 1, (size_t)rn, stdout);
                            fflush(stdout);
                        } else if (rn == 0) {
                            printf("\n[EOF from server]\n");
                            goto out;
                        } else {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                            perror("recv");
                            goto out;
                        }
                    }
                }
            } else if (fd == STDIN_FILENO) {
                if (re & EPOLLIN) {
                    char line[BUF_SIZE];
                    ssize_t rn = read(STDIN_FILENO, line, sizeof(line));
                    if (rn <= 0) {
                        // stdin closed; just exit
                        goto out;
                    }
                    // naive send (best-effort); for big outputs, you would loop until all bytes sent
                    ssize_t off = 0;
                    while (off < rn) {
                        ssize_t sn = send(sockfd, line + off, (size_t)(rn - off), 0);
                        if (sn > 0) 
							off += sn;
                        else if (sn < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                            // socket not ready for write; drop remainder to keep demo simple
                            break;
                        } else if (sn < 0) {
                            perror("send");
                            goto out;
                        }
                    }
                }
            }
        }
    }

	out:
		close(sockfd);
		close(epfd);
    return 0;
}
