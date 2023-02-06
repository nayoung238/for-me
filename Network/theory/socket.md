## Protocol 프로토콜

**Protocol**이란 컴퓨터 상호간의 데이터 송수신에 필요한 통신규약으로 **socket 끼리 같은 규약을 가지고 있어야 통신이 가능**하다.<br>

과거 한정된 자원으로 효율적인 통신을 하기 위해 **2진수 비트 기반**으로 프로토콜이 만들어졌다. 
현재는 Application 레벨의 프로토콜인 HTTP, SMTP 같은 프로토콜은 **문자 기반**이다. 
문자로 헤더, 데이터 등을 표현하고 text 파일과 같은 데이터가 전달되기 때문에 효율성은 비트 기반 프로토콜 보다 떨어지지만 확장성이 높다.<br>

TCP 는 transport 계층에서, IP는 Network(Internet) 계층에서 별도로 동작하지만 함께 사용되는데 이를 **프로토콜 스택**이라고 한다.
**프로토콜 스택**은 4개의 부분으로 나누어진다.

- Physical 계층 : Ethernet 이더넷
- Network 계층 : 데이터가 목적지로 가기위한 계층으로 IP, ICMP, ARP 존재
- Transport 계층 : 잘린 패킷을 데이터 형태로 조합하는 계층으로 TCP, UDP 존재
- Application 계층 : SSH, DNS 등이 있다.
<br>

## Protocol family 프로토콜 체계

프로토콜도 종류에 따라 부류가 나눠지는데 그 부류를 **프로토콜 체계**라고 한다.

- ```PF_INET``` : IPv4 인터넷 프로토콜 체계
- ```PF_INET6``` : IPv6 인터넷 프로토콜 체계
- ```PF_LOCAL``` : 로컬 통신을 위한 UNIX 프로토콜 체계
- ```PF_PACKET``` : LOW Level 소켓을 위한 프로토콜 체계
- ```PF_IPX``` : IPX 노벨 프로토콜 체계
<br>

## sock() 소켓 생성

데이터 전송방식으로 소켓을 생성할 때 소켓의 타입을 지정한다.

- TCP (SOCK_**STREAM**) : 연결 지향형 소켓 타입
- UDP (SOCK_**DGRAM**) : 비연결 지향형 소켓 타입

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
// 성공 시 파일 디스크립터를, 실패 시 -1 리턴
```
- domain : socket이 사용할 프로토콜의 체계(protocol family) 정보
- type : socket의 데이터 전송방식으로 tcp, udp 인지 작성
- protocol : 컴퓨터간 통신에 사용되는 프로토콜 정보
<br>

IPv4 인터넷 프로토콜 체계에서 동작하는 연결지향형 데이터 전송 소켓을 생성하려면 아래와 같이 작성한다.<br>

```c
int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
```

<br>

IPv4 인터넷 프로토콜 체계에서 동작하는 비 연결지향형 데이터 전송 소켓을 생성하려면 아래와 같이 작성한다.<br>

```c
int udp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
```

첫 번째 인자인 domain과 두 번째 인자인 type에 의해 socket의 protocol이 결정되므로 3번째 인자인 protocol은 0 으로 전달해도 상관없다.<br><br>

### TCP (SOCK_STREAM)

- 전송 중간에 발생하는 **데이터 소멸을 막고 전송 순서를 보장**
- 데이터의 경계가 존재하지 않아 receiver 입장에선 **받는 단위 지정이 가능**
  - 자세한 내용: https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/boundary-of-tcp-transmission-data.md
- 소켓끼리의 연결은 1 대 1 구조
- 주로 FTP(File Transfer Protocol)에 사용

### UDP (SOCK_DGRAM)

- 전송순서 상관없고 빠른 속도를 지향
- 데이터 파손 및 손실 가능
- 데이터의 경계가 존재해 **read**에 대한 **write**를 반드시 호출해야 함 -> 한번에 다 받아야함
- 소켓끼리의 1 대 1 구조가 아님
  - 1개의 소켓이 여러 소켓과 데이터 송수신 가능
- 한 번에 전송할 수 있는 데이터의 크기가 제한됨
- 주로 media 전송에 사용
<br>

## bind() 주소 할당 -> server 사용

```c
#include <sys/socket.h>

int bind(int sockfd, (struct sockaddr*) &myaddr, socklen_t addrlen);
// 성공 시 0, 실패 시 -1 리턴
```
- sockfd : 소켓의 파일 디스크립터
- myaddr : 설정할 주소가 담긴 구조체를 ```sockaddr``` 구조체로 casting 
<br>
 
- 소켓에 주소를 할당해야 소켓끼리의 통신이 가능
- 소켓 주소정보는 IP, Port 번호로 구성
- 서버만 ```bind()``` 호출: 클라이언트가 서버에 접근하기 위해 서버의 주소를 알아야 하므로 ```bind()``` 호출로 명시
  - 클라이언트 소켓은 ```Connect()``` 호출하면 OS가 자동으로 설정함
  - 클라이언트 소켓의 주소를 명시할 필요없음
<br>

## listen() 연결요청이 가능함을 설정 -> server 사용

```c
#include <sys/type.h>

int listen(int sock, int backlog);
// 성공 시 0, 실패 시 -1 리턴
```
- sock : listen 소켓으로 설정할 소켓의 파일 디스크립터
- backlog : 연결요청 대기 큐의 크기 정보 전달 -> 5 작성시 클라이언트 연결 요청을 5개까지 대기시킬 수 있음
<br>

- 연결 요청이 가능함을 설정하는 라이브러리
- 서버만 호출하고 클라이언트는 호출할 필요없음
  <br>

## accept() 연결 요청 수락 -> server 사용

```c
#include <sys/socket.h>

int accept(int sockfd, (struct sockaddr*) &clnt_addr, socklen_t *addrlen);
// 성공 시 파일 디스크립터, 실패 시 -1 리턴
```

- 클라이언트가 연결을 요청하면 수락
- ```accept()```는 새로운 소켓을 생성해 해당 소켓의 파일 디스크립터 리턴
- 클라이언트와 서버 소켓이 직접적인 데이터 송수신을 하는 것이 아니고
- ```accept()``` 에서 **생성한 새로운 소켓이 데이터 송수신용 소켓**
- 서버 소켓은 클라이언트의 요청에 따라 새로운 소켓(데이터 송수신용)을 생성
  - 서버 소켓, 리스닝 소켓 같은 말
- 연결 요청을 수락하는 메소드이므로 서버만 사용
<br>

## connect() 연결을 요청함 -> client 사용

```c
#include <sys/socket.h>

int connect(int sockfd, struct sockaddr *serv_addr, socklen_t addrlen);
// 성공 시 0, 실패 시 -1 리턴
```

- 서버 소켓에게 연결을 요청할 때 사용하므로 클라이언트만 사용
- 클라이언트는 ```socket()```으로 소켓 생성후 주소 할당을 하지 않고 바로 ```connect()``` 호출
- 서로간의 통신을 위해 클라이언트도 주소는 필요함
- 클라이언트의 주소는 ```connect()``` 호출 시 커널이 알아서 지정해줌
  - IP 주소는 클라이언트가 사용하는 IP로 동일하게 설정될 것이고
  - port 번호를 커널이 적절하게 설정함
- 즉, 사용자는 클라이언트의 주소를 알 필요가 없어 ```bind()``` 를 호출하지 않아도 됨