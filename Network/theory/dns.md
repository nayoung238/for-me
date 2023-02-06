# DNS (Domain Name System)

도메인 이름이란 ```www.google.com``` 같이 IP 주소를 대신하는 서버의 주소이다.

<br>

**IP 주소는 변경 가능하며 도메인 이름에 비해 상대적으로 변동이 심하다**. 
만약 웹페이지에 접속하기 위해 IP 주소를 입력해야 한다면, 서버의 IP 변경을 먼저 감지해야하는 번거로움이 생긴다.
그러므로 사용자는 **상대적으로 변동이 덜한 도메인 이름을 사용**하는데 **도메인 이름이 실제 접속에 사용되지는 않고 IP 주소로 변환해서 사용**한다.

<br>

## DNS server

DNS 서버는 도메인 이름을 IP 주소로 변환해주는 서버로 일종의 분산 데이터베이스 시스템이다.<br>

사용자가 ```www.google.com```을 작성하면 **default DNS server** 가 해당 도메인 이름과 일치하는 IP 주소를 반환해 주는데 반환받은 IP 주소를 이용해 서버에 접속하게 된다.
만약 **default DNS server**가 도메인 이름을 찾지 못하면 상위 서버에게 요청하고,상위 클래스에게 받은 IP 주소를 자신의 DNS 서버에 **update** 하므로 일종의 분산 데이터베이스 시스템이다. 
최상위 서버를 **Root DNS server** 라고 한다.

<br>

## hostent struct

```c
#include <netdb.h>

struct hostent* gethostbyname(const char* hostname);
```

```gethostbyname()``` 에 도메인 이름 정보를 string으로 전달하면 해당 도메인의 서버 정보가 ```hostent``` 구조체 변수에 채워 반환한다.
정상적으로 채워졌을 경우 ```hostent``` **구조체의 주소 값**이 리턴되고, 그렇지 않을 경우 NULL 포인터가 반환된다.<br>

도메인의 서버 정보가 담기는 ```hostent``` 구조체는 다음과 같다.<br>

```c
struct hostent {
    char  *h_name;      // official name
    char **h_aliases;   // alias list(domain)
    int    h_addrtype;  // host address type(IPv4, 6...)
    int    h_length;    // address lenght
    char **h_addr_list; // address list(IP)
};
```

- h_name : 1개의 서버에는 여러개의 IP와 도메인이 존재할 수 있는데 여러 도메인 중 공식 도메인 이름을 h_name 에 저장
- h_aliases: 여러 도메인 list로 string 타입
- h_addrtype : IPv4, IPv6 같은 주소 타입으로 IPv4 인 경우 AF_INET 가 반환
- h_length : 주소 타입의 길이 (IPv4는 4바이트이므로 4를, IPv6 16바이트이므로 16을 반환)
- h_addr_list : IP 주소 list

IPv4이면 4byte로 해석하고, IPv6은 16바이트로 해석하는 등 여러 형태의 주소 방식이 사용되기 때문에 string 으로 처리한다. 
해석하는 방법이 주소 체계에 따라 다르기 때문에 ```char``` 가 아닌 ```void``` 타입을 가져야 하는데 ```hostent``` 구조체가 만들어질 당시 void 타입이 표준화되지 않아 char를 적용했다.

<br>

## 도메인 이름으로 IP 주소 얻어오기

```c
#include <netdb.h>

struct hostent* gethostbyname(const char* hostname);
// 성공 시 hostent 구조체의 주소 값, 실패 시 NULL pointer return
```
도메인 이름으로 IP 주소를 얻어오려면 ```gethostbyname()``` 을 사용하면 된다.

```c
1  struct hostent *host;
2  host = gethostbyname(argv[1]);
3  if(!host) error_handling("gethost... error\n");
4    
5  printf("official name : %s\n", host -> h_name);
6
7  for(int i = 0; host -> host_aliases[i]; ++i) {
8      printf("Aliases %d : %s\n", i + 1, host -> h_aliases[i]);
9  }    
10 printf("Address type : %s\n", (host -> h_addrtype == AF_INET)?"AF_INET":"AF_INET6");
11
12 for(int i = 0; host -> h_addr_list[i]; ++i) {
13     printf("IP addr %d : %s\n", i + 1, inet_ntoa(*(struct in_addr*)host -> h_addr_list[i]));
14 }
```
- 전체 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/dns/gethostbyname.c

