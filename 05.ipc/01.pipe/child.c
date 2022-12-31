#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MSGSIZE	64

int main(){
	
	time_t timer1;
	char sbuf[MSGSIZE];
	int i;
	
	for(i=0; i<5; i++){
		sprintf(&sbuf[0], "Hello, Parent --- I am child -- %d", i);
		write(1, sbuf, strlen(sbuf));
		for(timer1=time(NULL); time(NULL)<timer1 + 1;)
			continue;
	}
	return 0;
}

