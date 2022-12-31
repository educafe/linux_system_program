#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define MEM_SIZE 128

int main(void) {
  int fd;
  float *addr;
  const char memid[] = "/mymem";
	char messages[4][20];

  /* open memory file descriptor */
  fd = shm_open(memid, O_RDONLY, 0600);
  if (fd == -1) {
     perror("Can't open file descriptor");
     return 1;
  }

  /* map shared memory */
  addr = mmap(NULL, MEM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
     perror("Memory mapping failed");
     return 1;
  }

  /* read the memory and print the numbers */
  memcpy(messages, addr, sizeof(messages));
  for (int i = 0; i<4; i++) {
		 printf("Msg %d: %s\n", i, messages[i]);
  }
	
	printf("Press enter to munmap .....");
  getchar();
	
	/* memory unmap and delete memory file*/
	munmap(addr, MEM_SIZE);
  shm_unlink(memid);

  return 0;
}
