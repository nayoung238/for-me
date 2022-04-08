# Nagle algorithm

- 세그먼트를 주고 받으면서 통신하는데 세그먼트가 많아지면 과도한 트래픽으로 인해 전송속도가 느려진다.
- 세그먼트에는 전송할 데이터와 각 계층의 헤더가 붙는다. 헤더의 용량도 크기 때문에 적은 데이터를 계속 보낸다면 비효율적인 통신이 된다.
- 하지만 적은 데이터를 보내는 방식이 필요할 때도 있기 때문에 상황에 맞게 Nagle-algorithm 을 on/off 해야 한다.

## Nagle ON

TCP 통신에선 NAGLE-ON 이 기본설정이며, ```NAGLE``` 문자열을 보내는 과정은 다음과 같다.<br>

![png](/network/_img/nagle_algorithm.png)

Nagle algorithm ON 설정은 보낸 데이터에 대한 ACK 을 받으면 그다음 데이터를 전송하는 방식이다.<br>

- N을 키보드에 입력하는 순간 데이터 N과 각 계층의 헤더를 추가해 전송한다.
- 해당 세그먼트에 대한 ACK 을 받기 전까지 보낼 데이터를 쌓아둔다.
- ACK 을 받았다면 쌓아둔 데이터와 각 계층의 헤더를 추가해 전송한다.
  - 앞으로 보내야할 데이터가 max packet size 에 도달하면 전송한다.

앞서 전송한 데이터에 대한 ACK 을 받으면 다음 데이터를 전송하는 방식이다. 또한, ACK 을 받기 전까지 데이터를 쌓아두고 쌓인 데이터를 한번에 보내기 때문에 소켓의 크기가 커진다.

<br>

## Nagle OFF

![png](/network/_img/nagle_on_off.png)

Nagle 설정을 OFF 하면 앞서 전송한 데이터에 대한 ACK을 기다리지 않고 버퍼에 쌓이는 즉시 데이터를 전송한다. 그렇기 때문에 빠른 속도로 데이터를 주고 받을 수 있다. 하지만 통신과정에선 데이터만 주고 받는 것이 아니고 각 계층의 헤더파일까지 포함해 주고 받기 때문에 비효율적인 방식이다. 또한 이런 방식이 많이 사용되면 과도한 트래픽으로 전체적인 전송속도가 느려지게 된다.<br>

빠른 통신도 좋지만 네트워크 통신에서 **같은 용량을 적은 세그먼트로 보내는 것을 선호**한다. 즉, Nagle algorithm을 ON 설정한 방법이 많이 사용된다.

<br>

## Nagle algorithm 설정하기

소켓에 대한 옵션 정보를 확인하려면 **getsockopt** 함수를 사용한다.

```c
#include <sys/socket.h>
int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);

// 성공 시 return 0 , 실패 시 return -1
```

- sock : 소켓의 파일 디스크립터
- level : 확인할 옵션의 protocol level
- optname : 확인할 옵션의 이름
- optval : 확인 결과를 저장하기 위한 버퍼 주소값. 여러 타입이 저장될 수 있기 때문에 void 타입
- optlen : optval 매개변수로 전달된 버퍼 주소 값의 버퍼 크기를 담고 있는 변수의 주소값 전달

<br>

소켓에 대한 옵션을 설정하려면 **setsockopt** 함수를 사용한다.
```c
#include <sys/socket.h>
int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);

// 성공 시 return 0 , 실패 시 return -1
```
- sock : 소켓의 파일 디스크립터
- level : 변경할 옵션의 protocol level
- optname : 변경할 옵션의 이름
- optval : 변경할 옵션정보를 저장한 버퍼의 주소. 여러 옵션을 처리하기 위해 void 타입
- optlen : optval 매개변수는 void 타입이므로 optval에서 읽어야할 크기를 전달

<br>

nagle algorithm ON 설정시 다음과 같이 작성한다.

```c
#include <arpa/inet.h>   // IPPROTO_TCP
#include <netinet/tcp.h> // TCP_NODELAY

int opt = 양수 (0 작성 시 nagle OFF)
int result = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (void*)&opt, sizeof(opt));

printf("Nagle status: %d\n", opt_val);
```
- nagle 설정 전체 코드: https://github.com/evelyn82/network/blob/master/code/sock-option/nagle_check.c

![png](/network/_img/nagle_result.png)

기본 설정은 nagle-algorithm ON 이므로 0이 출력되었고, OFF 로 설정하니 4가 출력되었다.