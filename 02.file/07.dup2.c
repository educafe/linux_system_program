#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int fd, newfd;
	char buf[128] = "This is a dup() demonstration\n";
	
	if((fd = open(argv[1], O_WRONLY |O_CREAT | O_TRUNC, 0644)) == -1){
		perror("open");
		exit(1);
	}
#if 0
	close(1);
	newfd=dup(fd);
	close(fd);
	printf("NEWFD=%d\n", newfd);
	fprintf(stdout, "This is the output to stdout\n");
	write(newfd, buf, strlen(buf));
#else
#if 1
	newfd=dup2(fd, 1);	//stdout redirected to fd 
	close(fd);
	printf("NEWFD=%d\n", newfd);
	fprintf(stdout, "This is the output to stdout\n");
	write(newfd, buf, strlen(buf));
#else
	newfd=dup2(fd, 1);	//stdout redirected to fd 
	close(fd);
	dprintf(newfd, "NEWFD=%d\n", newfd);
	dprintf(newfd, "This is the output to stdout\n");
	write(newfd, buf, strlen(buf));
#endif
#endif
	getchar();
	return 0;
}


/*
* It is good to check the order of output written to a file
*/

/*
* printf doesn't necessarily call write every time. Rather, printf buffers its output. 
* That is, it often stores its result in a memory buffer, 
* only calling write when the buffer is full, or on some other conditions.

* write is a fairly expensive call, 
* much more expensive than copying data into printf's buffer, 
* so reducing the number of write calls provides a net performance win.

* If your stdout is directed to a terminal device, 
* then printf calls write every time it sees a \n. 
* If your stdout is directed to a file (or to /dev/null), 
* then printf calls write only when its internal buffer is full.
*/