#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
	int fd;
	int maxread;
	char data[4096]="Hello World Good Morning how are you today I am fine thank you\n";
	size_t ret;
	char buf[4096]={0 };
	
	struct stat filestat;

	umask(0);
	// fd=open("file01", O_RDWR | O_CREAT , 0666);
	fd=open("file01", O_RDWR | O_CREAT | O_TRUNC, 0666);	
	if(fd == -1){
		perror("open a file to open failed");
		exit(1);
	}
	
	ret=write(fd, data, strlen(data));
	if(ret == -1){
		perror("write failed");
		exit(1);
	}
	printf("BYTE_WRITTEN=%ld\n", ret);
	close(fd);
	
	fd=open("file01", O_RDWR); 
	if(fd == -1){
		perror("open a file to read failed");
		exit(1);
	}
	
	if (fstat(fd, &filestat) == -1 ){
		fprintf(stderr, "Can't read file %s\n",  strerror(errno));
		exit(errno);
	}
	
	maxread=filestat.st_size;
	if(maxread >= 4096)
		maxread = 4096 - 1;

	while(1) {
		memset(buf, 0, maxread);
		ret=read(fd, buf, maxread);
		if(ret==0) 
			break;
		printf("BYTE_READ=%ld\n", ret);
		printf("%s", buf);
	}
	close(fd);
	return 0;
}