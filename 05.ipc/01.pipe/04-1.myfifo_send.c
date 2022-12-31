#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
	int fd, i;
	char buf[128];
	time_t timer1;

	if((access ("/tmp/myfifo", F_OK)) != 0){
		if(mkfifo("/tmp/myfifo", 0666) == -1){
			perror("mkfifo");
			exit(1);
		}
	}
#if 0
	if((fd = open("/tmp/myfifo", O_WRONLY)) == -1){
		perror("open");
		exit(2);
	}
#else
if((fd = open("/tmp/myfifo", O_WRONLY | O_NONBLOCK)) == -1){
		perror("open");
		exit(2);
	}
#endif
	
	for(i=0; i<5; i++){
		memset(buf, 0, sizeof(buf));
		sprintf(&buf[0], "Hello(%d)\n", i);
		write(fd, &buf[0], strlen(buf));
		printf("Tx: %s", buf);
		for(timer1=time(NULL); time(NULL)<timer1 + 2;)
			continue;
	}
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "end\n");
	write(fd, buf, strlen(buf)+1);
	close(fd);
	unlink("/tmp/mkfifo");
	return 0;
}


/*
* In linux fifo(7) manpage:
* A process can open a FIFO in nonblocking mode. 
* In this case, opening for read-only succeeds even if 
* no one has opened on the write side yet 
* and opening for write-only fails with ENXIO (no such device or address) 
* unless the other end has already been opened
*/