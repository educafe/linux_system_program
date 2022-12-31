#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h> 

void *fun();
sem_t s;

int main() {
	sem_init(&s, 0, 2);
	int num;
	printf("Enter the number of threads : \n");
	scanf("%d", &num);
	pthread_t thread[num];
	for(int i=0; i<num; i++) {
		pthread_create(&thread[i], NULL, fun, &i);
	}
	for(int i=0; i<num; i++) 
		pthread_join(thread[i], NULL);
	return 0;
}

void *fun(void *x) {
	int n=*(int *)x;
	sem_wait(&s);
	printf("Thread[%d] acquired the resource\n", n);
	sleep(1);
	sem_post(&s);
	printf("Thread[%d] finished\n", n);
}
