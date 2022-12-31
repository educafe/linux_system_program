#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


static int restart=0;

static void handler(int signo, siginfo_t *si, void *uap){
  printf("\nsignal %d received with siginfo_t:\n", signo);
  printf("-->sig #: si_signo=%d\n", si->si_signo);
  printf("-->error #: si_errno=%d\n", si->si_errno);
  printf("-->sig code: si_code=%d\n", si->si_code);
  printf("-->sig value : si_value, sival_int=%d\n", si->si_value.sival_int);
  printf("-->sending pid: si_pid=%d\n", si->si_pid);
  printf("-->sending uid: si_uid=%d\n", si->si_uid);
  printf("-->exit status : si_status=%d\n", si->si_status);
  printf("\n");

  waitpid(si->si_pid, NULL, 0);
	restart=10;
}

int main(void){
  struct sigaction sa;
  int pid;

  memset(&sa, 0, sizeof(sa));
  sa.sa_sigaction = handler;
  sa.sa_flags = SA_SIGINFO | SA_RESTART;

  sigaction(SIGCHLD, &sa, NULL);

again:
  switch(pid = fork()) {
    case -1: 
			exit(-1); 																					// error
    case 0: 																							/* children */
      sleep(4);
      execlp("./task.sh", "nothing", NULL);
    default: 																							/* parent */
			for(restart=0; restart<5 ; restart++){
				printf("waiting for child to be terminated --- %d\n", restart);
				for(int i=0; i<2000000000; i++);
				// sleep(10);
				if(restart>5)
					break;
			}
			goto again;
      return 0;
  }
  return 0;
}