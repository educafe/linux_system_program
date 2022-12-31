#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE	4096

int main() {
	FILE *fp;
	int fd;
	int maxread;
	char data[SIZE]="Hello World Good Morning how are you today I am fine thank you\n";
	size_t ret;
	char buf[SIZE]; 
	struct stat fileinfo;
	off_t filesize;
	
	if((fp=fopen("file01", "w+"))==NULL) {
		perror("fopen");
		exit(1);
	}
	if((ret=fwrite(data, sizeof(char), strlen(data), fp)) == -1) {
		perror("fwrite");
		fclose(fp);
		exit(2);
	}
	
	printf("BYTE_WRITTEN=%ld\n", ret);
	fclose(fp);

	
#if 1
	if((fp = fopen("file01", "r")) == NULL){
		perror("fopen");
		exit(1);
	}
	
	stat("file01", &fileinfo);
	filesize=fileinfo.st_size;
	if(filesize >=SIZE)
		maxread=filesize - 1;
	else
		maxread=filesize;
	
	while(1) {
		memset(buf, 0, sizeof(buf));
		ret=fread(buf, sizeof(char), maxread, fp);
		if (feof(fp))
			break;
		printf("BYTE_READ=%ld\n", ret);
		puts(buf);
	}
	fclose(fp);
#else
	fd=open("file01", O_RDWR, 0666);

	fstat(fd, &fileinfo);
	filesize=fileinfo.st_size;
	
	while(1) {
		memset(buf, 0, sizeof(buf));
		ret=read(fd, buf, maxread);
		if(ret==0) {
			break;
		}
		printf("BYTE_READ=%ld\n", ret);
		puts(buf);
	}
	close(fd);
#endif
	return 0;
}