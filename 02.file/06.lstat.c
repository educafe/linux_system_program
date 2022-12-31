#include <errno.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <time.h> 

int main(int argc, char *argv[]) { 
	struct stat fileinfo; 
	
	if (argc != 2) { 
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]); 
		return 1; 
	} 
	
	if (lstat(argv[1], &fileinfo) == -1) { 
		perror("lstat"); 
		return 1; 
	} 
	
	printf("File type: "); 
	switch (fileinfo.st_mode & S_IFMT) { 
		case S_IFBLK: 
			printf("block device\n"); 
			break; 
		case S_IFCHR: 
			printf("character device\n"); 
			break; 
		case S_IFDIR: 
			printf("directory\n"); 
			break; 
		case S_IFIFO: 
			printf("FIFO/pipe\n"); 
			break; 
		case S_IFLNK: 
			printf("symlink\n"); 
			break; 
		case S_IFREG: 
			printf("regular file\n"); 
			break; 
		case S_IFSOCK: 
			printf("socket\n"); 
			break; 
		default: 
			printf("unknown?\n"); 
			break; 
	} 
	
	printf("I-node : %ld\n", fileinfo.st_ino); 
	printf("Permission or mode : %o (octal)\n", fileinfo.st_mode); 
	printf("Link count: %ld\n", fileinfo.st_nlink); 
	printf("UID=%d GID=%d\n", fileinfo.st_uid, fileinfo.st_gid); 
	printf("I/O block size: %ld bytes\n", fileinfo.st_blksize); 
	printf("File size: %ld bytes\n", fileinfo.st_size); 
	printf("Blocks allocated: %ld\n", fileinfo.st_blocks); 
	printf("Last change: %s", ctime(&fileinfo.st_ctime)); 
	printf("Last access: %s", ctime(&fileinfo.st_atime)); 
	printf("Last modification: %s", ctime(&fileinfo.st_mtime)); 
	
	return 0; 
}

