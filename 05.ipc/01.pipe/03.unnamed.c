#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define MSGSIZE	64

int main(void){
	int status;
	time_t timer1;
	int fd;
	int pd[2];
	
	pipe(pd);
	
	switch(fork()){
		case 0:					//child
			close(pd[0]);
			dup2(pd[1], 1);
			if((execl("./child", "child", (char *) 0)) == -1)
				perror("execl-child");
		default:				//parent
			switch(fork()){
				case 0:
					close(pd[1]);
					dup2(pd[0],0);
					if((execl("./parent", "parent",  (char *) 0)) == -1)
						perror("execl-parent");
				default:
					wait(&status);
			}
	}
	close(pd[0]);
	close(pd[1]);
	return 0;
}

