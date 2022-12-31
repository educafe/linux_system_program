#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void handler(int signo) {
	printf ("signal[%d]='%s' is caughted\n", signo, strsignal(signo));
	// exit(20);
}

void exit_handler(int exit_code, void *arg){
	printf("PID=%d is exited with CODE=%d and MSG=%s\n", getpid(), exit_code, (char *)arg);
}

int main(int argc, char *argv[]) {
	signal(SIGCHLD, handler);
	on_exit(exit_handler, "exit function ended");
	switch (fork()){
		case -1:
			perror("fork");
			exit(30);
		case 0:
			printf("Child PID = %ld, Parend PID = %d \n", (long) getpid(), getppid());
			if (argc == 2) {
				exit(atoi(argv[1]));
			}
			_exit(10);
		default:
			getchar();
			exit(150);
	}
}

