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

    int client_sock;
    struct sockaddr_in client_addr, server_addr;
    socklen_t client_addr_len;

    char buf[BUF_SIZE];
    ssize_t send_len, recv_len;

    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&client_addr, 0, sizeof(client_addr));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(atoi(argv[1])); // 0.0.0.0
    server_addr.sin_port = htons(atoi(argv[2]));

    while(1) {
        fputs("Insert message(1 to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
            break;

        send_len = sendto(client_sock, buf, sizeof(buf), 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
        if(send_len == -1)
            error_handling("sendto() error");

        client_addr_len = sizeof(client_addr_len);
        recv_len = recvfrom(client_sock, buf, sizeof(buf), 0, (struct sockaddr*) &client_addr, &client_addr_len);
        if(recv_len == -1)
            error_handling("recvfrom() error");

        printf("message from server : %s\n", buf);
    }
    close(client_sock);
    return 0;
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
