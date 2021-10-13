# 인터넷 주소 할당하기

인터넷 주소를 할당하기 위해선 빅 엔디안, 리틀 엔디안이라는 개념을 알아야 한다.<br>
CPU의 데이터 저장방식이 빅 엔디안, 리틀 엔디안 에 따라 호스트는 데이터를 표현하고 해석하는 방식이 모두 다르다.<br>
하지만 네트워크 바이트 순서는 빅 엔디안을 기준으로 한다.<br>

## Big Endian & Little Endian

![png](/_img/big_endian.png) <br>

- 빅 엔디안 : 상위 바이트의 값을 작은 번지수에 저장
- 리틀 엔디안 : 상위 바이트의 값을 큰 번지수에 저장

```0x12345678```을 전송하면 빅 엔디안은 ```0x12345678```로, 리틀 엔디안은 ```0x78563412```로 저장한다.<br>

### 16진수 표기

리틀 엔디안의 저장 결과가 ```0x87654321``` 이 아니라 ```0x78563412``` 인 이유는 16진수로 표기했기 때문이다.<br>

- 2진수 4개를 16진수 1개로 표현가능하다. 즉, 16진수 1자리당 4bit
- 빅 엔디안, 리틀 엔디안은 1byte 단위로 데이터를 처리한다.
- 즉, 16진수 표기에서 1byte는 4bit가 2개인 2글자씩 처리됨으로 이해하면 된다. 


## 바이트 변환 라이브러리

### host 바이트 순서를 네트워크 바이트 순서로 바꾸기

- unsigned short htons(unsigned short); 
- unsigned long htonl(unsigned long);
 

### network 바이트 순서를  host cpu 기준으로 바꾸기

- unsigned short ntohs(unsigned short);
- unsigned long ntohl(unsigned long);


```c
unsigned short host_port = 0x1234;
unsigned short net_port;

net_port = htons(host_port);

printf("Host ordered port : %#x\n", host_port);
printf("Network ordered port : %#x\n", net_port);
```

위 코드를 실행하면 다음과 같은 결과를 출력할 것이며 host pc가 리틀 엔디안임을 알 수 있다.<br>

```c
Host ordered port : 0x1234
Network ordered port : 0x3412
```
<br><br>

## inet_addr()

IP 주소를 ```211.214.107.99``` 의 형태인 10진수로 표현한 문자열로 넘겨주면 32비트 정수형으로 반환해 sockaddr_in 구조체의 sin_addr을 채울 수 있다.<br>

```c
#include <arpa/inet.h>

in_addr_t inet_addr(const char* string);
```

string으로 넘져준 IP 주소를 제대로 변환했다면 32비트 정수 값을 리턴하고, 변환에 실패했다면 ```INADDR_NONE``` 을 반환한다.<br>

- IPv4 기반 주소체계를 표현하는 sockaddr_in 구조체에서 IP 주소를 명시하는 sin_addr를 채워야 한다.
- sin_addr 의 타입은 in_addr 구조체 타입이다.
- in_addr 구조체의 s_addr 에 32비트 IPv4 인터넷 주소를 저장하는데 s_addr 의 타입은 in_addr_t 이다.
- in_addr_t 는 unsigned_int 32비트로 정의되어있다.

