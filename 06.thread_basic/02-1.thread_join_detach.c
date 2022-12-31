#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

void *thread_run() {
	pthread_exit(0);
}

int main () {
  pthread_t thread;
  int ret;
  long count = 0;
  while(1) {
    if(ret = pthread_create(&thread, 0, thread_run, 0)) {
      printf("ERROR, ret is %d, so far %ld threads created\n", ret, count);
      perror("pthread_create :");
      return 1;
    }
#if 0
		if(ret = pthread_detach(thread)) {
      perror("pthread_detach :");
      return 1;
    }
#endif
    count++;
		// printf(".");
		// fflush(stdout);
		// sleep(1);
  }
  return 0;
}