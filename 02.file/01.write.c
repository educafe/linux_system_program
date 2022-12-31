#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int fd;
	int i, ret;
	char data[512]="Hello World good morning how are you\n";
	
	char *cmd;
	if(argc < 2){
		if ((cmd = strrchr(argv[0], '/')) == NULL)
			cmd = argv[0];
		else
			cmd++;
		fprintf(stderr, "Usafe : %s <filename>\n", cmd);
		exit(1);
	}
	
	// umask(0);
	if((fd=open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1){
	// if((fd=open(argv[1], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)) == -1) {
	// if((fd=open(argv[1], O_WRONLY|O_CREAT|O_APPEND, 0666)) == -1){
	// if((fd=open(argv[1], O_WRONLY|O_CREAT|O_EXCL, 0666)) == -1){
		perror("open");
		exit(1);
	}
	printf("FILE-POS : %ld\n", lseek(fd, 0, SEEK_CUR));
	if((ret=write(fd, data, strlen(data))) == -1){
		perror("write");
		close(fd);
		exit(2);
	}
	printf("FILE-POS : %ld\n", lseek(fd, 0, SEEK_CUR));
	if(fsync(fd) == -1){
		perror("fsync()");
	}
	close(fd);
	return 0;
}

/*  An optional third parameter of type mode_t is required if the O_CREAT flag is set */

