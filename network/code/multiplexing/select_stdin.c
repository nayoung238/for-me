#include <stdio.h>
#include <unistd.h>  // read
#include <sys/select.h>  // select
#include <sys/time.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    
    char buf[BUF_SIZE];

    fd_set reads, temps;
    FD_ZERO(&reads);
    FD_SET(0, &reads); // 0(stdin)

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    int result, str_len;
    while(1){
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        result = select(1, &temps, NULL, NULL, &timeout);

        if(result == -1) {
            puts("select() error");
            break;
        }
        else if(result == 0) {
            puts("Time-out!");
            break;
        }
        else {
            if(FD_ISSET(0, &temps)){
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;  // null (ASCII)
                printf("message from console : %s", buf);
            }
        }
    }
    return 0;
}