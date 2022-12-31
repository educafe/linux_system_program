#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void hello() {
   printf("Hello\n");
}
void world(int exit_code, void *arg) {
   printf("world with CODE=%d and MSG=%s\n", exit_code, (char *)arg);
}

int main() {
   /* register the termination function */
   atexit(hello);
   on_exit(world, "Good Bye");
	 atexit(hello);
   
   printf("Starting  main program...\n");

   printf("Exiting main program...\n");
   
   // exit(10);
   _exit(20);
   // return 100;
}


