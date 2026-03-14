#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sfd, 5);

    printf("Server waiting...\n");

    int cfd = accept(sfd, NULL, NULL);

    char buf[1024];
    int n;

    printf("Receiving request...\n");

    while ((n = read(cfd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);
    }

    printf("\nClient finished sending\n");

    write(cfd, "Response from server\n", 21);

    close(cfd);
    close(sfd);
}