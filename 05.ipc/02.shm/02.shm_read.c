#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

#define  KEY_NUM     0x2222
#define  MEM_SIZE    1024

int main() {
	int shm_id;
	void *shm_addr;
	char buf[128];
	
	key_t key = ftok("shmkey", 1029);
	
	// if((shm_id = shmget((key_t)KEY_NUM,MEM_SIZE,IPC_CREAT|0666)) == -1){
	if((shm_id = shmget(key,MEM_SIZE,IPC_CREAT|0666)) == -1){
		perror("shmget");
		exit(1);
	}
	
	printf("Please enter to attach shared memory ->\n");
	getchar();
	
	if((shm_addr = shmat(shm_id, NULL, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	
	while(1){
		printf("READ: %s\n", (char *)shm_addr);
		if(!strcmp(shm_addr, "end")){
			break;
		}
		for(int j=0; j<500000000; j++){}
	}
	
	if(shmdt(shm_addr) !=0){
		perror("shmdt");
		exit(2);
	}
	
	printf("Please enter to remove shared memory ->\n");
	getchar();
	
	if(shmctl(shm_id, IPC_RMID, NULL) == -1){
		perror("shmctl");
		exit(2);
	}
	return 0;
}