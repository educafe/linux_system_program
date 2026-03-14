#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	int fd, fd1;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char buf[1024];
	
	fd=socket(PF_INET, SOCK_STREAM, 0);
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	
	connect(fd, (struct sockaddr *)&server, sizeof(server));
	memset(buf, 0, sizeof(buf));
	// send(fd, "Hello server", strlen("Hello server"), 0);
	recv(fd, buf, sizeof(buf), 0);
	printf("RECV : %s\n", buf);
	close(fd);
	return 0;
}