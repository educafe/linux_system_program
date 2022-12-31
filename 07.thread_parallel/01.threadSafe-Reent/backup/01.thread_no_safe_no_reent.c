#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

time_t startTimec, startTimep;
int tmp;
int add10(int a) {
	tmp = a;
	for (startTimec = time(NULL); time(NULL) < startTimec + 2;){
		for(int i=0; i<100000000; i++);
		write(1, "+", 1);
	}
	return tmp + 10; 
}

pthread_t	tid1, tid2;	/* thread ID */
void *thread1(void *arg) {
	pthread_detach(pthread_self());
	for(int i=0; i<5; i++){
		int ret=add10(10);
		printf("RET10=%d\n", ret);
		printf("TMP10=%d\n", tmp);
		for(int j=0; j<10000000; j++) ;
	}
	return NULL;
}

void *thread2(void *arg) {
	pthread_detach(pthread_self());
	int m=1000, n=2000;
	for(int i=0; i<5; i++){
		int ret=add10(100);
		printf("RET100=%d\n", ret);
		printf("TMP100=%d\n", tmp);
		for(int k=0; k<10000000; k++) ;
	}
	return NULL;
}


int main() {
	if (pthread_create(&tid1, NULL, thread1, NULL) != 0) {
		exit(1);
	}
	if (pthread_create(&tid2, NULL, thread2, NULL) != 0) {
		exit(1);
	}
	
	// for(int i=0; i<5; i++) {
		// int ret=add10(1000);
		// printf("RET1000=%d\n", ret);
		// printf("TMP1000=%d\n", tmp);
		// for(int k=0; k<10000000; k++) ;
	// }
	
    // return 0;
	pthread_exit(NULL);
}