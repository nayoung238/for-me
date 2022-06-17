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

    int server_sock;
    struct sockaddr_in client_addr, server_addr, server_my_addr;
    socklen_t client_addr_len, server_addr_len;

    char buf[BUF_SIZE];
    ssize_t send_len, recv_len;

    if(argc != 1) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(9190);

    if(bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
       error_handling("bind() error");

    if(getsockname(server_sock, (struct sockaddr*) &server_my_addr, &server_addr_len) == -1)
        error_handling("getsockname() error");

    printf("IP : %u\n", ntohl(server_addr.sin_addr.s_addr));
    printf("Port : %hd\n", ntohs(server_addr.sin_port));

    for(int i = 0; i < 3; ++i) {
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr_len = sizeof(client_addr);

        recv_len = recvfrom(server_sock, buf, BUF_SIZE, 0, (struct sockaddr*) &client_addr, &client_addr_len);
        if(recv_len == -1)
            error_handling("recvfrom() error");
        else {
            printf("connection successful...\n");
            printf("Peer's IP : %u\n", ntohl(client_addr.sin_addr.s_addr));
            printf("Peer's Port : %d\n", ntohs(client_addr.sin_port));
        }

        printf("\nread result: %s\n", buf);
        send_len = sendto(server_sock, buf, sizeof(buf), 0, (struct sockaddr*) &client_addr, client_addr_len);
        if(send_len == -1)
            error_handling("sendto() error");
        printf("Disconnected...\n\n");
    }
    close(server_sock);
    return 0;
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
