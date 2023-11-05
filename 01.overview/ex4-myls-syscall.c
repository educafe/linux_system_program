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

void print_wrapped_text(const char *text, int line_width) {
  int space_left = line_width;
  const char *word_start = text;
  int word_length;

  while (*text) {
    // 단어의 시작을 찾습니다.
    while (*text && (*text == ' ' || *text == '\n')) {
      // 공백이나 개행 문자를 그대로 출력합니다.
      putchar(*text);
      if (*text == '\n') {
          space_left = line_width; // 개행 문자가 나오면 공간을 리셋합니다.
      } else {
          space_left--; // 공백 문자를 출력할 때마다 남은 공간을 줄입니다.
      }
      text++;
    }
    word_start = text;

    // 다음 공백이나 문자열의 끝까지 단어의 길이를 측정합니다.
    while (*text && *text != ' ' && *text != '\n') {
      text++;
    }
    word_length = text - word_start;

    // 현재 단어를 현재 라인에 출력할 수 있는지 검사합니다.
    if (word_length > space_left) {
      // 단어를 다음 라인으로 넘깁니다.
      putchar('\n');
      space_left = line_width;
    }

    // 단어를 출력합니다.
    while (word_start < text) {
      putchar(*word_start++);
      space_left--;
    }
  }
}


int main(int argc, char *argv[]) {
  int fd;
  int nread;
  char buffer[BUFFER_SIZE];
  struct linux_dirent *d;
  int bpos;
	char buf[4096]={0 };

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
      // printf("%s\t", d->d_name);
			int current_length = strlen(buf);
			snprintf(buf + current_length, sizeof(buf) - current_length,\
															"%-30s", d->d_name);
      bpos += d->d_reclen;
    }
  }
	// printf("%s", buf);
	print_wrapped_text(buf, 90);
	printf("\n");
  close(fd);
  return 0;
}
