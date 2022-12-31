#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

void handler(int signo) {
	pid_t exit_pid;
	printf("\nhandler_pid [%d], handler_ppid [%d]\n", getpid(), getppid());
	exit_pid = waitpid(-1, NULL, WNOHANG);
	if(exit_pid == -1)
		perror("waitpid");
	else
		printf("Terminated child PID = %d\n", exit_pid);
}

int main(void) {
	pid_t pid;
	char *args1[] = {"01-1.mytask", "20", NULL};
	signal(SIGCHLD, handler);
	switch (pid=fork()) {
		case 0:				// child process
			printf("CHILD PROCESS : %d\n", getpid());
			char *args[] = {"01.mytask", "10", NULL};
			if(execv("./01.mytask", args) == -1)
				perror("execv");
			exit(100);
		default:
			if(execv("./01-1.mytask", args1) == -1)
				perror("execv");
			exit(100);
	}
	return 0;
}

