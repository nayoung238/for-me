#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <stdlib.h> /* exit */
#include <unistd.h> /* close */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_int, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <sys/types.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024
void error_handling(const char* message) {
	perror(message);
	exit(1);
}
int main(int argc, char* argv[]){
    
    if(argc != 3){
        printf("Usage : %s <addr>\n", argv[0]);
        exit(1);
    }
	
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	char *sendMsg, recvMsg[BUFF_SIZE];

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
    
    if(!inet_aton(argv[1], &addr.sin_addr))
        error_handling("Convergent error\n");
    
    struct hostent *host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
    if(!host) error_handling("gethostbyname() error");
    
    addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) error_handling("connect() error");

	sendMsg = "GET /webhp HTTP/1.1\r\nUser-Agent: Mozilla/4.0\r\nContent-type: text/html\r\nConnection:clos\r\n\r\n";
	if(write(sock, sendMsg, strlen(sendMsg)) == -1) error_handling("send() error");
	if(read(sock, recvMsg, BUFF_SIZE) == -1) error_handling("recv() error");
			
	printf("%s\n", recvMsg);
	close(sock);
	return 0;
}
