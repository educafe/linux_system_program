// #define _POSIX_C_SOURCE 200112L
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
  sigset_t set;
  int sig, ret;
  int *sigptr = &sig;
	
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigprocmask(SIG_BLOCK, &set, NULL);

  for(int i=0; i<5; i++){
		printf("Critical Region 1 is running --- %d\n", i);
		sleep(1);
	}
	printf("wait for a signal SIGINT ---------\n");
  ret = sigwait(&set,sigptr);
  if(ret == -1){
		perror("sigwait failed\n");
		exit(2);
	}
  if(sig == 2)
    for(int i=0; i<5; i++){
			printf("Critical Region 2 is running --- %d\n", i);
			sleep(1);
		}
  else
		printf("sigwait returned with sig: %d\n", sig);
}