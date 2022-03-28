# UDP (User Datagram Protocol)

UDP도 TCP와 같은 통신 프로토콜이다.<br>

TCP는 소켓끼리의 1대 1 연결이고, 소켓 전달시 SEQ, ACK 같은 신호로 데이터의 전송을 보장해준다.<br>
하지만 UDP는 데이터의 전송도 보장해주지 않고 그저 빠른 속도만 보장한다. 즉, 안전성보다 성능이 중요하다면 UDP를 사용하면된다.<br>

- 데이터의 전송을 보장하지 않아 SEQ, ACK 같은 신호가 없다.
- 데이터의 흐름을 제어하지 않고 일단 보낸다. -> flow control 없음
- 빠르게 보내는 것만 제공하므로 TCP보다 구조가 단순하다.
- 전송 체크를 따로 하지 않기 때문에 속도가 빠르다.
- 1대 1 방식의 TCP와 다르게 여러 host와 동시에 통신이 가능하다. (broadcast, multicast)
- 여러 host와 동시에 통신이 가능하므로 connection 이라는 개념이 적용되지 않아 통신할 때마다 수신자를 지정해야한다.


## UDP의 데이터 송수신 방법

- TCP 는 소켓끼리의 1대 1 연결이므로 데이터 송수신하는 동안 목적지가 정해져있다.
- UDP 는 1대 1 연결이 아니다.
- 동시에 여러 소켓과 연결이 가능하기 때문에 서버 소켓, 클라이언트 소켓이라는 구분도 없다.

### sendto()

```c
#include <sys/socket.h>

ssize_t sendto(int sock, void *buff, size_t nbytes, int flags,
                struct sockaddr *to, socklen_t addrlen);
// 성공 시 보낸 문자 수(byte), 실패 시 -1 return            
```

- sock : 데이터 전송에 사용될 UDP 소켓의 파일 디스크립터
- buff : 전송할 데이터를 저장하고 있는 버퍼 주소 값
- nbytes : 전송할 데이터 크기(바이트 단위)
- flags : 옵션 지정에 사용되는 매개변수로 지정할 옵션 없으면 0 전달
- to : 목적지 주소정보를 담고 있는 sockaddr 구조체 변수의 주소 값
- addrlen : 매개변수 to로 전달된 주소 값의 구조체 변수 크기(value)

UDP는 여러 소켓과 연결이 가능하므로 데이터를 전송할 때마다 **to** 매개변수에 목적지에 대한 정보를 전달해야한다.<br><br>

### recvfrom()

```c
#include <sys/socket.h>

ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags,
                    struct sockaddr *from, socklen_t *addrlen);
// 성공 시 받은 문자 수(byte), 실패 시 -1 return <br>
```

- sock : 데이터 수신에 사용될 UDP 소켓의 파일 디스크립터
- buff : 데이터 수신에 사용될 버퍼 주소 값
- nbytes : 수신할 최대 바이트 수 전달. buff가 가리키는 버퍼의 크기를 넘을 수 없음
- flags : 옵션 지정에 사용되는 매개변수로 지정할 옵션 없으면 0 전달
- from : 발신지 정보를 채워 넣을 sockaddr 구조체 변수의 주소 값
- addrlen : sockaddr 구조체의 크기를 할당한 pointer

from에 비어있는 sockaddr 구조체를 넘겨주면 커널에서 발신자 주소를 알아서 채워준다.<br><br>

### TCP / UDP 포트번호 관리

하나의 PC에 TCP 소켓과 UDP 소켓의 Port 번호가 같아도 상관없다.<br>
같은 PC에 존재하는 소켓이므로 IP는 같다. 하지만 Port 번호까지 같아도 상관없는 이유는 TCP/UDP 따로 관리되기 때문이다.<br><br>

## UDP to-client : echo-server 역할

```c
1  recv_len = recvfrom(from_sock, buf, BUF_SIZE, 0, (struct sockaddr*) &to_addr, &to_addr_len);
2  if(recv_len == -1)
3      error_handling("recvfrom() error");
4  else printf("connection successful...\n");
5        
6  send_len = sendto(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &to_addr, to_addr_len);
7  if(send_len == -1)
8      error_handling("sendto() error");
9    
10 close(from_sock);
```

> 전체 코드 : [https://github.com/evelyn82/network/blob/master/code/udp/to_client.c](https://github.com/evelyn82/network/blob/master/code/udp/to_client.c)<br>

TCP는 데이터의 경계가 없어 받고 싶은 만큼만 받아도 되지만 UDP는 데이터의 경계가 존재한다. 즉, 한번에 모두 읽기위해 recvfrom() 이 1번 호출된다.(line 8 )<br>

## UDP from-client : echo-client 역

```c
1   while(1) {
2         fputs("Insert message(1 to quit): ", stdout);
3         fgets(buf, BUF_SIZE, stdin);
4         if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
5             break;
6        
7         send_len = sendto(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &to_addr, sizeof(to_addr));
8         if(send_len == -1)
9             error_handling("sendto() error");
10    
11        from_addr_len = sizeof(from_addr_len);
12        recv_len = recvfrom(from_sock, buf, sizeof(buf), 0, (struct sockaddr*) &from_addr, &from_addr_len);
13        if(recv_len == -1)
14            error_handling("recvfrom() error");
15   
16        printf("message from server : %s\n", buf);
17   }
18   close(from_sock);
```

> 전체 코드 : [https://github.com/evelyn82/network/blob/master/code/udp/from_client.c](https://github.com/evelyn82/network/blob/master/code/udp/from_client.c)<br>

UDP는 소켓끼리 1대 1 연결이 아니므로 데이터를 전송하기 위해 sendto()를 호출할 때마다 상대방 주소를 작성해야하지만, 자신의 IP, Port 주소는 **sendto() 첫 호출 때 OS가 알아서 할당**해준다. 즉, sendto()를 호출했는데 클라이언트의 IP,Port 번호가 이미 할당된 상태라면 넘어간다.<br>
echo server와 마찬가지로 UDP는 데이터의 경계가 존재해 모든 데이터를 한번에 읽기 위해 recvfrom()를 1번 호출한다.<br>

이렇게 UDP는 여러 소켓과 통신이 가능하므로 sendto()를 호출할 때마다 UDP 소켓에 목적지의 IP, Port번호를 등록하고 전송이 끝나면 목적지 주소를 삭제한다. 이를 **unconnected UDP**라고 한다.<br><br>

## connected UDP socket

```c
1  sock = socket(PF_INET, SOCK_DGRAM, 0);
2  if(sock == -1)
3       error_handling("sock() error");
4  
5  memset(&serv_adr, 0, sizeof(serv_adr));  
6  serv_adr.sin_family = AF_INET;
7  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
8  serv_adr.sin_port = htons(atoi(argv[2]));
9 
10 connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
```

UDP 소켓은 여러 UDP 소켓과 통신이 가능하므로 sendto()를 호출할 때마다 상대방의 주소를 작성해야한다. 하지만 connect()를 사용하면 UDP 소켓에 상대방의 IP, Port 주소가 고정 등록되어 sendto()가 아닌 read(), write()로 데이터 송수신이 가능하다.<br>
즉, sendto()를 사용하지 않으니 데이터를 보낼 때마다 상대방 주소를 작성할 필요가 없는 소켓을 connected UDP 소켓이라 한다.<br>

