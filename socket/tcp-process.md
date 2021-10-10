# TCP 와 UDP

TCP/IP 프로토콜 스택이란 **인터넷 기반의 데이터 송수신을 목적으로 설계된 스택**이다.<br>
OSI 7계층에서 상위 3개의 계층을 Application 계층이라하고 나머지 하위 4계층을 Data flow 계층이라고 한다.<br>
Data flow 계층인 하위 4개의 계층에서 데이터 송수신 과정을 전문적으로 나눈다.<br>

- 4계층 : TCP/UDP 프로토콜로 end to end (출발지와 최종 도착지)
- 3계층 : IP(Internet Protocol)로 소켓이 전송되는 **최적의 라우터 경로**를 제공
- 2계층 : Link 계층으로 P2P (출발지와 라우터, 라우터와 라우터, 라우터와 도착지처럼 근접한 사이)

link 계층인 2계층과 TCP/UDP 계층인 4계층은 소켓 흐름과 오류를 제어한다.<br>
개발자는 파일 전송이면 tcp, 영상 전송이면 udp를 선택하는 것처럼 Application에 따른 4계층의 프로토콜을 선택하고, 2계층와 3계층은 표준으로 운영된다.<br><br>

## TCP / UDP 계층

출발지와 최종 목적지까지(end to end)의 데이터 송수신 계층이므로 **transport** 계층이라고도 한다.<br>
TCP는 데이터의 전송을 보장하는 신뢰성있는 프로토콜이고, UDP는 데이터의 전송을 보장하지 않는 프로토콜이다.<br>

TCP는 데이터의 전송을 보장하기 때문에 UDP보다 복잡한 프로토콜이며 receiver에게 패킷을 전송했을 때 receiver로부터 전송한 패킷에 대한 응답을 받지 못하면 time out으로 처리해 다시 보낸다.<br><br>

## TCP 서버의 기본적인 함수호출 순서

1. socket() : 소켓 생성
2. bind() : 소켓 주소할당
3. listen() : 연결요청 대기상태로 전환
4. accept() : 연결허용 및 데이터 송수신만을 처리하는 새로운 소켓이 생성
5. read()/write() : 데이터 송수신
6. close() : 연결 종료

과정 2의 bind()를 호출하면 주소가 할당된 소켓을 얻는다.<br>과정 3인 listen() 호출을 통해 연결요청이 가능한 상태가 된다.<br>

### 과정3 : listen()

주소가 할당된 소켓이 연결호출 가능한 상태가 되기 위해 listen()을 호출해야한다.<br>
연결요청도 데이터 전송이기 때문에 소켓이 필요하며 이를 **서버 소켓(리스닝 소켓)** 이라고 한다.<br>
즉, listen() 호출을 통해 소켓을 서버 소켓이 되게 만든다.<br>

```c
#include <sys/type.h>

int listen(int sock, int backlog);
```
연결요청 데이터를 보낸다 해서 바로 처리되는게 아니다.<br>
일단 연결요청 대기 큐에 연결요청된 데이터를 대기 시켰다가 accept 해주는 과정이 필요하다.<br>

- sock : 연결 요청 대기상태에 두고자 하는 소켓의 파일 디스크립터 전달. 즉, 전달된 디스크립터의 소켓이 서버소켓이다.
- backlog : 연결 요청 대기 queue의 크기를 전달. 5가 전달되면 클라이언트의 연결을 5개까지만 대기시킬 수 있다는 의미이다. 연결 요청 대기큐는 linked list 구조이다.

listen() 성공시 ```return 0```, 실패시 ```return -1```<br><br>

## 클라이언트의 연결 요청 수락

연결요청 대기큐에 있는 클라이언트의 연결요청을 허용하기 위해 accpet() 으로 데이터 송수신 용 소켓을 새로 생성한다.<br>
accept()으로 생성된 소켓을 대상으로만 데이터의 송수신이 진행된다.<br>

```c
#include <sys/type.h>

int accpet(int sock, struct sockaddr * addr, socklen_t * addrlen);
```

