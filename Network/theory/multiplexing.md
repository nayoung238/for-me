# I/O multiplexing
    
다중 접속 서버란 둘 이상의 클라이언트에게 동시에 접속을 허용하여 동시에 둘 이상의 클라이언트에게 서비스를 제공하는 서버이다. 다중 접속 서버 구현 방법은 다음과 같다.<br>

- multiprocess 기반 서버 : ```fork()```로 프로세스를 생성해 서비스 제공
- multithread 기반 서버 : 클라이언트 만큼의 스레드를 생성해 서비스 제공 (**stack만 따로 사용하고 나머지는 부모와 공유**하기 때문에 multiprocess보다 간단한 방식)
- multiplexing 기반 서버 : 하나의 프로세스가 file descriptor로 다수 입출력 대상을 묶어서 관리하는 방식으로 서비스 제공

multiprocess 기반은 ```fork()``` 할 때마다 부모와 자식은 서로 독립적인 메모리를 할당해줘야 하므로 성능이 저하된다. 
멀티 프로세스의 흐름을 고려해야 하기 때문에 구현도 쉽지 않다. 
또한 프로세스 간 통신이 필요한 상황에선 서버 구현이 더 복잡해지는 등 여러 문제점이 있다.<br>

이는 **1개의 프로세스가 소켓으로 다수의 클라이언트에게 서비스를 하는 I/O multiplexing** 을 사용해 해결한다.

<br>

##  file descriptor

실행 파일, pipe, socket 등 모두 파일로 취급되며 번호가 할당된다. 
이 번호는 PCB의 file descriptor table에 등록되고 해당 번호로 리소스를 사용한다. 
0부터 2까지는 **stdin(0), stdout(1), stderr(2)** 로 지정되어 있어 3번부터 사용자 할당이 가능하다.<br>

multiplexing은 file descriptor 를 이용해 입출력 대상을 묶어서 관리한다. 
관련된 모든 것을 직접 감시하지 않고 관찰 대상을 지정해 신호를 받아 처리하는 비동기 방식이며, 신호가 올 때까지 Block 된다.
그러므로 I/O multiplexing은 Blocking + Async이다. 

<br>

## select()

```select()``` 메서드로 다음 3가지 상태에 대해 처리한다.

- **수신한 데이터를 지닌 소켓이 존재하는가**
- **blocking 되지 않고 데이터 전송이 가능한 소켓이 존재하는가**
- **예외 상황이 발생한 소켓이 존재하는가** 

이벤트가 발생하거나 **timeval**에 지정한 시간을 넘어갈 경우 block 상태인 프로세스를 깨워 return 한다.

```c
#include <sys/select.h>
#include <sys/time.h>

int select(int maxfd, fd_set *readset, fd_set *writeset, fd_set *exceptset, const struct timeval *timeout);
// 성공 시 0 이상(변화가 발생한 파일 디스크립터 수), 실패 시 -1 return
```

- maxfd : 관심 대상 중 가장 큰 파일 디스크립터 + 1
- readset : fd_set형 변수에 **수신된 데이터의 존재 여부**에 관심 있는 파일 디스크립터 정보를 모두 등록해 주소값 전달
- writeset  : fd_set형 변수에 **blocking 없는 데이터 전송의 가능 여부**에 관심 있는 파일 디스크립터 정보를 모두 등록해 주소값 전달
- exceptset : fd_set형 변수에 **예외 발생 여부**에 관심 있는 파일 디스크립터 정보를 모두 등록해 주소값 전달
- timeout : select() 호출 이후 무한정 blocking 상태에 빠지지 않도록 time-out 구조체 설정 (timeval 파라미터에 NULL 설정 시 무한히 block 상태)

```c
struct timeval{
    long tv_sec;  // seconds
    long tv_usec; // microseconds
}
```
<br>

파일 디스크립터를 설정하는 방법은 다음과 같다.

- ```void FD_ZERO(fd_set *set)``` : set의 모든 비트를 0으로 초기화
- ```void FD_SET(int fd, fd_set *set)``` : fd 번 비트를 1로 초기화
- ```void FD_CLR(int fd, fd_set *set)``` : fd 번 비트를 0으로 초기화
- ```void FD_ISSET(int fd, fd_set *set)``` : fd 번 비트가 1인지 검사 (1인 경우 양수 리턴)

<br>

## 소켓 이벤트 발생

> water-mark 설명 : https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=ablax&logNo=100021152758

### Readable

- 수신 버퍼에 데이터가 존재할 때 (low-water mark 이상일 때)
- read-half of the connection closed (상대의 **EOF**를 받아 ```read()```가 0을 리턴)
- new connection ready for listening socket
- pending error

### writable

- space available for writing (low-water mark 보다 클 때, 2048B)
- write-half of the connection closed (SIGPIPE 발생, 연결이 끊어진 소켓에 쓰는 경우)
- pending error

### Exception

- Out of band data 

<br>

## stdin 에 대한 select() 

키보드의 입력만 관심 대상으로 설정해 ```select()``` 호출로 인한 변화를 확인해봤다.

```c
1   fd_set reads, temps;
2   FD_ZERO(&reads);
3   FD_SET(0, &reads); // 0(stdin)
4
5   while(1){
6      temps = reads;
7      timeout.tv_sec = 5;
8      timeout.tv_usec = 5000;
9      result = select(1, &temps, NULL, NULL, &timeout);
10
11     if(result == -1) {
12          puts("select() error");
13          break;
14     }
15     else if(result == 0) {
16         puts("Time-out!");
17         break;
18     }
19     else {
20         if(FD_ISSET(0, &temps)) {
21               str_len = read(0, buf, BUF_SIZE);
22               buf[str_len] = 0;
23               printf("message from console : %s", buf);
24         }
25     }
26  }return 0;
```
> 전제 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/multiplexing/select_stdin.c

- line 3 : 키보드 입력을 관심 대상으로 설정
- line 6 : ```select()``` 의 readset은 reference를 전달하므로 값이 계속 바뀌므로 이를 방지하기 위해 temps에 copy해 전달
- line 7, 8 : ```select()``` ~~호출 후에는 타임아웃 발생 직전의 시간이 담기므로 초기화한다고 하는데 테스트 해본 결과 그렇지 않은 것 같은데... (해결 못함)~~
- line 9 : 지금은 수신된 데이터의 존재 유무만 확인할 것이므로 readset만 설정하고 writeset과 exceptset은 NULL로 설정
- line 15 : 지정된 시간만큼 아무런 변화가 없으면 0 리턴 (select 메서드는 변화가 발생한 파일 디스크럽트의 수를 반환)
- line 20 : 관심 대상 범위 내이면서 파일 디스크립터 0 에서 변화가 일어났다면 read해 출력
<br>

## multiplexing 기반 server & client

- server 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/multiplexing/multiplexing_server.c
- client 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/multiplexing/multiplexing_client.c