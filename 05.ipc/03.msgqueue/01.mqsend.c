#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char * argv[]) {
	char buf[1024];
	int msqid;
	int msgtype;
	
	struct msgbuf{
		long m_type;
		char m_text[1024];
	}sendbuf;
	
	if ((argc < 2 ) || (atoi(argv[1]) > 3)) {
		fprintf(stderr, "Usage : %s '0 < <magtype> < 4'\n", argv[0]);
		exit(1);
	}	else
		msgtype = atoi(argv[1]);
	
	msqid = msgget(0x222, IPC_CREAT | 0666);				//�޼��� ť ����
	
	for(int i=1; i<5; i++) {												
		memset(&sendbuf, 0, sizeof(sendbuf));
		sendbuf.m_type = msgtype;											//�޼��� Ÿ�� ����
		sendbuf.m_type = (i%3)+1;											//�޼��� Ÿ�� ����
		printf("MSGTYPE[%d] --- %ld\n", i, sendbuf.m_type);
		sprintf(buf, "message ---- %d\n", i);
		strncpy(sendbuf.m_text, buf, strlen(buf));
		msgsnd(msqid, &sendbuf, sizeof(sendbuf), 0);	//0: wait till queue space is available
	}

	printf("Press Enter to remove message queue....");
	getchar();

	if(msgctl(msqid, IPC_RMID, 0) == -1){		//�޼��� ť ��ü ����
		perror("msgctl error");
		exit(1);
	}
	return 0;
}
