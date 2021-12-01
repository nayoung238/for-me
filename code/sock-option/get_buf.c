#include <stdio.h>
#include <stdlib.h>     // exit
#include <unistd.h>     // close sock
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {
	
	int sock, result;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	socklen_t len, snd_buf, rcv_buf;
	len = sizeof(snd_buf);
	result = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	if(result == -1)
        error_handling("getsockopt() error");
    printf("OUTPUT buffer size : %d\n", snd_buf);
	
	len = sizeof(rcv_buf);
	result = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if(result == -1)
        error_handling("getsockopt() error");
	printf("INPUT buffer size : %d\n", rcv_buf);
    
    close(sock);
    return 0;
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
