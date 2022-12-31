#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#define MEM_SIZE 128

int main(void) {
	char *addr;
  pid_t pid;
	char writebuf[4096];
	char readbuf[4096];
 
  addr = mmap(NULL, MEM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (addr == MAP_FAILED) {
     perror("mmap");
     return 1;
  }
   
  printf("Parent PID is %d\n", getpid());
	
  if ( (pid = fork()) == -1 ) {
     perror("Can't fork");
     return 1;
  }
	
  if (pid == 0) {     								/* child */
		for(int i=0; i<5; i++) {
			sprintf(writebuf, "I am child  --- %d\n", i);
			memcpy(addr, writebuf, strlen(writebuf) + 1);
			sleep(1);
		}
		sprintf(writebuf, "%s", "end");
		memcpy(addr, writebuf, strlen(writebuf) + 1);
  }
  else if(pid > 0) {									/* parent */
		printf("Child executed with PID %d\n", pid);
		for(int i=0; ; i++) {
			sleep(1);
			sprintf(readbuf, "%s", addr);
			if(strstr(readbuf, "end"))
				break;
			printf("PARENT: %s", addr);
		}
  }
  munmap(addr, MEM_SIZE);
  return 0;
}
