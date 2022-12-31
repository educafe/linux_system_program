#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(void) {
	pid_t pid;
	int status;
	switch (pid=fork()) {
		case -1:
			perror("fork failed");
			break;
		case 0:				// child process
			printf("CHILD PROCESS : %d\n", getpid());
			if(execl("./01.mytask", "01.mytask", "10", (char *) 0) == -1)
				perror("execl");
		default:
			sleep(1);
			system("ps -lf");
			pid = wait(&status);
			printf("parent : waited (%d) and completed!\n", pid);
			exit(0);
	}
	return 0;
}

