#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 3										/* Size of shared buffer */

int buffer[BUF_SIZE];									/* shared buffer */
int msg_cnt=0;												/* number of messages in buffer */

// pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;		/* mutex lock for buffer */
// pthread_cond_t ready=PTHREAD_COND_INITIALIZER; 	/* condition variable */
pthread_mutex_t mutex;
pthread_cond_t ready;

void *producer(void *arg);
void *consumer(void *arg);

int main(){
	pthread_t tid1, tid2;	

	if ((pthread_mutex_init(&mutex, NULL)) != 0){
		fprintf(stderr, "Couldn't initialize mutex\n");
    return 1;
  }

  if ((pthread_cond_init(&ready, NULL)) != 0){
		fprintf(stderr, "Couldn't initialize condition variable\n");
    return 1;
  }
	

	if(pthread_create(&tid1,NULL,producer,NULL) != 0){
		perror("pthread_create for producer");
		exit (1);
	}
	if (pthread_create(&tid2,NULL,consumer,NULL) != 0) {
		perror("pthread_create for consumer");
		exit (1);
	}

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	printf ("main exits\n");
}


void *producer(void *arg){
	int i;
	int index=0;
	
	for(i=0; i<6; i++){
		pthread_mutex_lock(&mutex);
		if (msg_cnt == BUF_SIZE){							// wait until buffer is freed		
			printf("waiting for singal by consumer\n");
			fflush(stdout);
			pthread_cond_wait(&ready, &mutex);
		}
		buffer[index] = i;
		printf("Produced --- %d\n", buffer[index]);
		fflush(stdout);
		msg_cnt++;
		index = (index+1) % BUF_SIZE;
		pthread_cond_signal(&ready);
		pthread_mutex_unlock(&mutex);
		for(int i=0; i<100000000; i++){}
	}
	printf ("producer exits\n");  fflush (stdout);
}

void *consumer(void *arg){
	int i=0;
	int index=0;
	
	for(i=0; i<6; i++){
		pthread_mutex_lock(&mutex);
		if (msg_cnt == 0){									/* block if buffer empty */
			printf("\twaiting for singal by producer\n");
			fflush(stdout);
			pthread_cond_wait(&ready, &mutex);
		}
		/* if executing here, buffer not empty so take element */
		printf("\tConsumed --- %d\n", buffer[index]);
		fflush(stdout);
		msg_cnt--;
		index = (index+1) % BUF_SIZE;
		pthread_cond_signal(&ready);
		pthread_mutex_unlock(&mutex);
		// for(int i=0; i<100000000; i++){}
	}
	printf("Consumer exits\n"); fflush(stdout);
}