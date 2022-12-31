#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int a=10, b;
int main() {
	int c=100, c2;
	char *d = "I am parent";
	char *name;
	name = malloc(50);
	name = "educafe";
	pid_t pid=fork();
	if(pid == 0){
		a=200; b=400; c=600; d="I am child"; name="ubuntu";
	}
	printf("\e[00m%s[%d] of ppid[%d]\n", d, getpid(), getppid());
	printf("paddr(TEXT)=> main:%p\n", main);
	printf("paddr(CONST)=> d:%p, d_value:%s\n", d, d);
	printf("paddr(DATA)=> a:%p, a_value:%d\n", &a, *(&a));
	printf("paddr(BSS)=> b:%p, b_value:%d\n", &b, *(&b));
	printf("paddr(STACK)=> c:%p, c_value:%d, d:%p, d_value:%p\n", &c, *(&c), &d, *(&d));
	printf("paddr(HEAP)=> name:%p, name_value:%s\n", &name, name);
	sleep(1);
}