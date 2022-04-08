#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit
#include <fcntl.h>  // open, close
#include <unistd.h> // read, write

#define BUF_SIZE 1024

void error_handling(const char *message);
int main(){
    
    int fd;
    char buf[BUF_SIZE] = "Hello world!\n";
    
    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if(fd == -1)
        error_handling("open() error");
    
    printf("file descriptor: %d\n", fd);
    
    if(write(fd, buf, BUF_SIZE) == -1)
        error_handling("write() error");
    
    fd = open("data.txt", O_RDONLY);
    if(read(fd, buf, BUF_SIZE) == -1)
        error_handling("read() error");
    
    printf("file data: %s", buf);
    close(fd);
    return 0;
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
