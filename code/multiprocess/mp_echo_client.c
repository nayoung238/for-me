#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
//#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(const char *message);

int main(int argc, char *argv[]){
    
    int sock, read_len;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    
    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else
        puts("connected...");
    
    while(1) {
        fputs("Input message(Q to quit) : ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
            break;
        
        write(sock, buf, strlen(buf));
        read_len = read(sock, buf, BUF_SIZE - 1);
        buf[read_len] = 0;
        printf("Message from server : %s", buf);
    }
    close(sock);
    return 0;
}

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
