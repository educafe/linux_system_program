#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>

void handler(int signo) {
	pid_t exit_pid;
	fprintf(stderr, "\nhandler_pid [%d], handler_ppid [%d]\n", getpid(), getppid());
	exit_pid = waitpid(-1, NULL, WNOHANG);
	if(exit_pid == -1)
		perror("waitpid");
	else
		fprintf(stderr, "Terminated child PID = %d\n", exit_pid);
}

int main(int argc, char *argv[]) {
	int i, fd;
	signal(SIGCHLD, handler);
	
	printf("%s's PID[%d], PPID[%d]\n", argv[0], getpid(), getppid());
	if((fd = open("/dev/null", O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1){
		perror("open");
		exit(1);
	}
#if 1
	dup2(fd, 1);
#endif
	for(i=1; i<=atoi(argv[1]); i++) {
		printf("MyTask_%d!!\n", i);
		sleep(1);
	}
	return 0;
}

