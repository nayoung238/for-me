#include <stdio.h>
#include <stdlib.h>     // exit
#include <unistd.h>     // close sock
#include <sys/socket.h> // sock, getsockopt

void error_handling(char *message);

int main(int argc, char *argv[]) {
	
	int tcp_sock, udp_sock;
    int result, sock_type;
    socklen_t optlen;
    
	tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
	udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
	
	printf("SOCK_STREAM : %d\n", SOCK_STREAM);
	printf("SOCK_DGRAM : %d\n", SOCK_DGRAM);

    /*
    sock_type 을 초기화해야함.
    기존 값에 어떠한 연산을 적용시켜 할당하는 것 같은데 어떤 연산인지 찾지못함.
    일단은 0으로 초기화 한다.
    */
    sock_type = 0;
    
	result = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if(result == -1)
        error_handling("getsockopt() error!");
	printf("Socket type one : %d\n", sock_type);

	result = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if(result == -1)
        error_handling("getsockopt() error!");
	printf("Soctet type two : %d\n", sock_type);
    
    close(tcp_sock);
    close(udp_sock);
    return 0;
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
