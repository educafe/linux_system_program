#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char * argv[]) {
	pid_t pid;
  FILE *fp;
  time_t now; /* for the current time */
  const char pidfile[] = "/var/run/mydaemon.pid";
	char *cmd;
	int fd0, fd1, fd2;
	
	if ((cmd = strrchr(argv[0], '/')) == NULL)
		cmd = argv[0];
	else
		cmd++;

  if ((pid = fork()) == -1 ){
     perror("first fork failed");
     exit(1);
  }else if(pid != 0){
     exit(0);
  }
  /* the parent process has exited, so this is the child. */
    
  if((pid = fork()) == -1 ){
     perror("second fork failed");
     return 1;
  }else if(pid != 0){ 			   /* the child process which will exit */
     exit(0);
  }
  /* fork again, creating a grandchild, the actual daemon */
	
	/* create a new session to lose the controlling terminal */
	setsid();
	if ((fp = fopen(pidfile, "w")) == NULL){
		perror("fopen");
    exit(1);
  }
     // /* write pid to file */
  fprintf(fp, "%d\n", getpid()); 
  fclose(fp); /* close the file pointer */

  umask(022); 						/* set the umask to something ok */
  chdir("/"); 						/* change working directory to / */
	
  /* from here, we don't need stdin, stdout or, stderr anymore, 
   * so let's close them all, then re-open them to /dev/null */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  fd0=open("/dev/null", O_RDONLY); 	/* 0 = stdin */
  fd1=open("/dev/null", O_WRONLY); 	/* 1 = stdout */
  fd2=open("/dev/null", O_RDWR); 		/* 2 = stderr */

	/* open the syslog file for writing */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "file open error with FD0=%d FD1=%d FD2=%d", fd0, fd1, fd2);
		exit(1);
	}

  /* here we start the daemons "work" */
  for (;;){
     /* get the current time and write it to the daemonfile" that we opened above */
     time(&now);
     syslog(LOG_INFO|LOG_DAEMON, "Daemon Process alives at %s\n", ctime(&now));
     sleep(5);
  }
  return 0;
}
