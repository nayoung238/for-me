#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // atoi()
#include <unistd.h>  // close(socket)
#include <netinet/in.h> // INADDR_ANY
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
void error_handling(const char *message);

int main(int argc, char *argv[]){
    
    int from_sock;
    struct sockaddr_in from_addr, to_addr, my_addr;
    socklen_t to_addr_len, my_addr_len;
    
    char buf[BUF_SIZE];
    ssize_t send_len, recv_len;
    
    if(argc != 1) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    from_sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    memset(&from_addr, 0, sizeof(from_addr));
    from_addr.sin_family = AF_INET;
    from_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    from_addr.sin_port = htons(9190);
    
    if(bind(from_sock, (struct sockaddr*) &from_addr, sizeof(from_addr)) == -1)
       error_handling("bind() error");
    
    my_addr_len = sizeof(my_addr_len);
    if(getsockname(from_sock, (struct sockaddr*) &my_addr, &my_addr_len) == -1)
        error_handling("getsockname() error");
    
    printf("IP : %u\n", ntohl(my_addr.sin_addr.s_addr));
    printf("Port : %hd\n", ntohs(my_addr.sin_port));
    
    for(int i = 0; i < 5; ++i) {
        memset(&to_addr, 0, sizeof(to_addr));
        to_addr_len = sizeof(to_addr);
        recv_len = recvfrom(from_sock, buf, BUF_SIZE, 0, (struct sockaddr*) &to_addr, &to_addr_len);
        if(recv_len == -1)
            error_handling("recvfrom() error");
        else {
            printf("connection successful...\n");
            printf("Peer's IP : %u\n", ntohl(to_addr.sin_addr.s_addr));
            printf("Peer's Port : %d\n", ntohs(to_addr.sin_port));
        }
        send_len = sendto(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &to_addr, to_addr_len);
        if(send_len == -1)
            error_handling("sendto() error");
        printf("Disconnected...\n");
    }
    close(from_sock);
    return 0;
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
