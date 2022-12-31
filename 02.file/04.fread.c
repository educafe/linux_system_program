#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]){
	FILE *fp;
	size_t ret;
	char data[512];	
	if((fp = fopen(argv[1], "r")) == NULL){
		perror("fopen");
		exit(1);
	}
	memset(data, 0, sizeof(data));
	while(1) {
		printf("FILE-POS-IND: %ld\n", ftell(fp));
		ret=fread(data, sizeof(char), sizeof(data), fp);
		printf("FILE-POS-IND: %ld\n", ftell(fp));
#if 0
		puts(data);
#else
		printf("%s", data);
#endif
		if (feof(fp)) {
			break;
		}
	}
	fclose(fp);
	return 0;
}

