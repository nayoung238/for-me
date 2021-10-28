# 4-way handshake

TCP 통신에서 데이터 송수신 전에 서로 데이터를 주고 받을 수 있는 상태인지 체크하는 것을 **3-way handshake**라고 한다.<br>
**4-way handshake**는 서로간의 연결을 종료하기 위한 방식이다.<br>

## TCP 소켓의 입출력 버퍼

TCP 소켓마다 입출력 버퍼가 존재하는데 이는 소켓 생성시 자동으로 생성된다.<br>

![png](/_img/tcp_buffer.png) <br>

read(입력) buffer에 있는 데이터를 read()로 읽고, write()하면 write(출력) buffer에 쌓인다.<br>
write buffer에 쌓인 데이터는 상대방의 read buffer에 쌓이는 방식이다.<br>

하지만 입출력 버퍼에 데이터가 있는 상태로 close()를 호출하면 어떻게 될까?
- 입력 버퍼에 데이터가 있는 상태에서 close()로 소켓을 닫으면 입력버퍼에 있는 데이터는 소멸된다.
- 출력 버퍼에 데이터가 있는 상태에서 close()로 소켓을 닫아도 일단 데이터를 모두 전송한 다음 버퍼를 닫는다.

즉, close()를 호출해도 write buffer에 있는 데이터(전송해야하는 데이터)가 상대방에게 전달하는 것을 보장한다.<br>
하지만 상대방에게 전달하는 것을 보장하는 것이지 상대방이 받았다는 것을 보장하는 게 아니다.<br>

만약 상대가 나보다 먼저 소켓을 닫은 경우 이미 입력 버퍼가 닫혀있기 때문에 내가 데이터를 보냈다해도 받을 방법이 없다. 정말 전송의 의무만 다하는 것이다.<br>
물론 이 경우는 효율적인 방법이 아니다.<br><br>

## 4-way handshake

![png](/_img/4way_handshake.png) <br>

- 먼저 연결을 끊고자 하는 쪽을 **Active close**라 하고, 상대를 **Passive close**라고 한다.
- FIN_WAIT_1 (host A) : close()를 호출하면 '연결을 끊겠다'라는 신호를 보내기 위해 FIN flag를 1로 설정한 패킷을 상대에게 보내면서 ```FIN_WAIT_1``` 상태가 된다.
- CLOSE_WAIT (host B) : 상대에게 FIN flag가 1인 패킷을 받았다면 **'연결을 종료하겠다는 것을 일단 알고있겠다.'** 라는 의미로 해당 패킷에 대한 응답 ACK을 보내고 ```CLOSE_WAIT``` 상태가 된다. 출력 버퍼에 있는 데이터를 모두 보낸 다음 서로간의 연결을 끊어야 하므로 ACK 만 보낸다.
- FIN_WAIT_2 (host A) : 상대방이 FIN flag가 1인 패킷을 보낼 때까지 기다리는데 이를 ```FIN_WAIT_2``` 상태라고 한다.
- LAST_ACK (host B) : 상대에게 모든 데이터를 보냈다면 close()를 호출해 FIN flag가 1인 패킷을 보낸다.
- TIME_WAIT (host A) : 상대에게 FIN flag가 1인 패킷을 받았다면 서로가 연결을 끊을 준비가 된 것이므로 이에 대한 응답 ACK을 보낸다.
- CLOSED : ACK을 받으면 소켓을 닫아 연결을 종료한다.

이렇게 4번의 패킷 전송으로 인해 연결이 종료되므로 이를 ```4-way handshake```라고 한다.<br><br>

## TIME_WAIT

![png](/_img/time_wait.png) <br>
연결을 먼저 끊겠다고 한 Active close의 소켓은 TIME_WAIT 상태를 거친다. TIME_WAIT 상태를 거치는 이유는 **TIME_WAIT 상태 동안에는 해당 소켓의 주소를 다른 소켓에게 할당하는 것을 막기 위함**이다.<br>
서로가 FIN flag가 1인 소켓을 주고 받았다면 Active close는 마지막 ACK을 상대에게 보낸다. 하지만 상대가 ACK을 받지 못하면 LAST_ACK 상태에서 FIN flag가 1인 소켓을 다시 보낸다.<br>
FIN flag가 1인 소켓을 다시 받은 Active close는 '상대가 ACK을 받지 못했구나.' 라고 알아 차린 뒤 다시 ACK을 보낸다. 일정 기간 동안 상대방에게 소켓이 오지 않으면 '상대가 소켓을 닫았구나.' 라고 인식해 자신도 소켓을 닫는다.<br>

TIME_WAIT 시간은 커널에 default 값으로 정해져 있지만 수정가능하다. passive close 에게 ACK을 보내면 시간이 측정되는데 상대가 ACK을 받지 못해 다시 ACK을 보내는 경우 시간을 다시 측정한다.<br>

