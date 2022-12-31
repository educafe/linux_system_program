#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void handler(int signo) {
	pid_t exit_pid;
	
	printf ("\n%s[%d] signal is caughted\n", sys_siglist[signo], signo);
	exit_pid = waitpid(-1, NULL, WNOHANG);
	printf("Terminated child PID = %d\n", exit_pid);
}

int main(void) {
	int i;
	time_t startTimec, startTimes, startTimep;
	signal(SIGCHLD, handler);
	
	switch(fork()){
		case -1:
			exit(1);
		case 0:
			printf("Child process[%d] started\n", getpid());
			for (startTimec = time(NULL); time(NULL) < startTimec + 8;){
				for(int i=0; i<100000000; i++);
				write(1, "+", 1);
			}
			exit(0);
		default:
			printf("Main process[%d] started\n", getpid());
			for (startTimep = time(NULL); time(NULL) < startTimep + 10;){
				for(int i=0; i<100000000; i++);
				write(1, "*", 1);
			}
	}
	printf("\n");
	return 0;
}