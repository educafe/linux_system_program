#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE	4096

int main(int argc, char *argv[]){
	
	char buf[BUFSIZE];
	int fd1, fd2, byteRead, maxread;
	struct stat fileinfo;
	off_t filesize;
	
	if(argc != 3) {
		printf("Usage: %s <source filename> <destination filename>\n", argv[0]);
		exit(1);
	}
	
	if((fd1 = open(argv[1], O_RDONLY)) == -1){
		perror("file open1 error");
		exit(2);
	}
	
	if((fd2 = open(argv[2], O_WRONLY|O_CREAT|O_EXCL, 0664)) == -1){
		if(errno == EEXIST){
			printf("%s exists, do you want to overwrite it?\n", argv[2]);
			fd2 = open(argv[2], O_WRONLY|O_TRUNC, 0664);
		}else{
			perror("file open2 error");
			exit(2);
		}
	}
	
	fstat(fd1, &fileinfo);
	filesize=fileinfo.st_size;
	if(filesize >= BUFSIZE)
		maxread=BUFSIZE-1;
	else
		maxread=filesize;
	
	while (1){
		memset(buf, 0, sizeof(buf));
		byteRead = read(fd1, buf, maxread);
		if(byteRead == 0)
			break;
		if(write(fd2, buf, byteRead) != byteRead){
			perror("write()");
			break;
		}
	}
	close(fd1);
	close(fd2);
	return 0;
}
