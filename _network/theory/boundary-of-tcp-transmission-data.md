## TCP가 전송하는 데이터는 경계가 존재하지 않는다.

- TCP 로 전달하는 데이터의 특징은 **경계가 존재하지 않음**
  - 경계가 존재하지 않아 receiver 는 원하는 크기만큼 끊어서 받을 수 있음
  - ```read()``` 여러번 호출가능
<br>

- UDP로 전달되는 데이터는 **경계가 존재**
  - 데이터 경계가 존재하기 때문에 주는 만큼 다 받아야 함
  - ```write()``` 1번 호출

<br>

```c
while(read_len = read(sock, &message[idx++], 1)) {
    if(read_len == -1) {
		error_handling("read() error!");
		break;
	}
	str_len += read_len;
}
```

- client 전체 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/tcp-data-boundary/tcp_client_read_1byte.c
- server 전체 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/tcp-data-boundary/tcp_server.c

<br>

```tcp_client_read_1byte.c``` 의 일부코드이다. 해당 클라이언트는 전달받은 데이터를 **1byte** 씩 읽는 방식이다.<br>

![png](/_network/_img/tcp_result.png)<br>

![png](/_network/_img/helloworld_length.png)<br>

- 1byte 씩 몇번 읽었는지 확인한 결과 13
- null 까지 읽고 ```EOF(End Of File)```에 접근한 순간 return 0 하기 때문에 13

<br>

```c
char message[30];

str_len = read(sock, message, sizeof(message) - 1);
```

- client 전체 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/tcp-data-boundary/tcp_client_read_buffer_size.c
- server 전체 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/tcp-data-boundary/tcp_server.c

```tcp_client_read_buffer_size.c``` 의 일부코드이다. 해당 클라이언트는 전달받고자 하는 크기인 30 보다 1개 적은 **29** 만큼만 1번 읽는다.<br>
여기서 1을 제외한 이유는 **null 을 처리해야 되기 때문**이다.<br>

```c
sock = socket(PF_INET, SOCK_STREAM, 0);       // client
serv_sock = socket(PF_INET, SOCK_STREAM, 0);  // server
```
client 와 server 모두 TCP 방식을 사용하기 위해 SOCK_STREAM 으로 설정했기 때문에 자신이 원하는 사이즈만큼 읽을 수 있던 것이다.