> 해당 내용 : [https://github.com/evelyn82/network/blob/master/socket/sockaddr.md](https://github.com/evelyn82/network/blob/master/socket/sockaddr.md) <br>

string으로 IP 주소를 넘겨주면 inet_addr() 은 in_addr_t 타입인 unsigned_int 32비트로 반환하며, 이를 in_addr 구조체의 s_addr 에 저장한다.<br>

```c
char *addr = "1.2.3.4";
unsigned long conv_addr = inet_addr(addr);

if(conv_addr == INADDR_NONE)
    printf("Error occured!\n");
else 
    printf("Network ordered integer addr : %#lx\n", conv_addr);
```

위 코드의 결과는 다음과 같다.<br>
```c
Network ordered integer addr : 0x4030201  // 04에서 0 생략 가능
```

네트워크 바이트 순서는 빅 엔디안이므로 0x4030201 결과가 출력된다.<br>

```c
char *addr = "1.2.3.256";
unsigned long conv_addr = inet_addr(addr);

if(conv_addr == INADDR_NONE)
    printf("Error occured!\n");
else 
    printf("Network ordered integer addr : %#lx\n", conv_addr);
```

```1.2.3.256```를 변환하도록 전달했다면 **Error occured!** 으로 출력된다.<br>
IP는 32비트이므로 . 을 기준으로 1바이트로 변환된다. 즉, 1바이트가 표현할 수 있는 **0부터 255까지** 범위 내에서 작성해야 한다.<br>

inet_addr() 은 in_addr_t 타입인 32비트 정수를 반환한다.<br>
즉, in_addr 구조체를 생성해 반환된 32비트 정수를 저장해야 하는 과정을 거쳐야 한다.<br><br>

## inet_aton()

inet_addr() 은 in_addr 구조체를 만드는 과정을 거쳐야 했다면, inet_aton() 을 사용하면 in_addr 구조체를 만드는 과정을 생략할 수 있다.<br>
aton에서 a는 ASCII를 의미한다.<br>

```c
#include <arpa/inet.h>

int inet_aton(const char* string, struct in_addr* addr);
```

- string : 변환할 IP 주소를 담고 있는 문자열의 주소 값
- addr : 변환된 주소를 저장할 in_addr 구조체 변수의 주소 값

in_addr 구조체 변수의 주소 값을 전달하기 때문에 변환한 32비트 정수 IP 주소를 바로 저장하면 된다.<br>
저장에 성공하면 1을 리턴하고, 실패하면 0을 리턴한다.<br>

```c
char *addr = "127.232.124.79";
struct sockaddr_in addr_inet;

if(!inet_aton(addr, &addr_inet.sin_addr)) 
    error_handling("Conversion error");
else
    printf("Network ordered integer addr : %#x\n", 
                       addr_inet.sin_addr.s_addr);
```

IPv4 기반의 주소체계 구조체인 sockaddr_in을 만들고 inet_aton()을 호출할 때 sockaddr_in의 IP 주소를 저장하는 sin_addr의 주소를 전달한다.<br>
위 코드의 결과는 **Network ordered integer addr : 0x4f7ce87f** 이다.<br><br>

## inet_ntoa()

```c
#include <arpa/inet.h>

char* inet_ntoa(struct in_addr adr);
```

inet_aton() 와 반대 기능으로 네트워크 바이트 순서로 정렬된 IP 주소를 우리가 보기 편한 문자열의 형태로 변환한다.<br>

```c
struct sockaddr_int addr;
addr.sin_addr.s_addr = htonl(0x01020304);

char* str_ptr = inet_ntoa(addr.sin_addr);
printf("Dotted-Decimal notation : %s", str_ptr);
```

위 코드의 결과는 **Dotted-Decimal notation : 1.2.3.4** 이다.<br><br>

## 인터넷 주소 초기화하기

```c
struct sockaddr_in addr;
char* serv_ip = "211.217.168.13";   // IP 주소 (현재 string)
char* serv_port = "9190";           // Port 번호 (현재 string)
memset(&addr, 0, sizeof(addr));     

addr.sin_family = AF_INET;   // IPv4
addr.sin_addr.s_addr = inet_addr(serv_ip); // string인 IP 주소 변환해 초기화
addr.sin_port = htons(atoi(serv_port));    // string인 Port 번호 변환해 초기화
```

### 서버가 주소를 설정하는 의미

```IP 211.217.168.13```, ```Port 9190``` 으로 들어오는 연결은 나와 연결 시켜라 라는 의미로 서버 자신의 주소를 명시한다.<br>

### 클라이언트가 주소를 설정하는 의미

```IP 211.217.168.13```, ```Port 9190``` 으로 연결해라 라는 의미로 목적지의 주소를 저장한다.<br>
클라이언트는 자신의 IP 주소, port 번호를 굳이 알 필요가 없어서 커널에서 알아서 할당한다.<br>

sockaddr_in 구조체로 IPv4 기반의 주소체계를 만들었다면 bind() 로 소켓에 주소를 할당하면 된다.<br>

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

```c
struct sockaddr_in addr;
char* serv_port = "9190"; 
memset(&addr, 0, sizeof(addr));     

addr.sin_family = AF_INET; 
addr.sin_addr.s_addr = htonl(INADDR_ANY);
addr.sin_port = htons(atoi(serv_port));
```

**INADDR_ANY** 는 IP 주소를 4바이트 unsigned int로 변환한 상수를 의미한다.<br>
현재 실행중인 컴퓨터의 IP를 소켓에 할당할 때 **INADDR_ANY** 을 사용하는데 주로 서버 프로그램 구현에 사용된다.<br>

