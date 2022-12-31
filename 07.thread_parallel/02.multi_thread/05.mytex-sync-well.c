#include <stdio.h>
#include <pthread.h>

void *add(void *arg);
long long int sum = 0;
pthread_mutex_t mutex_id;

int main(void){
  pthread_t tid1, tid2, tid3, tid4, tid5;

  if ((pthread_mutex_init(&mutex_id, NULL)) != 0) {
     fprintf(stderr, "Couldn't initialize mutex\n");
     return 1;
  }
  pthread_create(&tid1, NULL, add, NULL);
  pthread_create(&tid2, NULL, add, NULL);
  pthread_create(&tid3, NULL, add, NULL);
  pthread_create(&tid4, NULL, add, NULL);
  pthread_create(&tid5, NULL, add, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
  pthread_join(tid4, NULL);
  pthread_join(tid5, NULL);

  printf("Total is %lld\n", sum);
  if ( (pthread_mutex_destroy(&mutex_id)) != 0 )   {
     fprintf(stderr, "Couldn't destroy mutex\n");
     return 1;
  }
  return 0;
}

void *add(void *arg){
  long long int j = 1;
  while(j < 1000000000)   {
     j = j + 1;
  }

  pthread_mutex_lock(&mutex_id);
  sum = sum + j;
  pthread_mutex_unlock(&mutex_id);
  return NULL;
}
