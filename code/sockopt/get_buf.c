#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {
	
	int sock, state;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	socklen_t len, snd_buf, rcv_buf;
	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	if(state) error_handling("getsockopt() error");
	
	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if(state) error_handling("getsockopt() error");

	printf("INPUT buffer size : %d\n", rcv_buf);
	printf("OUTPUT buffer size : %d\n", snd_buf);
}

void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
