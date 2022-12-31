#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#define  KEY_NUM     0x2222
#define  MEM_SIZE    1024

int ready=0;

void handler (int signo) {
	ready = 0;
}

int main() {
	int shm_id;
	void *shm_addr;
	char line[1024];
	sigset_t set1; 
	FILE *fp;
	pid_t pid;
	
	if((shm_id = shmget((key_t)KEY_NUM,MEM_SIZE,IPC_CREAT|0666)) == -1){
		perror("shmget");
		exit(1);
	}
	
	if((shm_addr = shmat(shm_id, NULL, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	
	signal(35, handler);
	
	while(1){
		fp=popen("pidof write","r"); 
		fgets(line,1024,fp);
		pid = strtoul(line,NULL,10);
		if(pid == 0) {
			printf("Process doesn't exit!!\n");
			sleep(1);
		} else 
			break;
	}
	pclose(fp);
	printf("PID = %d\n", pid);
	
	sigemptyset(&set1); 
	
	while(1){
		if(ready == 0){
			printf("READ: %s\n", (char *)shm_addr);
			if(!strcmp(shm_addr, "end")){
				break;
			}
			for(int j=0; j<200000000; j++){}
			kill(pid, 36);
			ready = 0;
			sigsuspend(&set1);
		} 
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