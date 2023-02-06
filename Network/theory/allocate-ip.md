# 인터넷 주소 할당하기

인터넷 주소를 할당하기 위해선 **빅 엔디안**과 **리틀 엔디안**이라는 개념을 알아야 한다.

- CPU의 데이터 저장방식이 **빅 엔디안**과 **리틀 엔디안**에 따라 호스트는 데이터를 표현하고 해석하는 방식이 모두 다름
- Network 바이트 순서는 **빅 엔디안이 기준**

<br>

## Big Endian & Little Endian

![png](/Network/_img/big_endian.png) <br>

- 빅 엔디안 : 상위 바이트의 값을 **작은 번지수에 저장**
- 리틀 엔디안 : 상위 바이트의 값을 **큰 번지수에 저장**

```0x12345678```을 전송하면 빅 엔디안은 ```0x12345678```로, 리틀 엔디안은 ```0x78563412```로 저장한다.

<br>

### 16진수 표기

리틀 엔디안의 저장 결과가 ```0x87654321``` 이 아니라 ```0x78563412``` 인 이유는 **16진수로 표기했기 때문**이다.

- 2진수 4개를 16진수 1개로 표현가능 (즉, 16진수 1자리당 4bit)
- 빅 엔디안, 리틀 엔디안은 1byte 단위로 데이터를 처리
- 즉, 16진수 표기에서 1byte는 4bit가 2개인 **2글자씩 처리**

<br>

## 바이트 변환 라이브러리

- header : arpa/inet.h

### host 바이트 순서를 네트워크 바이트 순서로 바꾸기

- unsigned short **htons**(unsigned short); 
- unsigned long **htonl**(unsigned long);

### network 바이트 순서를 host cpu 기준으로 바꾸기

- unsigned short **ntohs**(unsigned short);
- unsigned long **ntohl**(unsigned long);
<br>

- host cpu 기준으로 변환하는 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/convert-address/htons.c

```c
unsigned short host_port = 0x1234;
unsigned short net_port;

net_port = htons(host_port);

printf("Host ordered port : %#x\n", host_port);
printf("Network ordered port : %#x\n", net_port);
```

위 코드를 실행하면 다음과 같은 결과를 출력하며 host pc가 리틀 엔디안임을 알 수 있다.<br>

```c
Host ordered port : 0x1234
Network ordered port : 0x3412
```
<br><br>

## inet_addr()

IP 주소를 ```211.214.107.99``` 의 형태인 10진수로 표현한 문자열로 넘겨주면 32비트 정수형으로 반환해 ```sockaddr_in``` 구조체의 ```sin_addr```을 채울 수 있다.

```c
#include <arpa/inet.h>

in_addr_t inet_addr(const char* string);
// 성공 시 big-endian으로 변환한 32비트 정수 값, 실패 시 INADDR_NONE return 
```

string 으로 넘져준 IP 주소를 제대로 변환했다면 **32비트 정수** 값을 리턴하고, 변환에 실패했다면 ```INADDR_NONE``` 을 반환한다.<br>
정상적인 32비트를 받았다면 ```in_addr``` 구조체의 ```s_addr``` 에 저장한다.

```c
#include <netinet/in.h>

struct sockaddr_in {
    sa_family_t     sin_family;    // 주소체계
    uint16_t        sin_port;      // Port 번호, 2byte
    struct in_addr  sin_addr;      // 32비트 IP주소, 4byte
    char            sin_zero[8];   // 모두 0으로 초기화, 8byte
};

struct in_addr {
    in_addr_t    s_addr;  // 32비트 IPv4 인터넷 주소
};
```

- IPv4 기반 주소체계를 표현하는 ```sockaddr_in``` 구조체에서 IP 주소를 명시하는 ```sin_addr```를 채워야 한다.
- ```sin_addr``` 의 타입은 ```in_addr``` 구조체 타입이다.
- ```in_addr``` 구조체의 ```s_addr``` 에 32비트 IPv4 인터넷 주소를 저장하는데 ```s_addr``` 의 타입은 ```in_addr_t``` 이다.
- ```in_addr_t``` 는 unsigned_int 32비트이다.

> sockaddr_in 에 대한 내용 : https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/sockaddr.md

<br>

주소를 입력하고 네트워크 주소로 제대로 변환되는지 확인해봤다.

- 네트워크 주소로 변환하는 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/addr-conv/inet_addr.c

```c
char *addr = "1.2.3.4";
unsigned long conv_addr = inet_addr(addr);
```
위 코드의 결과는 다음과 같다.
```c
Network ordered integer addr : 0x4030201  // 04에서 0 생략 가능
```
네트워크 바이트 순서는 **빅 엔디안**이므로 0x4030201 결과가 출력된다.

<br>

```c
char *addr = "1.2.3.256";
unsigned long conv_addr = inet_addr(addr);  -> INADDR_NONE return
```

```1.2.3.256```를 변환하도록 전달했다면 **네트워크 주소로 변환할 수 없다.**<br>
IP는 32비트이므로 ```.``` 을 기준으로 1바이트로 변환된다. 즉, 1바이트가 표현할 수 있는 **0부터 255까지** 범위 내에서 작성해야 한다.<br>

```inet_addr()``` 은 ```in_addr_t``` 타입인 **32비트 정수**를 반환만 해준다.<br>
즉, ```in_addr``` **구조체를 생성해 반환된 32비트 정수를 저장해야 하는 과정을 거쳐야 한다**.

<br>

## inet_aton()

