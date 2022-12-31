#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>

void handler(int signo) {
	pid_t exit_pid;
	printf("\nhandler_pid [%d], handler_ppid [%d]\n", getpid(), getppid());
	exit_pid = waitpid(-1, NULL, WNOHANG);
	if(exit_pid == -1)
		// printf("waitpid error with ERRNO=%d meaning %s\n", errno,  strerror(errno));
		perror("waitpid");
	else
		printf("Terminated child PID = %d\n", exit_pid);
}

int main() {
	pid_t pid1, pid2;
	time_t startTimec, startTimes, startTimep;
	signal(SIGCHLD, handler);
	switch(pid1 = fork()){
		case 0: 
			switch(pid2=fork()){
				case 0:
					printf("grand-child pid [%d] of ppid [%d] started\n", getpid(), getppid());
					// for (startTimec = time(NULL); time(NULL) < startTimec + 8;){
						// for(int i=0; i<100000000; i++);
						// write(1, "+", 1);
					// }
					sleep(5);
					exit(0);
				default:
					printf("child pid [%d] of ppid [%d] started\n", getpid(), getppid());
					// for (startTimes = time(NULL); time(NULL) < startTimes + 6;){
						// for(int i=0; i<100000000; i++);
						// write(1, "#", 1);
					// }
					sleep(10);
					exit(0);
			}
		default: 
			printf("main pid [%d] of ppid [%d] started\n", getpid(), getppid());
			for (startTimep = time(NULL); time(NULL) < startTimep + 13;){
					for(int i=0; i<100000000; i++);
					write(1, "*", 1);
			}
			printf("\n");
	} 
	return 0;
}    	
     	
			
     	
     	
     	
