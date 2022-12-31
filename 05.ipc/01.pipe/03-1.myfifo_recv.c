#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int fd;
	char buf[128];
	int count = 0;
	
	if((access ("/tmp/myfifo", F_OK)) != 0){
		if(mkfifo("/tmp/myfifo", S_IRUSR | S_IWUSR) == -1){
			perror("mkfifo");
			exit(1);
		}
	}
#if 0
	if((fd = open("/tmp/myfifo", O_RDONLY)) == -1){
		perror("open");
		exit(1);
	}
#else
	if((fd = open("/tmp/myfifo", O_RDONLY | O_NONBLOCK)) == -1){
		perror("open");
		exit(1);
	}
#endif
	
	while(1){
		memset(buf, 0, sizeof(buf));
		read(fd, buf, sizeof(buf));
		printf("Rx - %s", buf);
		fflush(stdout);
		if(strstr(buf, "end")){
			break;
		}
		sleep(1);
	}
	close(fd);
	unlink("/tmp/myfifo");
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