#include <stdio.h>
#include <unistd.h>   
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#define  KEY_NUM     0x2222
#define  MEM_SIZE    1024

int ready=1;

void handler (int signo) {
	if(signo == 36) {
		ready = 1;
	} 
}

int main() {
	int shm_id;
	void *shm_addr, *shm_addr_dt;
	int count;
	char line[1024];
	sigset_t set2; 
	FILE *fp;
	pid_t pid;
	
	if((shm_id = shmget((key_t)KEY_NUM,MEM_SIZE,IPC_CREAT|0666)) == -1){
		perror("shmget");
		exit(1);
	}
	
	signal(36, handler);
	
	while(1){
		fp=popen("pidof read","r"); 
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
	
	sigemptyset(&set2); 
	
	printf("Please enter to attach shared memory ->\n");
	getchar();
	
	if((shm_addr = shmat(shm_id, NULL, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	
	for(count=11; count<=15; count++){
		if(ready == 1){
			sprintf((char *)shm_addr, "Message --- %d", count);
			printf("WRITE: %s\n", (char *)shm_addr);
			for(int j=0; j<500000000; j++){}
			kill(pid, 35);
			ready = 0;
			sigsuspend(&set2);
		} 
	}
	for(int j=0; j<100000000; j++){}
	sprintf((char *)shm_addr, "end");
	kill(pid, 35);
	
	printf("Please enter to detach shared memory ->\n");
	getchar();
	if(shmdt(shm_addr) !=0){
		perror("shmdt");
		exit(2);
	}
#if 0
	if(shmctl(shm_id, IPC_RMID, NULL) == -1){
		perror("shmctl");
		exit(2);
	}
#endif
	return 0;
}