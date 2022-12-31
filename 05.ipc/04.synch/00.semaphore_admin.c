#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

int main(int argc, char * argv[]) {
	union senum {
		int val;
		struct semid_ds *buf;
		unsigned short int *array;
	} sem_ctrl;
	
	int sem_val;
	static int	semid;
	
	if((semid = semget((key_t)0x1234,1,IPC_CREAT|0666)) == -1){
		perror("shmget");
		exit(1);
	}
	
	if(argc == 2) {
		if(strcmp(argv[1], "-d") == 0) {
			printf("Destroy----\n");
			if (semctl(semid, 0, IPC_RMID, sem_ctrl) == -1) {
				perror("semctl");
				exit(1); 
			} 
		} else {
			fprintf(stderr, "Usage : %s -d\n", argv[0]);
			exit(1);
		}
		exit(0);
	}
		
	sem_val=semctl(semid, 0, GETVAL, sem_ctrl);
	printf("current number of semphone : %d\n ", sem_val);
	
	if(sem_val != 1)
		sem_val=1;
	sem_ctrl.val=sem_val;
	semctl(semid, 0, SETVAL, sem_ctrl);
	
	printf("modified number of semphone : %d\n ", sem_val);
	return 0;
}