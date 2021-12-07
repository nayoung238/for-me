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
    
    char buf1[BUF_SIZE], buf2[BUF_SIZE];
    int idx, len, write_len, read_len;
    
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
    
    read_len = write_len = 0;
    int read_idx = 0, target = 0;
    while(1) {
        fputs("Input message(Q(q) to quit): ", stdout);
        fgets(buf1, BUF_SIZE, stdin);
        
        if(!strcmp(buf1, "q\n") || !strcmp(buf1, "Q\n")) {
            shutdown(clnt_sock, SHUT_WR);
            break;
        }
        write_len = write(clnt_sock, buf1, strlen(buf1));
        if(len == -1)
            error_handling("write() error");
        /*
        target = 0;
        while(target < write_len) {
            read_len = read(clnt_sock, &buf2[read_idx], BUF_SIZE);
            if(read_len == -1) break;
            printf("client : %s", buf2);
            printf("store: %d %d\n", read_idx, read_len);
            read_idx += read_len;
            target += read_len;
            printf("%d, %d\n", target, write_len);
        }*/
    }
    
    read_len = idx = 0;
    while(read_len < write_len) { // read() != 0
        len = read(clnt_sock, &buf2[read_len], BUF_SIZE - 1);
        if(len == -1)
            error_handling("read() error");
        read_len += len;
    }
    
    printf("Received message content: %s\n", buf2);
    close(clnt_sock);
    return 0;
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
