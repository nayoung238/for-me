# UDP (User Datagram Protocol)

- TCP 는 소켓끼리의 1대 1 연결이므로 데이터 송수신하는 동안 목적지가 정해져있다.
- UDP 는 1대 1 연결이 아니다.
- 동시에 여러 소켓과 연결이 가능하기 때문에 서버 소켓, 클라이언트 소켓이라는 구분도 없다.

<br>

### sendto()

```c
#include <sys/socket.h>

ssize_t sendto(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
// 성공 시 보낸 문자 수(byte), 실패 시 -1 return            
```

- sock : 데이터 전송에 사용될 UDP 소켓의 파일 디스크립터
- buff : 전송할 데이터를 저장하고 있는 버퍼 주소 값
- nbytes : 전송할 데이터 크기(바이트 단위)
- flags : 옵션 지정에 사용되는 매개변수로 지정할 옵션 없으면 0 전달
- to : 목적지 주소정보를 담고 있는 ```sockaddr``` 구조체 변수의 주소 값
- addrlen : 매개변수 to로 전달된 주소 값의 구조체 변수 크기(value)

UDP는 여러 소켓과 연결이 가능하므로 데이터를 전송할 때마다 **to** 매개변수에 **목적지에 대한 정보를 전달해야한다**.

<br>

### recvfrom()

```c
#include <sys/socket.h>

ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen);
// 성공 시 받은 문자 수(byte), 실패 시 -1 return <br>
```

- sock : 데이터 수신에 사용될 UDP 소켓의 파일 디스크립터
- buff : 데이터 수신에 사용될 버퍼 주소 값
- nbytes : 수신할 최대 바이트 수 전달 (**buff가 가리키는 버퍼의 크기를 넘을 수 없음**)
- flags : 옵션 지정에 사용되는 매개변수로 지정할 옵션 없으면 0 전달
- from : 발신자 정보를 채워 넣을 ```sockaddr``` 구조체 변수의 주소 값
- addrlen : ```sockaddr``` 구조체의 크기를 할당한 pointer

from에 비어있는 ```sockaddr``` 구조체를 넘겨주면 커널에서 발신자 주소를 알아서 채워준다.

<br>

### TCP / UDP 포트번호 관리

같은 PC에 존재하는 소켓이므로 IP는 같다.<br>
하나의 PC에 TCP 소켓과 UDP 소켓의 Port 번호가 같아도 상관없다.<br>
Port 번호까지 같아도 상관없는 이유는 TCP/UDP 따로 관리되기 때문이다.

<br>

## UDP to-client : echo-server 역할

```c
recv_len = recvfrom(from_sock, buf, BUF_SIZE, 0, (struct sockaddr*) &to_addr, &to_addr_len);
if(recv_len == -1) error_handling("recvfrom() error");
else printf("connection successful...\n");
        
send_len = sendto(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &to_addr, to_addr_len);
if(send_len == -1) error_handling("sendto() error");
   
close(from_sock);
```

- 전체 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/udp/server.c

TCP는 데이터의 경계가 없어 받고 싶은 만큼만 받아도 되지만 UDP는 데이터의 경계가 존재한다. 
즉, ```sendto()``` 1번 호출하면  ```recvfrom()``` 1번 호출로 모든 데이터를 받아야 한다.

<br>

## UDP from-client : echo-client 역

```c
while(1) {
    fputs("Insert message(1 to quit): ", stdout);
    fgets(buf, BUF_SIZE, stdin);
    if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
        break;
       
    send_len = sendto(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &to_addr, sizeof(to_addr));
    if(send_len == -1)
        error_handling("sendto() error");
   
    from_addr_len = sizeof(from_addr_len);
    recv_len = recvfrom(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &from_addr, &from_addr_len);
    if(recv_len == -1)
        error_handling("recvfrom() error");
  
    printf("message from server : %s\n", buf);
}
close(from_sock);
```

- 전체 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/udp/client.c

UDP는 소켓끼리 1대 1 연결이 아니므로 데이터를 전송하기 위해 ```sendto()```를 호출할 때마다 **상대방 주소를 설정**해야한다.<br>
UDP 통신에서 보내는 입장의 **IP**와 **Port** 번호는 ```sendto()``` **첫 호출 때 OS가 알아서 할당**해준다.<br>
즉, ```sendto()```를 호출할 때 보내는 입장의 **IP** 와 **Port** 번호가 이미 할당된 상태라면 그냥 넘어가고 그게 아니라면 할당한다.<br>
echo server와 마찬가지로 UDP는 데이터의 경계가 존재해 모든 데이터를 한번에 읽기 위해 ```recvfrom()``` 를 1번 호출한다.<br>

이렇게 UDP는 여러 소켓과 통신이 가능하므로 ```sendto()``` 를 호출할 때마다 UDP 소켓에 목적지의 IP, Port 번호를 등록하고 전송이 끝나면 목적지 주소를 삭제한다. 
이를 **unconnected UDP**라고 한다.

<br>

## connected UDP socket

```c
sock = socket(PF_INET, SOCK_DGRAM, 0);
if(sock == -1)
    error_handling("sock() error");

memset(&serv_adr, 0, sizeof(serv_adr));  
serv_adr.sin_family = AF_INET;
serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
serv_adr.sin_port = htons(atoi(argv[2]));

connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
```

UDP 소켓은 여러 UDP 소켓과 통신이 가능하므로 ```sendto()```를 호출할 때마다 상대방의 주소를 작성해야한다. 
하지만 ```connect()```를 사용하면 UDP 소켓에 상대방의 IP 와 Port 번호가 고정 등록되어 ```sendto()``` 가 아닌 ```read()``` 와 ```write()```로도 데이터 송수신이 가능하다.<br>
즉, ```sendto()```를 사용하지 않으니 데이터를 보낼 때마다 **상대방 주소를 작성할 필요가 없는 소켓을 connected UDP 소켓**이라 한다.

