#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(const char *message);

int main(int argc, char *argv[]){
    
    int clnt_sock;
    struct sockaddr_in serv_addr;
    
    char buf[BUF_SIZE];
    int write_len, read_len, read_total_len;
    
    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(clnt_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    
    write_len = 0;
    while(1) {
        fputs("Input message(Q(q) to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
            shutdown(clnt_sock, SHUT_WR);
            break;
        }
        write_len = write(clnt_sock, buf, strlen(buf));
        if(write_len == -1)
            error_handling("write() error");
    }
    
    read_total_len = read_len = 0;
    while(read_len < write_len) {
        read_len = read(clnt_sock, &buf[read_total_len], BUF_SIZE - 1);
        if(read_len == -1)
            error_handling("read() error");
        read_total_len += read_len;
    }
    
    printf("Received message content: %s\n", buf);
    close(clnt_sock);
    return 0;
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
