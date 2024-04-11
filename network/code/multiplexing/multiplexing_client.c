#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server_addr;

    char buffer[30];
    int str_len, index, read_len;

    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    str_len = index = 0;
    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        error_handling("connect() error");

    while(1) {
        fputs("Insert message(1 to quit): ", stdout);
        fgets(buffer, sizeof(buffer) - 1, stdin);
        if(!strcmp(buffer, "q\n") || !strcmp(buffer, "Q\n"))
            break;

        str_len = write(sock, buffer, sizeof(buffer) - 1);
        if(str_len == -1)
            error_handling("write() error");

        str_len = read(sock, buffer, sizeof(buffer) - 1);
        if(str_len == -1)
            error_handling("read() error");

        printf("Message from Server: %s\n", buffer);
        printf("Function read call count: %d\n", str_len);
    }

    close(sock);
    return 0;
}