![png](/_img/time_wait2.png) <br>
TIME_WAIT 상태를 거치는 이유는 **TIME_WAIT 상태 동안에는 해당 소켓의 주소를 다른 소켓에게 할당하는 것을 막기 위함**이다. 마지막 ACk이 제대로 전달되지 않을 경우 상대는 FIN flag가 1인 소켓을 다시 보내는데 이 과정에서 기존 포트번호를 다른 소켓에게 할당했다면 제대로된 전송이 이루어지지 않기 때문이다.<br>
TIME_WAIT 상태는 클라이언트에겐 중요하지 않고 서버에게 중요한 개념이다. 클라이언트 소켓을 생성할 땐 우리가 주소를 직접 지정해주지 않고 connect() 호출시 커널에 자동으로 해준다. 즉, 우리가 클라이언트 소켓의 포트번호를 알 필요도 없고 close() 직후 새로운 클라이언트 소켓을 생성해도 커널이 알아서 적절한 포트번호를 지정해준다.<br>
하지만 서버 소켓을 생성할 떄는 어떤 주소를 쓰고 있는지 명시하기 위해 bind()를 호출한다. 즉, 우리가 원하는 포트번호를 지정할 수 있기 때문에 완전히 연결이 끝나지 않는 포트 번호를 사용하면 안된다. 그러므로 TIME_WAIT 상태는 서버에게 중요한 개념이며, TIME_WAIT 동안 기존 소켓 주소를 다른 소켓에게 할당하면 안된다.<br><br>

## Half-close

4-way handshake 과정을 보면 host B가 소켓을 닫겠다라는 패킷을 받아도 일단 host A에게 write buffer에 있는 데이터를 보낸다.<br>
하지만 host A가 이미 read buffer를 닫았다면 해당 패킷들은 버려지게 되고 이는 좋은 방식이 아니다.<br>
이렇게 일방적인 종료를 해결하고자 half close 개념을 적용한다.<br>
close()를 호출하면 입출력 버퍼가 둘다 닫지만, shutdown()을 호출하면 입출력 버퍼 중 하나만 닫을 수 있다.<br>

host A가 먼저 소켓을 닫겠다는 의지를 보인다면 아래와 같은 과정이 일어난다.<br>

![png](/_img/half-close.png) <br>

1. host A는 일단 출력 버퍼만 닫기 위해 shutdown()를 호출하면 출력버퍼에 있는 데이터와 EOF를 상대방에게 보낸 다음 출력 버퍼를 닫는다. (half-close)
2. host B는 입력 버퍼에서 데이터를 읽고 출력 버퍼에 데이터를 쌓은 뒤 close()를 호출해 입출력 버퍼 둘다 닫는다. close() 호출 또한 출력 버퍼에 있는 데이터와 EOF 까지 전송하면 출력 버퍼를 닫는다.
3. 출력 버퍼만 닫힌 host A는 host B에게 받은 데이터를 입력 버퍼에서 읽고 입력 버퍼를 닫는다.

동시에 입출력 버퍼를 닫는 것이 아니라 출력 버퍼닫고 받을 데이터를 모두 받은 다음 입력 버퍼를 닫는 half-close 개념을 적용하면 패킷이 버려지는 일도 없다.<br>
host B의 경우 half-close가 아닌 close() 호출로 입출력 버퍼를 둘다 닫도록 했는데 half-close 방식을 사용해도 상관없다. 과정 3을 제외하고 과정 1, 과정 2에선 상대방에게 EOF를 전달한다.<br><br>

## shutdown()

half-close 하기 위해 shutdown() 을 호출한다.<br>

```c
#include <sys/socket.h>

int shutdown(int sock, int howto);
```
- sock : 종료할 소켓의 파일 디스크립터
- howto : 종료방법을 대한 정보
    - SHUT_RD : 입력 스트림 종료
    - SHUT_WR : 출력 스트림 종료
    - SHUT_RDWR : 입출력 스트림 종료


### 먼저 연결을 끊고자 하는 쪽 (half-close)

```c
1  while(1) {
2      read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
3      if(read_cnt < BUF_SIZE) {
4         write(clnt_sd, buf, read_cnt);
5         break;
6  
7      }
8      write(clnt_sd, buf, BUF_SIZE);
9  }
10
11 shutdown(clnt_sd, SHUT_WR);
12
13 read(clnt_sd, buf, BUF_SIZE);
14 printf("Message from client : %s \n", buf);
15
16 fclose(fp);
17 close(clnt_sd);
18 close(serv_sd);
```

line 11 : write buffer를 먼저 닫는다.
line 17 : connect()에서 리턴한 데이터 소켓을 닫는데 이때 read buffer를 닫는다.
line 18 : 연결을 기다리는 서버소켓을 닫는다.

### 연결을 끊겠다고 통보받은 쪽

```c
1  while((read_cnt = read(sd, buf, BUF_SIZE)) != 0)  -> EOF를 받을 때까지 읽는다.
2      fwrite((void*)buf, 1, read_cnt, fp);
3    
4  puts("Received file data");
5  write(sd, "Thank you", 10);
6
7  fclose(fp);
8  close(sd);
```