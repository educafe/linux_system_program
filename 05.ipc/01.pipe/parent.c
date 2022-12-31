#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MSGSIZE	1024

int main(){
	
	time_t timer1;
	char rbuf[MSGSIZE];
	int i, len;
	
	for(i=0; ; i++){
		memset(&rbuf[0], 0, MSGSIZE);
		len = read(0, rbuf, MSGSIZE);
		if(len == 0){
			break;
		}
		printf("%s\n", rbuf);
	}
	exit(0);
}

