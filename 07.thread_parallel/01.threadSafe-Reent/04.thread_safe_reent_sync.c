#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>

typedef struct {
	int x;
	int y;
} thread_args;

void swap(int *x, int *y) {
	int t = *x;
	*x = *y;

	for(int i=0; i<10000000; i++) ;
/* while performing for loop, there might be an task switching between threads */
	*y = t;
}

pthread_t	tid1, tid2;	/* thread ID */
pthread_mutex_t mutex_id;

void *thread(void *_arg) {
	pthread_detach(pthread_self());
	thread_args *args = (thread_args *)_arg;
	pthread_mutex_lock(&mutex_id);
	args->x=100;
	args->y=200;
	swap(&args->x, &args->y);
	printf("A=%d, B=%d\n", *(&args->x), *(&args->y));
	pthread_mutex_unlock(&mutex_id);
	return NULL;
}

void *thread2(void *_arg) {
	pthread_detach(pthread_self());
	thread_args *args = (thread_args *)_arg;
	
	pthread_mutex_lock(&mutex_id);
	args->x=1000;
	args->y=2000;
	swap(&args->x, &args->y);
	printf("M=%d, N=%d\n", *(&args->x), *(&args->y));
	pthread_mutex_unlock(&mutex_id);
	return NULL;
}

int main() {
	int x=10, y=20;
	thread_args arg;
	
	if(pthread_mutex_init(&mutex_id, NULL) != 0){
		perror("pthread_mutex_init");
		exit(errno);
	}
	
	if (pthread_create(&tid1, NULL, thread, &arg) != 0) {
		exit(1);
	}
	
	for(int i=0; i<1000000; i++) ;
	
	if (pthread_create(&tid2, NULL, thread2, &arg) != 0) {
		exit(1);
	}
	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	swap(&x, &y);
	printf("X=%d, Y=%d\n", *(&x), *(&y));
	
    pthread_mutex_destroy(&mutex_id);
    // return 0;
	pthread_exit(NULL);
}