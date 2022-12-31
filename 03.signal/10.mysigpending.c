#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "signalprint.h"

static void handler(int signo) { 
	printf("Signal #(%d) is caught\n", signo); 
}

int main( void){
	sigset_t sigset, oldset, pendingset;
	for(int i=1; i<=64; i++){
		signal(i, handler);
	}
	sigfillset(&sigset);
	sigprocmask(SIG_SETMASK, &sigset, &oldset);
	print_sigset_t(&oldset);
	int i=0;
	while(1){
		printf( "I am running --- %d\n", i++);
		sleep(1);
		if (sigpending(&pendingset) == 0){
			print_sigset_t(&pendingset);
			if (sigismember(&pendingset, SIGINT)){
				printf( "SIGINT was pended --- end of loop.\n");
				break;
			}
			if(sigismember(&pendingset, SIGTSTP)){
				printf( "SIGTSTP was pended --- end of loop.\n");
				break;
			}
		}
   }
	 sigprocmask(SIG_SETMASK, &oldset, NULL);
	 sleep(3);
   return 0;
}

