#include <stdio.h>
#include <pthread.h>

void *add(void *arg);
long long int sum = 0;

int main(void){
   pthread_t tid1, tid2, tid3, tid4, tid5;

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
   return 0;
}

void *add(void *arg){
   for (long long int j = 1; j <= 1000000000; j++) {
      sum = sum + 1;
   }
   return NULL;
}
