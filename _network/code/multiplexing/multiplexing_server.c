#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h> // memset
#include <unistd.h> // read, close
#include <netinet/in.h> // sockaddr_in
#include <sys/select.h> // select
#include <sys/socket.h> // socket
#include <signal.h> // sigaction, sigemptyset

#define BUF_SIZE 100

void read_childproc(int sig) {
    pid_t pid;
    int status;
    while(!(pid = waitpid(-1, &status, WNOHANG)));
    
    printf("child proc id : %d\n", pid);
    if(WIFEXITED(status))
        printf("child proc return value : %d\n", WEXITSTATUS(status));
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
int main(int argc, char *argv[]) {
    
    int serv_sock, clnt_fd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    
    struct sigaction act;
    
    fd_set readfds, readtmp;
    struct timeval timeout;
    int max_fd, select_result;
    int read_len;
    char buf[BUF_SIZE];
    
    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    
    FD_ZERO(&readfds);
    FD_SET(serv_sock, &readfds);
    max_fd = serv_sock;
    timeout.tv_sec = 10;
    timeout.tv_usec = 1000;
    
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    while(1) {
        readtmp = readfds;
        timeout.tv_sec = 10;
        timeout.tv_usec = 1000;
        
        select_result = select(max_fd + 1, &readtmp, NULL, NULL, &timeout);
        if(select_result == -1) {
            error_handling("select() error");
        }
        else if(select_result == 0) {
            puts("Time-out");
        }
        else {
            for(int i = 0; i <= max_fd; i++) {
                if(FD_ISSET(i, &readtmp)) {
                    if(i == serv_sock) {
                        clnt_addr_sz = sizeof(clnt_addr_sz);
                        clnt_fd = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_sz);
                        if(clnt_fd == -1) {
                            error_handling("accept() error");
                        }
                        FD_SET(clnt_fd, &readfds);
                        printf("new connected: %d\n", clnt_fd);
                        if(max_fd < clnt_fd) {
                            max_fd = clnt_fd;
                        }
                    }
                    else {
                        read_len = read(i, buf, BUF_SIZE);
                        if(read_len == 0) {
                            close(i);
                            FD_CLR(i, &readfds);
                            printf("Closed client: %d\n", i);
                        }
                        else {
                            write(i, buf, read_len);
                        }
                    }
                }
            }
        }
    }
    return 0;
}