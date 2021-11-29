# Protocol 프로토콜

컴퓨터 상호간의 데이터 송수신에 필요한 통신규약으로 socket 끼리 같은 규약을 가지고 있어야 통신이 가능하다.<br>
과거 한정된 자원으로 효율적인 통신을 하기 위해 2진수 비트 기반으로 프로토콜이 만들어졌다. 현재는 Application 레벨의 protocol인 HTTP, SMTP 같은 프로토콜은 문자 기반이다. 문자로 헤더, 데이터 등을 표현하고 text 파일과 같은 데이터가 전달되기 때문에 효율성은 비트 기반 프로토콜 보다 떨어지지만 확장성이 높다.<br>

TCP는 transport 계층에서, IP는 Network(Internet) 계층에서 별도로 동작하지만 함께 사용되는데 이를 ```프로토콜 스택```이라고 한다. 프로토콜 스택은 4개의 부분으로 나누어진다.<br>

- physical 계층 : Ethernet 이더넷
- network 계층 : 데이터가 목적지로 가기위한 계층으로 IP, ICMP, ARP가 있다.
- transport 계층 : 잘린 패킷을 데이터 형태로 조합하는 계층으로 TCP, UDP가 있다.
- application 계층 : SSH, DNS 등이 있다.
<br>

## Protocol family 프로토콜 체계

프로토콜도 종류에 따라 부류가 나눠지는데 그 부류를 프로토콜 체계라고 한다.<br>

- ```PF_INET``` : IPv4 인터넷 프로토콜 체계
- ```PF_INET6``` : IPv6 인터넷 프로토콜 체계
- ```PF_LOCAL``` : 로컬 통신을 위한 UNIX 프로토콜 체계
<br>

## type 소켓 타입 

데이터 전송방식으로 소켓을 생성할 때 소켓의 타입을 지정한다.<br>

- TCP (SOCK_**STREAM**) : 연결 지향형 소켓 타입
- UDP (SOCK_**DGRAM**) : 비연결 지향형 소켓 타입

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
// 성공 시 파일 디스크립터를, 실패 시 -1 리턴
```
- domain : socket이 사용할 프로토콜의 체계(protocol family)정보
- type : socket의 데이터 전송방식으로 tcp, udp인지 작성
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

첫 번째 인자인 domain과 두 번째 인자인 type에 의해 socket의 protocol이 결정되므로 3번째 인자인 protocol은 0으로 전달해도 상관없다.<br><br>

## TCP (SOCK_STREAM)

- 전송 중간에 발생하는 데이터 소멸을 막고 전송 순서를 보장
- 데이터의 경계가 존재하지 않아 receiver 입장에선 받는 단위 지정이 가능
  - 자세한 내용 : [https://github.com/evelyn82/network/blob/master/socket/boundary-of-tcp-transmission-data.md](https://github.com/evelyn82/network/blob/master/socket/boundary-of-tcp-transmission-data.md)
- 소켓끼리의 연결은 1 대 1 구조
- 주로 FTP(File Transfer Protocol)에 사용
<br>

## UDP (SOCK_DGRAM)

- 전송순서 상관없고 빠른 속도를 지향
- 데이터 파손 및 손실 가능
- 데이터의 경계가 존재해 read에 대한 write를 반드시 호출해야 함 (한번에 다 받아야함)
- 소켓끼리의 1 대 1 구조가 아님
- 한 번에 전송할 수 있는 데이터의 크기가 제한됨
- 주로 media 전송에 사용




