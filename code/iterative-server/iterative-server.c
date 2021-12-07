#include <stdio.h>
#include <stdlib.h>     // exit
#include <fcntl.h>      // open, close
#include <unistd.h>     // read, write
#include <arpa/inet.h>  // htons
#include <sys/socket.h> // socket
#include <sys/types.h>

#define BUF_SIZE 1024
void error_handling(const char *message);

int main(int argc, char *argv[]){
    
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t addr_len;
    
    char buf[BUF_SIZE];
    int read_len;
    
    if(argc != 2) {
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 3) == -1)
        error_handling("listen() error");
    
    for(int i = 0; i < 3; ++i) {
        printf("new connect...\n");
        addr_len = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_len);
        
        if(clnt_sock == -1)
            error_handling("accept() error");
        
        while(read_len = read(clnt_sock, &buf, BUF_SIZE)) {
            if(read_len == -1)
                error_handling("read() error");
            write(clnt_sock, &buf, read_len);
        }
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}
void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
