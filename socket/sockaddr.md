# IPV4 기반의 주소표현을 위한 구조체

통신하기 위해 소켓을 생성하고 IP 주소, port 번호까지 할당해야 데이터 송수신을 수행할 수 있는 상태가 된다.<br>
IPv4, IPv6, UNIX 프로토콜 등 통신하기 위해 사용되는 방식들이 다양한데 이를 모두 처리가능한 구조체가 sockaddr 이다.<br>

## IP

- 인터넷 주소라고도 하는 IP(Internet Protocol)는 인터넷 상에서 컴퓨터를 구분하는 목적으로 사용되는 주소이다.
- 우리가 현재 많이 쓰는 4바이트 주소체계인 IPv4 와 주소 고갈로 인해 생긴 16바이트 주소체계인 IPv6 이라는 프로토콜이 존재한다.
- 소켓을 생성하고 IPv4 프로토콜을 사용할 것인지, IPv6 프로토콜을 사용할 것인지 지정해야 한다.
- IP 주소는 네트워크 주소와 호스트 주소로 나뉘고, 네트워크 주소로 네트워크를 찾은 다음 호스트 주소로 호스트를 찾는다.
  - 현재는 완전한 class 체계 사용보다 classless 개념인 서브넷 마스크로 네트워크, 호스트 주소를 나눈다.
  - 자세한 내용 : [https://github.com/evelyn82/network/blob/master/theory/ip.md](https://github.com/evelyn82/network/blob/master/theory/ip.md)

책을 보면 현재는 Class 체계를 사용하지 않는다고 하는데 Classless 체계는 Class 체계를 기반으로 만들어졌기 때문에 아예 사용하지 않는다고 말하는게 조금은 이상하지 않나 생각든다.<br>

- IP주소를 보면 첫 번째 비트가 0으로 시작하면 /8 이라고 인식하면 된다.
- 즉, 앞 8비트를 네트워크 주소로 사용하겠다라는 의미인데
- 항상 책을 보면 결국은 '첫 번째 비트가 0이면 클래스 A이다.'라고 설명한다.
- 그렇기 때문에 이제는 더이상 Class 체계룰 사용하지 않는다라는 말이 어색하지 않나 싶지만 나의 개인적인 생각이다.
- 아직 전문가가 아니라서 제대로 모르는 거 일 수도 있고.. 더 공부해보면 언젠간 의도를 파악할 수 있지 않을까

하튼 간단하게 정리하면 네트워크 주소와 호스트 주소의 경계는 다음과 같다.

- 첫 번째 비트가 0으로 시작하면 앞 8비트가 네트워크 주소다. 
  - 클래스 A이고 뒤 24비트가 호스트 주소이다.
- 첫 번째 비트가 10으로 시작하면 앞 16비트가 네트워크 주소다. 
  - 클래스 B이고 뒤 16비트가 호스트 주소이다.
- 첫 번째 비트가 110으로 시작하면 앞 24비트가 네트워크 주소다.
  - 클래스 C이고 뒤 8비트가 호스트 주소이다.

즉, 첫 번째 바이트 정보로 IP 주소에서 네트워크 주소와 호스트 주소를 나눌 수 있다.<br>
이제 호스트 주소를 찾았다면 그 다음으로 할일은 Port 번호를 찾아야 한다. 즉, 컴퓨터를 찾았다면 컴퓨터 안에 여러 소켓 중 target 을 찾아야 한다.<br>

Port 번호는 소켓을 구분하는 용도로 16비트다. (0부터 65535 이하)<br>
0부터 1023까지는 Well-known port 로 이미 용도가 정해져 있어서 사용하면 안된다.<br><br>

## sockaddr_in 구조체

sockaddr_in 구조체를 설명하겠다고 얘기가 길어졌는데 결국 소켓은 네트워크 주소와 호스트 주소를 찾기위한 IP 주소와 호스트 안에서 여러 소켓 중 target을 찾기 위한 Port 번호가 필요하다.<br>
IP 주소, Port 번호는 데이터 송수신을 위해 상대방의 것도 필요하지만, 상대방이 나를 찾아야 하므로 나의 주소도 설정해야 한다.<br>

```c
struct sockaddr_in {
    sa_family_t     sin_family;    // 주소체계
    uint16_t        sin_port;      // Port 번호
    struct in_addr  sin_addr;      // 32비트 IP주소
    char            sin_zero[8];   // 모두 0으로 초기화
}
```

sockaddr_in 구조체는 IPv4 프로토콜만 사용하는 주소체계를 표현하기 위한 구조체이다.<br>

### sin_family 주소체계

IPv4, IPv6 등 어떤 프로토콜을 사용하는지에 대한 주소체계를 작성해야 하는데 sockaddr_in 구조체는 IPv4 전용이므로 ```AF_INET``` 을 작성한다.<br>

그럼 'sockaddr_in 구조체는 어짜피 IPv4 프로토콜만 사용하는데 작성할 필요가 있나?' 라는 생각이 들 수도 있지만 작성해야한다.<br>

- sockaddr 구조체 부분에서 자세히 설명하겠지만 많은 소켓은 IPv4, IPv6, UNIX 등 같이 여러 주소체계 중 하나를 사용한다.
- 소켓에 주소를 할당할 때는 여러 주소체계를 한번에 처리할 수 있는 sockaddr 구조체를 사용하는데 이때 어떤 주소체계를 사용하는지 구분하기 위해서 sin_famaily 를 명시해야 한다.

더 자세한 내용은 sockaddr 구조체에서 하고 여러 프로토콜에 대한 주소체계는 다음과 같다.<br>

- AF_INET : IPv4 인터넷 프로토콜에 적용하는 주소체계
- AF_INET6 : IPv6 인터넷 프로토콜에 적용하는 주소체계
- AF_LOCAL : 로컬 통신을 위한 유닉스 프로토콜 주소체계

### sin_port : 포트 번호

16비트로 port 번호를 저장하고 네트워크 바이트 순서인 **Big Endian**을 기준으로 저장한다.<br>

### sin_addr :  IP 주소

32비트 IP 주소를 저장하고 네트워크 바이트 순서인 **Big Endian**을 기준으로 저장한다.<br>

```c
struct in_addr{
    in_addr_t    s_addr;  // 32비트 IPv4 인터넷 주소
};  
```

- sin_addr의 타입은 in_addr 구조체 타입이다.
- in_addr에서 Ipv4 인터넷 주소를 저장하기 위한 s_addr은 **in_addr_t** 타입이다.
- in_addr_t 은 unsigned_int 32비트로 정의되어 있다.

굳이 **in_addr_t** 로 작성하는 이유는 직관적으로 보이게 하고, 시스템 확장성을 위해서 이다.<br>

- int8_t : signed 8bit int
- uint8_t : unsigned 8bit int(unsigned char)
- int16_t : signed 16bit int
- uint16_t : unsigned 16bit int(unsigned short)
- int32_t : signed 32bit int
- uint32_t : unsigned 32bit int(unsigned long)
- sa_family_t : 주소 체계(address family)
- socklen_t : 길이 정보(length of struct)
- in_addr_t : IP 주소, uint32_t로 정의
- in_port_t : Port 번호, uint16_t로 정의

### sin_zero[8]

8바이트로 무조건 0으로 채워야 하는데 sockaddr 구조체에서 sa_data 부분을 채우기 위해서이다.<br>
자세한 설명은 sockaddr 에서!<br><br>

## sockaddr 구조체

```c
struct sockaddr {
    sa_family_t   sin_family;   // 주소 체계
    char          sa_data[14];  // 주소 정보
};
```

### sin_family

sockaddr 구조체는 IPv4, IPv6, local 프로토콜을 모두 처리하기 때문에 sin_family로 어떤 프로토콜인지 구분한다.<br>
앞서 sockaddr_in 구조체는 IPv4 전용인데 ```AF_INET```로 명시하는 이유가 sockaddr로 변환할 때 어떤 프로토콜인지 구분하기 위해서 이다.<br>

### sa_data[14]

어떤 프로토콜인지 구분했다면 IP 주소와 Port 번호를 작성해야 하는데 sockaddr 구조체는 14바이트에 한번에 담는다.<br>
IPv4의 주소 정보를 담기 불편해서 동일한 바이트 열을 구성하는 sockaddr_in 구조체가 생긴 것이다.<br>

![png](/_img/sockaddr_sa_data14.png) <br>

14 바이트로 구성되어 있으며 앞 2바이트는 Port 번호를 담고, 그 다음 4바이트에는 IP 주소를 담는다.<br>
IPv4를 표현하기 위한 sockaddr_in 구조체의 sin_zero[8]를 모두 0으로 채운 이유는 sa_data의 나머지 8바이트를 0으로 채우기 위해서 였다.<br><br>

## 소켓에 주소 할당하기

```c
struct sockaddr_in serv_addr;

if(bind(serv_addr, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) 
    error_handing("bind() error");
```

소켓을 생성했다면 주소까지 할당하기 위해 bind()를 호출한다.<br>
위 코드를 보면 여러 기반의 주소체계를 처리하기 위해 sockaddr 구조체로 casting 한 다음 소켓에 sockaddr 구조체의 시작 주소를 연결해준다.<br>