- sock : 서버 소켓의 파일 디스크립터 전달
- addr : 연결요청한 클라이언트의 주소정보를 담을 sockaddr 구조체의 주소 값을 전달
- addrlen : 전달된 수조의 변수 크기를 바이트 단위로 전달

accept() 성공시 ```새로 생성된 소켓의 파일 디스크립터를 return```, 실패시 ```return -1```<br>

### struct sockaddr * addr

매개변수 중 sockaddr 구조체인 addr에 클라이언트의 주소정보를 채운다.<br>
처음 전달된 상태는 비어있는 sockaddr 구조체를 전달하고 커널에서 클라이언트 정보를 채운다.<br>
즉, accept() 호출이 완료되면 전달된 addr에 클라이언트 주소가 채워져있다.<br><br>

accpet() 호출로 생성된 소켓은 인자로 전달된 sock과 다름을 주의하고!! 새로 생성된 소켓의 파일 디스크립터를 return 한다.<br>
반환된 파일 디스크립터로 read()/write()를 수행한다.<br>
참고로 accpet() 의 결과로 만들어진 데이터 송수신용 소켓의 port번호는 커널과 커널사이의 문제이므로 개발자가 신경쓸 필요없다.<br>
클라이언트에서 connect를 요청하면 클라이언트의 IP, Port 번호가 자동으로 할당되기 때문이다.<br><br>

## TCP 클라이언트 Connect 호출

클라이언트는 소켓을 생성하고 생성된 소켓으로 연결을 요청하기 위해 connect()를 호출하는게 전부다.<br>
connect() 호출시 연결할 서버의 주소정보를 함께 전달한다.<br>

```c
#include <sys/socket.h>

int connect(int sock, const struct sockaddr * servaddr, socklen_t addrlen);
```
- sock : 클라이언트 소켓의 파일 디스크립터 전달
- servaddr : 연결하려는 서버소켓 주소
- addrlen : servaddr에 전달된 주소의 변수 크기를 바이트 단위로 전달

<br>

## TCP 클라이언트의 함수호출 순서

1. socket() : 소켓 생성
2. connect() : 연결 요청
3. read()/write() : 데이터 송수신
4. close() : 연결종료

### 과정2 : connect()

connect()에는 2가지 return이 존재한다.<br>

1. read()/write() 가능한 상태로 return<br>
   연결하고자 하는 서버의 backlog가 모두 채워져 있지 않으면 서버의 accpet()을 호출할 수 있는 상태이다.<br>서버의 accpet() 까지 호출해 데이터 송수신용 소켓을 생성해 클라이언트와의 연결까지 처리한 다음 return 한다.<br>이 경우는 바로 read, write 수행이 가능한 상태이다.<br>
2. read()/write() 불가능한 상태로 return<br>
   연결하고자 하는 서버의 연결요청 대기큐가 모두 차있으면 서버의 backlog에 기록만하고 되돌아 오기 때문에 read, write를 할 수 없는 상태이다.<br><br>

## TCP 기반 서버, 클라이언트의 함수호출 관계

![png](/_img/tcp_server_client_process.png)
<br><br>

## close() 호출 시 TCP 소켓의 입출력 버퍼

소켓을 생성하면 입력버퍼와 출력버퍼가 생기고 이를 통해 데이터를 송수신한다.<br>
그럼 입력버퍼나 출력버퍼에 데이터가 있는 상태에서 close()하게 되면 남은 데이터는 어떻게 될까?<br>

- 입력버퍼에 데이터가 있는데 소켓을 close() 한 경우 : 입력버퍼에 있는 데이터는 무시된다.
- 출력버퍼에 데이터가 있는데 소켓을 close() 한 경우 : 출력버퍼에 있는 데이터를 상대방의 입력버퍼에 모두 전달한 다음 close()를 수행한다.

TCP는 데이터의 전송을 보장해 주기 때문에 출력 버퍼에 있는 데이터를 모두 처리하고 close()를 수행한다.<br>
더 자세한 내용 : [4-way handshake](https://github.com/evelyn82/network/blob/master/socket/4-way-handshake.md)