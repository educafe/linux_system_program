#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

struct linux_dirent {
		unsigned long  d_ino;
    off_t          d_off;
    unsigned short d_reclen;
    char           d_name[];
};

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    int fd;
    int nread;
    char buffer[BUFFER_SIZE];
    struct linux_dirent *d;
    int bpos;

    if(argc > 1){
			fd = open(argv[1], O_RDONLY | O_DIRECTORY);
		} else {
			fd = open(".", O_RDONLY | O_DIRECTORY);
		}
		
    if (fd == -1)
        return 1;

    for ( ; ; ) {
        nread = syscall(SYS_getdents, fd, buffer, BUFFER_SIZE);
        if (nread == -1)
            return 1;

        if (nread == 0)
            break;

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *)(buffer + bpos);
            printf("%s\n", d->d_name);
            bpos += d->d_reclen;
        }
    }
    close(fd);
    return 0;
}
