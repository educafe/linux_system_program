#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: %s <src filename> <new filename>\n", argv[0]);
        return 1;
    }
    if (symlink(argv[1], argv[2]) == -1){
        perror("Can't create symbolic link");
        return 1;
    }
    return 0;
}
