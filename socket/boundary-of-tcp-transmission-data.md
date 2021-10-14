# TCP가 전송하는 데이터는 경계가 존재하지 않는다.

TCP로 전달되는 데이터의 특징은 경계가 존재하지 않는다.<br>
하지만 UDP로 전달되는 데이터는 경계가 존재한다.<br>
즉, TCP로 전달되는 데이터는 읽고 싶은 만큼 나눠서 받아도 되고, UDP로 전달되는 데이터는 무조건 주는 만큼 다 받아야 한다.<br>
```c
while(read_len=read(sock, &message[idx++], 1)){
    if(read_len==-1){
		error_handling("read() error!");
		break;
	}
	str_len+=read_len;
}
```
> tcp_client.c 전체 코드 : [https://github.com/evelyn82/network/blob/master/tcp/tcp_client.c](https://github.com/evelyn82/network/blob/master/tcp/tcp_client.c) <br>

tcp_client.c 의 일부코드이다. 해당 client는 전달받은 데이터를 1byte씩 읽는 방식이다.<br><br>
![png](/_img/chap2_tcp_result.png)<br>
몇번 읽었는지 확인하기 위해 strlen을 출력한 결과 13이 출력되었다.<br>
![png](/_img/helloworld_length.png)<br>
null까지 읽고 ```EOF(End Of File)```에 접근한 순간 return 0 하기 때문에 13이 출력되었다.<br><br>
```c
char message[30];

str_len=read(sock, message, sizeof(message)-1);
```
> tcp_server.c 전체 코드 : [https://github.com/evelyn82/network/blob/master/tcp/tcp_server.c](https://github.com/evelyn82/network/blob/master/tcp/tcp_server.c) <br>

hello_client.c 의 일부코드이다.<br>해당 client는 전달받고자 하는 크기인 30보다 1개 적은 29만큼만 읽는 방식이다.<br>
여기서 1을 제외한 이유는 null을 처리해야 되기 떄문이며, 해당 방식은 1번에 모두 읽는다.<br>tcp_client는 13번동안 데이터를 읽었다면, hello_client는 1번에 데이터를 읽었다.<br>
```c
sock=socket(PF_INET, SOCK_STREAM, 0);       // client
serv_sock=socket(PF_INET, SOCK_STREAM, 0);  // server
```
client와 server 모두 TCP를 사용하기 위해 SOCK_STREAM으로 설정했기 때문에 자신이 원하는 사이즈만큼 읽을 수 있던 것이다.<br>