#include <stdio.h>
#include <stdlib.h>     // exit
#include <unistd.h>     // close socket
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {

    int sock, result;
    socklen_t len, snd_buf = 0, recv_buf = 0;
    
	sock = socket(PF_INET, SOCK_STREAM, 0);

    // read buf
    len = sizeof(recv_buf);
    result = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&recv_buf, &len);
    if(result == -1)
        error_handling("getsockopt() error");
    printf("Input buffer size : %d\n", recv_buf);
    
    recv_buf = 30;
	result = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&recv_buf, sizeof(recv_buf));
	if(result == -1)
        error_handling("setsockopt() error");
	
    len = sizeof(recv_buf);
    result = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&recv_buf, &len);
    if(result == -1)
        error_handling("getsockopt() error");
    printf("Changed Input buffer size : %d\n", recv_buf);
    
    // send buf
    snd_buf = 30;
	result = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
	if(result == -1)
        error_handling("setsockopt() error");

	len = sizeof(snd_buf);
	result = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	if(result == -1)
        error_handling("getsockopt() error");
	printf("Changed Output buffer size : %d\n", snd_buf);
    
    close(sock);
	return 0;
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

