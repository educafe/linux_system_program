#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

void *isprime(void *arg);
void *progress(void *arg);
pthread_mutex_t mutex_id;
pthread_cond_t ready;
pthread_t primeid = 0;

int main(int argc, char *argv[]){
  long long num1, num2;
  pthread_t tid1, tid2, tid3;
  void *result1, *result2;

  if ((pthread_mutex_init(&mutex_id, NULL)) != 0){
     fprintf(stderr, "Couldn't initialize mutex\n");
     return 1;
  }

  if ((pthread_cond_init(&ready, NULL)) != 0){
     fprintf(stderr, "Couldn't initialize condition variable\n");
     return 1;
  }

  if (argc != 3){
     fprintf(stderr, "Please supply two numbers.\n" "Example: %s 9 7\n", argv[0]);
     return 1;
  }
	
  num1 = atoll(argv[1]);
  num2 = atoll(argv[2]);
 
  pthread_create(&tid3, NULL, progress, NULL);  
  pthread_detach(tid3);
  pthread_create(&tid1, NULL, isprime, &num1);
  pthread_create(&tid2, NULL, isprime, &num2);

  pthread_mutex_lock(&mutex_id);
  for (int i=0; i<2; i++) {
    while (primeid == 0) {
			printf("I am waiting for any thread to be finished\n");
			pthread_cond_wait(&ready, &mutex_id);
		}
        
    if (primeid == tid1) {
      pthread_join(tid1, &result1);
      if ((uintptr_t)result1 == 1)
        printf("\n%lld is a prime number\n",  num1);
      else
        printf("\n%lld is not a prime number\n", num1);
      primeid = 0;
    } else {
      pthread_join(tid2, &result2);   
      if ((uintptr_t)result2 == 1)
        printf("\n%lld is a prime number\n", num2);
      else
        printf("\n%lld is not a prime number\n", num2);
      primeid = 0;
    }
  }
  pthread_mutex_unlock(&mutex_id);

  if (pthread_cancel(tid3) != 0)
    fprintf(stderr, "Couldn't cancel progress thread\n");
 
  return 0;
}

void *isprime(void *arg){
  long long int number = *((long long*)arg);
  long long int j;
  int prime = 1;
   
  for(j=2; j<number; j++){
    if(number%j == 0)
      prime = 0;
  }
  pthread_mutex_lock(&mutex_id);
  primeid = pthread_self();
  pthread_cond_signal(&ready);
  pthread_mutex_unlock(&mutex_id);
  if(prime == 1)
    return (void*)1;
  else
    return (void*)0;
}

void *progress(void *arg){
  while(1) {
    sleep(1);
    printf(".");
    fflush(stdout);
  }
  return NULL;
}
