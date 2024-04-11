#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main(){
    char str_child[] = "Hi! I'm a child";
    char str_parent[] = "Hi! I'm a parent";
    char buf[BUF_SIZE];

    int fds1[2], fds2[2];
    pipe(fds1);
    pipe(fds2);

    pid_t pid = fork();
    if(pid == 0) {
        close(fds1[0]);
        close(fds2[1]);

        write(fds1[1], str_child, sizeof(str_child));
        read(fds2[0], buf, BUF_SIZE);
        printf("Child proc output : %s\n", buf);
    }
    else {
        close(fds1[1]);
        close(fds2[0]);

        read(fds1[0], buf, BUF_SIZE);
        printf("Parent proc output : %s\n", buf);
        write(fds2[1], str_parent, sizeof(str_parent));
        sleep(2);
    }
    return 0;
}