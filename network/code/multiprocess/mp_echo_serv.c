#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>

#define BUF_SIZE 30
void read_childproc(int sig);
void error_handling(const char *message);

int main(int argc, char *argv[]) {
    
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t adr_sz;
    
    pid_t pid;
    struct sigaction act;
    int state, read_len;
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
    state = sigaction(SIGCHLD, &act, 0);
    
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listne() error");
    
    while(1) {
        adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
        if(clnt_sock == -1)
            continue;
        else
            puts("new client conneted...");
        
        pid = fork();
        if(pid == -1) {
            close(clnt_sock);
            continue;
        }
        if(pid == 0) {
            close(serv_sock);
            while((read_len = read(clnt_sock, buf, BUF_SIZE)) > 0)
                write(clnt_sock, buf, read_len);
            
            close(clnt_sock);
            puts("client disconnected...");
            return 7;
        }
        else
            close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void read_childproc(int sig) {
    pid_t pid;
    int status;
    while(!(pid = waitpid(-1, &status, WNOHANG)));
    
    printf("removed proc id : %d\n", pid);
    if(WIFEXITED(status)) {
        printf("removed proc status : %d\n", WEXITSTATUS(status));
    }
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
