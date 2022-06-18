#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main(){
	int fds[2];
	char str_child[] = "I'm a child\n";
    char str_parent[] = "I'm a parent\n";
	char buf[BUF_SIZE];

	pipe(fds);
	pid_t pid = fork();

	if(pid == 0) {
        write(fds[1], str_child, sizeof(str_child));
        sleep(2);
        read(fds[0], buf, BUF_SIZE);
        printf("Child proc output: %s\n", buf);
    }
	else {
		read(fds[0], buf, BUF_SIZE);
        printf("Parent proc output : %s\n", buf);
        write(fds[1], str_parent, sizeof(str_parent));
        sleep(3);
	}
	return 0;
}