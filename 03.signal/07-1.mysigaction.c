/*
 * SIGCHLD tutorial using sigaction with siginfo_t
 *
 * cat test.sh
 *  echo ">>>foo"
 *  echo PID=$$
 *  echo "<<<bar"
 *  exit 7
 *
 * gcc -Wall -o test_si test_si.c
 * ./test_si &
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

void handler(int signo, siginfo_t *si, void *uap){
  printf("\nsignal %d received with siginfo_t:\n", signo);
  printf("-->sig #: si_signo=%d\n", si->si_signo);
  printf("-->error #: si_errno=%d\n", si->si_errno);
  printf("-->sig code: si_code=%d\n", si->si_code);
  printf("-->sig value : si_value, sival_int=%d\n", si->si_value.sival_int);
  printf("-->sending pid: si_pid=%d\n", si->si_pid);
  printf("-->sending uid: si_uid=%d\n", si->si_uid);
  printf("-->exit status : si_status=%d\n", si->si_status);
  printf("\n");
}

int main(void){
  struct sigaction sa;
  int pid;

  memset(&sa, 0, sizeof(sa));
  sa.sa_sigaction = handler;
  // sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sa.sa_flags = SA_SIGINFO | SA_RESETHAND;

  sigaction(SIGINT, &sa, NULL);

  for(int i=1; ; i++){
		printf("signal test --- %d\n", i);
		sleep(2);
	}
  return 0;
}