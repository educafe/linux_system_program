#include <stdio.h> 
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h> 
#include <string.h>

int main(int argc, char * argv[]){ 
	char s[100];
	char *dir;
	// char dir[100];

	// printing current working directory 
	printf("%s\n", getcwd(s, 100)); 
	
	// Get the current user's UID
	uid_t uid=getuid();
	// Get the passwd structure for the user's UID
	struct passwd *pw=getpwuid(uid);

	if(argc > 1)
		dir=argv[1];										//directory to change
		// strcpy(dir,argv[1]);					//directory to change
	else
		dir=pw->pw_dir;									//user's home directory
		// strcpy(dir, pw->pw_dir);			//user's home directory
	
	// if(chdir(argc > 1 ? argv[1] : pw->pw_dir) == -1){
		// perror("chdir");
		// exit(100);
	// }
	
	if(chdir(dir) == -1){
		perror("chdir");
		exit(100);
	}

	// printing current working directory 
	printf("%s\n", getcwd(s, 100)); 

	return 0; 
} 
