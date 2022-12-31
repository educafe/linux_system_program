#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int isprime(long long arg);

int main(int argc, char *argv[]){
  long long number1;
  long long number2;
  if (argc != 3) {
    fprintf(stderr, "Please supply two numbers.\n" "Example: %s 9 7\n", argv[0]);
    return 1;
  }
  number1 = atoll(argv[1]);
  number2 = atoll(argv[2]);
	isprime(number1);
	isprime(number2);
  return 0;
}

int isprime(long long arg){
   long long int number = arg;
   long long int j;
   int prime = 1;
    
   for(j=2; j<number; j++) {
      if(number%j == 0){
         prime = 0;
      }
   }
   if(prime == 1){
      printf("\n%lld is a prime number\n", number);
      return 1;
   }else{
      printf("\n%lld is not a prime number\n", number);
      return 0;
   }
}

