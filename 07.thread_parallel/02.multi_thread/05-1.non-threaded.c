#include <stdio.h>
int main(void){
	long long int i = 0;
	for (int x = 1; x <= 5; x++){
		for (long long int j = 1; j <= 1000000000; j++){
			i = i + 1;
		}
	}
	printf("Sum is %lld\n", i);
	return 0;
}