#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char *argv[]){
	char buf[1024];
	ssize_t n;
	int msqid;
	int msg_type;
	
	struct msgbuf {
		long m_type;
		char m_text[1024];
	}recvbuf;
	
	msqid = msgget(0x222, IPC_CREAT | 0666);		//기존 메세지 큐를 구하거나 새로운 메세지 큐 객체 생성
	if(msqid == -1){
		perror("msgget error");
		exit(1);
	}

	if ((argc < 2 ) || (atoi(argv[1]) > 3)) {
		fprintf(stderr, "Usage : %s '0 < <magtype> < 4'\n", argv[0]);
		exit(1);
	}	else
		msg_type = atoi(argv[1]);
	
	printf("MSG_TYPE = %d\n", msg_type);
	
	while(1) {
		if(msgrcv(msqid, &recvbuf, sizeof(recvbuf), msg_type, IPC_NOWAIT) == -1){
			perror("msgrcv : ");
      break; 
		}
		printf("Type: %ld\t", recvbuf.m_type);
		printf("Rx: %s", recvbuf.m_text);
	}
	
#if 0	
	if(msgctl(msqid, IPC_RMID, 0) == -1){		//메세지 큐 객체 삭제
		perror("msgctl error");
		exit(1);
	}
#endif
	return 0;
}
