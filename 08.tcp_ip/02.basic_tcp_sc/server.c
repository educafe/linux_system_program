#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	int s_sfd, c_sfd;
	struct sockaddr_in s_sock;
	struct sockaddr_in c_sock;
	int sock_size;
	char buf[1024];
	
	s_sfd=socket(AF_INET, SOCK_STREAM, 0);
	
	s_sock.sin_family=AF_INET;
	s_sock.sin_port=htons(atoi(argv[1]));
	s_sock.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if(bind(s_sfd, (struct sockaddr *)&s_sock, sizeof(s_sock)) == -1){
		perror("bind");
		exit(10);
	}
	
	listen(s_sfd, 5);
	
	sock_size=sizeof(c_sock);
	c_sfd=accept(s_sfd, (struct sockaddr *) &c_sock, &sock_size);
	
	memset(buf, 0, sizeof(buf));
	// recv(c_sfd, buf, sizeof(buf), 0);
	send(c_sfd, "hello client", strlen("hello client"), 0);
	// write(c_sfd, "Hello client", strlen("Hello client"));
	printf("%s\n", buf);
	 
	close(c_sfd);
	return 0;
}