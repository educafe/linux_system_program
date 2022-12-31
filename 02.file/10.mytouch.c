#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <utime.h>
#include <libgen.h>

int main(int argc, char *argv[]){

	if (argc != 2){
		fprintf(stderr, "Usage : %s <filename>\n", basename(argv[0]));
		return 1;
	}

	if (utime(argv[1], NULL) == -1 ){
		if (errno == ENOENT) {
			if (creat(argv[1], 00666) == -1){
				perror("Can't create file");
				return errno;
			}
		}
		else {
			perror("utime");
			return errno;
		}
	}
	return 0;
}
