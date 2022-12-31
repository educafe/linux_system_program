#include <stdio.h>
#include <stdlib.h>
typedef struct _INFO {
	char name[20];
	int age;
	float height;
} INFO;

#define DATA_SIZE	5

int main(int argc, char *argv[]){
	INFO data[DATA_SIZE] = {
					"kim",20,170.1F,
					"lee",25,180.3F,
					"park",21,176.2F,
					"choi",28,167.9F,
					"kang",25,172.5F
	};
	FILE *fp;
	if((fp=fopen(argv[1], "wb"))==NULL){
		perror("fopen");
		exit(1);
	}
	printf("FILE-POS-IND: %ld\n", ftell(fp));
	printf("SZ-DATA=%ld, SZ-INFO=%ld\n", sizeof(data), sizeof(INFO));
	printf("COUNT=%ld\n", sizeof(data)/sizeof(INFO));
	if(fwrite(&data, sizeof(INFO), sizeof(data)/sizeof(INFO), fp) == -1){
		perror("fwrite");
		fclose(fp);
		exit(2);
	}
	printf("FILE-POS-IND: %ld\n", ftell(fp));
	fclose(fp);
	return 0;
}