line 13 을 보면 string으로 저장된 IP 주소를 **in_addr 구조체로 casting**한 다음 **in_addr_t** 타입으로 저장된 값을 ```inet_ntoa()``` 를 통해 string 으로 변환해 출력한다. 
```hostent``` 구조체에 IP 주소를 저장하고 있는 ```h_addr_list``` 도 char** 타입으로 지정되어있기 때문에 결국 string 인데 왜 casting 하는 과정을 거치고 ```inet_ntoa()``` 함수를 호출해 다시 string 으로 바꾸는 번거로움을 거쳤을까?<br>

```h_addr_list``` 에는 IPv4, IPv6 등 여러 주소 체계가 모두 string으로 처리되어있기 때문에 이를 구분해야 한다.<br>
```in_addr``` 구조체는 32비트 unsigned int 인 ```in_addr_t``` 타입 하나만을 가지고 있음에도 구조체로 만들어졌다. 굳이 구조체로 왜 만들었을까 생각들지만 이는 **4바이트를 가져오면 1바이트씩 해석**하라고 정의하기 위함이다.<br>

line 13 에서 IP 주소를 출력하기 위해 ```in_addr``` 구조체로 casting 하고 ```inet_ntoa()```을 이용해 string으로 변환하는 번거로운 과정 또한 같은 이유이다.<br>
즉, 가져온 string을 **IPv4 기반으로 해석하겠다는 의미를 명시**하는 것이다.

<br>

```gethostbyname.c``` 를 ```hostname``` 으로 컴파일한 후 네이버와 구글에 대한 도메인 이름을 입력한 결과이다.<br>

![png](/Network/_img/gethostbyname.png)

네이버와 구글 둘다 IPv4 기반의 주소 체계를 사용하고 있으며 네이버는 IP 주소가 2개임을 알 수 있다.

<br>

## IP 주소로 도메인 정보 얻어오기

도메인 이름으로 IP 주소를 얻어오려면 ```gethostbyname()``` 을 사용했다면, 반대로 IP 주소를 이용해 도메인 정보를 얻어오려면 ```gethostbyaddr()``` 을 사용

```c
#include <netdb.h>

struct hostent* gethostbyaddr(const char* addr, socklen_t len, int family);
// 성공시 hostent 구조체의 주소 값, 실패 시 NULL pointer return
```

- addr : in_addr 구조체 변수의 포인터를 전달 (IPv4 이외의 다양한 정보를 일반화하기 위해 char* 타입)
- len : IPv4는 4바이트이므로 4 를, IPv6는 16바이트이므로 16 전달
- family : 주소쳬계 정보 전달 (IPv4는 **AF_INET**, IPv6는 **AF_INET6** 전달)

```c
1  struct hostent *host;
2  struct sockaddr_in addr;
3  memset(&addr, 0, sizeof(addr));
4  addr.sin_addr.s_addr = inet_addr(argv[1]);  
5  
6  host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
7  if(!host) error_handling("gethost... error\n");
8  printf("Official name : %s\n", host->h_name);
9      
10 for(i = 0; host->h_aliases[i]; i++)
11      printf("Aliases %d : %s\n", i + 1, host -> h_aliases[i]);
12 
13 printf("Address type : %s\n", (host->h_addrtype == AF_INET)?"AF_INET":"AF_INET6");
14 for(i = 0; host->h_addr_list[i]; i++)
15      printf("IP addr %d : %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
```
- 전체 코드: https://github.com/evelyn82ny/for-developer/blob/master/Network/code/dns/gethostbyaddr.c

<br>

![png](/Network/_img/gethostbyaddr.png) 

```gethostbyname.c``` 를 ```hostname``` 으로 컴파일하고 ```gethostbyaddr.c``` 를 ```hostaddr``` 으로 컴파일했다.

- ./hostname www.google.com : 도메인 이름을 입력하면 ```gethostbyname()```을 사용해 구글 서버 주소를 가져온다.
- ./hostaddr 142.250.196.100 : ```gethostbyname()```으로 얻은 IP 주소로 다시 한번 구글 서버 주소를 얻고자 ```gethostbyaddr()```를 호출했다.

그 결과 같은 주소 체계와 같은 IP 주소 목록이 출력되었고 네이버 도메인 이름으로 한번 더 해봤다.<br> 

- ./hostname www.naver.com : 도메인 이름을 입력하면 ```gethostbyname()```을 사용해 네이버 서버 주소를 가져온다.
- ./hostaddr 222.130.195.200 : ```gethostbyname()```으로 얻은 IP 주소로 다시 한번 네이버 서버 주소를 얻고자 ```gethostbyaddr()```를 호출했지만 도메인 정보를 얻지 못했다.

두 번째 IP 주소로 도메인 정보를 얻고자 시도했지만 실패했다.<br>
알아보니 해당 사이트가 IP정보로 서버 정보를 얻는 것을 막으면 해당 사이트에 대한 서버 정보를 가져올 수 없다고 한다.

