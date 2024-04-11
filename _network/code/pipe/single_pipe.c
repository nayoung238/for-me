#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main(){
	int fds[2];
	char str[] = "Hello world!";
	char buf[BUF_SIZE];

	pipe(fds);
	pid_t pid = fork();

	if(pid == 0) write(fds[1], str, sizeof(str));
	else {
		wait(NULL);
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}