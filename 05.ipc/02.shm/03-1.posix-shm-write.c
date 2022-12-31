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
	char messages[4][20] = {"hello", "world", "good", "morning"};
	
  /* create shared memory file descriptor */
  if ( (fd = shm_open(memid, O_RDWR | O_CREAT, 0600)) == -1) {
     perror("Can't open memory fd");
     return 1;
  }

	/* truncate memory to MEM_SIZE */
	if ( (ftruncate(fd, MEM_SIZE)) == -1 ) {
     perror("Can't truncate memory");
     return 1;
  }

  /* map memory using our file descriptor */
  addr = mmap(NULL, MEM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
     perror("Memory mapping failed");
     return 1;
  }

  /* copy data to memory */
  memcpy(addr, messages, sizeof(messages));

  printf("Press enter to munmap .....");
  getchar();
	
 /* memory unmap */
  munmap(addr, MEM_SIZE);
  return 0;
}