```inet_addr()``` 은 ```in_addr``` 구조체를 만드는 과정을 거쳐야 했다면, ```inet_aton()``` 을 사용하면 ```in_addr``` 구조체를 인자로 넘겨주는 차이가 있다.<br>
**aton**에서 **a**는 **ASCII**를 의미한다.

```c
#include <arpa/inet.h>

int inet_aton(const char *string, struct in_addr *addr);
// 성공 시 1, 실패 시 0 return
```

- string : 변환할 IP 주소를 담고 있는 문자열의 주소 값
- addr : 변환된 주소를 저장할 ```in_addr``` 구조체 변수의 주소 값

인자에 ```in_addr``` 구조체 변수의 주소 값을 전달하기 때문에 **변환한 32비트 정수 IP 주소를 바로 저장**된다.

<br>

- ```inet_aton()```으로 변환하는 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/addr-conv/inet_aton.c

```c
char *addr = "127.232.124.79";
struct sockaddr_in addr_inet;

if(!inet_aton(addr, &addr_inet.sin_addr)) 
    error_handling("Conversion error");
else
    printf("Network ordered integer addr : %#x\n", addr_inet.sin_addr.s_addr);
```

IPv4 기반의 주소체계 구조체인 ```sockaddr_in```을 만들고 ```inet_aton()``` 호출 시 ```sockaddr_in```의 IP 주소를 저장하는 ```sin_addr```에 변환한 주소를 저장한다.<br>
위 코드의 결과는 **Network ordered integer addr : 0x4f7ce87f** 이다.

<br>

## inet_ntoa()

```c
#include <arpa/inet.h>

char* inet_ntoa(struct in_addr adr);
// 성공 시 변환된 문자열의 주소값, 실패 시 -1 반환
```

```inet_aton()```의 반대 기능으로 네트워크 바이트 순서로 정렬된 IP 주소를 우리가 보기 편한 **문자열의 형태로 변환**한다.

<br>

- ```inet_ntoa()``` 에 대한 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/addr-conv/inet_ntoa.c

```c
struct sockaddr_int addr;
addr.sin_addr.s_addr = htonl(0x01020304);

char* str_ptr = inet_ntoa(addr.sin_addr);
printf("Dotted-Decimal notation : %s", str_ptr);
```

위 코드의 결과는 **Dotted-Decimal notation : 1.2.3.4** 이다.

<br>

## 인터넷 주소 초기화하기

```c
struct sockaddr_in addr;
char *serv_ip = "211.217.168.13";   // IP 주소 (현재 string)
char *serv_port = "9190";           // Port 번호 (현재 string)
memset(&addr, 0, sizeof(addr));     

addr.sin_family = AF_INET;   // IPv4
addr.sin_addr.s_addr = inet_addr(serv_ip); // string인 IP 주소 변환해 초기화
addr.sin_port = htons(atoi(serv_port));    // string인 Port 번호 변환해 초기화, atoi: 문자를 정수로 변환
```

### 서버가 주소를 설정하는 의미

```IP 211.217.168.13```, ```Port 9190``` 으로 들어오는 연결은 나와 연결 시켜라 라는 의미로 서버 자신의 주소를 명시한다.

<br>

### 클라이언트가 주소를 설정하는 의미

```IP 211.217.168.13```, ```Port 9190``` 으로 연결해라 라는 의미로 목적지의 주소를 저장한다.<br>
클라이언트는 자신의 IP 주소와 port 번호를 굳이 알 필요가 없어서 ```connect()``` 호출시 서버와 연결 전에 커널에서 알아서 할당한다.<br>

<br>

서버 측은 ```sockaddr_in``` 구조체로 IPv4 기반의 주소체계를 만들었다면 ```bind()``` 로 소켓에 주소를 할당하면 된다.<br>

```c
int serv_sock;
struct sockaddr_in serv_addr;
char *serv_port = "9190";

// IPv4 기반의 TCP 프로토콜을 사용하겠다.
serv_sock = sock(PF_INET, SOCK_STREAM, 0);

memset(&addr, 0, sizeof(addr));     
addr.sin_family = AF_INET; 
addr.sin_addr.s_addr = htonl(INADDR_ANY);
addr.sin_port = htons(atoi(serv_port));

bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
```

### INADDR_ANY

- header : netinet/in.h

```c
struct sockaddr_in addr;
char* serv_port = "9190"; 
memset(&addr, 0, sizeof(addr));     

addr.sin_family = AF_INET; 
addr.sin_addr.s_addr = htonl(INADDR_ANY);
addr.sin_port = htons(atoi(serv_port));
```

**INADDR_ANY** 는 IP 주소를 4바이트 **unsigned int**로 변환한 상수를 의미한다.<br>
현재 실행중인 컴퓨터의 IP를 소켓에 할당할 때 **INADDR_ANY** 을 사용하는데 주로 서버 프로그램 구현에 사용된다.

<br>

## 소켓에서 주소 추출하기

1개의 소켓에는 나와 상대방의 주소가 존재한다.

### 나의 주소 추출

```c
#include <sys/socket.h>

int getsockname(int s, struct sockaddr *addr, socklen_t *addrlen);
// 성공 시 0, 실패 시 -1 return
```

- s : sock descriptor
- addr : 주소를 받아서 저장할 구조체 주소
- addrlen : sockaddr 구조체의 크기
  - return 시 addrlen 에 실제 크기가 저장됨 (call by reference)
  - 이만큼을 읽어야 함을 의미

### 상대방 주소 추출

```c
#include <sys/socket.h>

int getpeername(int s, struct sockaddr *name, socklen_t *namelen);
// 성공 시 0, 실패 시 -1 return
```

