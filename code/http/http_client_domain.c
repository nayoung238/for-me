//
//  main.c
//  http
//
//  Created by nayoung on 2021/11/06.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFF_SIZE 1024
void error_handling(const char *message){
    perror(message);
    exit(1);
}
int main(int argc, const char * argv[]) {
    
    if(argc != 3){
        printf("Usage : %s <addr>\n", argv[0]);
        exit(1);
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    
    struct hostent *host = gethostbyname(argv[1]);
    if(!host) error_handling("gethostbyname() error");
    
    addr.sin_addr = *((struct in_addr*)host -> h_addr_list[0]);
    addr.sin_port = htons(atoi(argv[2]));
    
    if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        error_handling("connect() error");
    
    char *sendMsg = "GET /webhp HTTP/1.1\r\nUser-Agent: Mozilla/4.0\r\nContent-type: text/html\r\nConnection:clos\r\n\r\n";
    
    if(write(sock, sendMsg, strlen(sendMsg)) == -1) error_handling("send() error");
    
    char recvMsg[BUFF_SIZE];
    if(read(sock, recvMsg, BUFF_SIZE) == -1) error_handling("recv() error");
    
    printf("%s\n", recvMsg);
    close(sock);
    return 0;
}
