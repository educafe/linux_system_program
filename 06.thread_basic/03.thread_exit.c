#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>

void *thread_function(void *arg);

int main() {
	int status, i;
	pthread_t tid;
	void *thread_result;
	void * ret;

	status = pthread_create(&tid, NULL, thread_function, "hello thread");
	if(status !=0){
		perror("pthread_create");
		exit(1);
	}
	
	sleep(1);
	pthread_cancel(tid);
	pthread_join(tid, &ret);
	printf("RET = %ld\n", (intptr_t)ret);
	return 0;
}

void *thread_function(void *arg){
	int i;
	pid_t tpid, pid;
	int ret, unused;
#if 0
	ret = pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &unused);
#else
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif

	for(int i=1; i<=10; i++){
		printf("\tChild thread %d\n", i);
		for(int k=0; k<300000000; k++) {}
		if(i==7){
			pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, &unused);
		}
	}
	return (void *) 5;
}
