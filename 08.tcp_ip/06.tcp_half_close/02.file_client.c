#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(fd, (struct sockaddr *)&addr, sizeof(addr));

    write(fd, "Hello server\n", 13);
    write(fd, "This is client\n", 15);

	getchar();
    /* Half close: stop sending */
    shutdown(fd, SHUT_WR);

    printf("Client has sent EOF\n");

    char buf[1024];
    int n;

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);
    }

    close(fd);
}