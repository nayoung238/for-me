#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // atoi()
#include <unistd.h>  // close(sock)
#include <netinet/in.h> // INADDR_ANY
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
void error_handling(const char *message);

int main(int argc, char *argv[]) {
    
    int from_sock;
    struct sockaddr_in from_addr, to_addr;
    socklen_t from_addr_len;
    
    char buf[BUF_SIZE];
    ssize_t send_len, recv_len;
    
    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    from_sock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&from_addr, 0, sizeof(from_addr));
    from_addr.sin_family = AF_INET;
    from_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    from_addr.sin_port = htons(9190);
    
    if(bind(from_sock, (struct sockaddr*) &from_addr, sizeof(from_addr)) == -1)
        error_handling("bind() error");
    
    memset(&to_addr, 0, sizeof(to_addr));
    to_addr.sin_family = AF_INET;
    to_addr.sin_addr.s_addr = htonl(atoi(argv[1])); // 0.0.0.0
    to_addr.sin_port = htons(atoi(argv[2]));
    
    while(1) {
        fputs("Insert message(1 to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
            break;
        
        send_len = sendto(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &to_addr, sizeof(to_addr));
        if(send_len == -1)
            error_handling("sendto() error");
    
        from_addr_len = sizeof(from_addr_len);
        recv_len = recvfrom(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &from_addr, &from_addr_len);
        if(recv_len == -1)
            error_handling("recvfrom() error");
    
        printf("message from server : %s\n", buf);
    }
    close(from_sock);
    return 0;
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
