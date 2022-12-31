#include <stdio.h>
#include <stdlib.h>

typedef struct INFO {
	char	name[20];
	int		age;
	float	height;
} INFO;

#define DATA_SIZE	5

int main(int argc, char * argv[]){
	FILE *fp;
	
	INFO data[DATA_SIZE];		/* first if is 0 */
	
	if((fp = fopen(argv[1], "rb")) == NULL){
		perror("fopen");
		exit(1);
	}
	printf("DATA-PTR : %p\n", &data);
	while(1) {
		printf("FILE-POS-IND: %ld\n", ftell(fp));
		fread(data, sizeof(INFO), 1, fp);
		// fread(&data, sizeof(INFO), 1, fp);
		if(feof(fp)){
			break;
		}
		printf("name:%s, age:%d, height:%.1f\n", data->name, data->age, data->height);
	}
	fclose(fp);
	return 0;
					
}
