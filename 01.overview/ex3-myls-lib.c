#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>

int main(int argc, char **argv) {
    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(argc > 1 ? argv[1] : ".");

    if (dr == NULL) {  // opendir returns NULL if couldn't open directory
        perror("Could not open current directory" );
        return 0;
    }

    // For readdir()
    while ((de = readdir(dr)) != NULL)
            printf("%s\n", de->d_name);

    closedir(dr);
    return 0;
}
