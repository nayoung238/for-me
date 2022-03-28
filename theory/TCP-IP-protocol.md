# TCP/IP (Transmission Control Protocol / Internet Protocol)

```TCP/IP``` 프로토콜 스택이란 **인터넷 기반의 데이터 송수신을 목적으로 설계된 스택**이다.
OSI 7계층에서 상위 3개의 계층을 Application 계층이라하고 나머지 하위 4계층을 Data flow 계층이라고 한다. Data flow 계층인 하위 4개의 계층에서 데이터 송수신 과정을 전문적으로 나눈다.

- Transport(4계층): TCP/UDP 프로토콜로 end to end (출발지와 최종 도착지)
- Network(3계층): IP(Internet Protocol)로 소켓이 전송되는 **최적의 라우터 경로**를 제공
- Data link(2계층): Link 계층으로 P2P (출발지와 라우터, 라우터와 라우터, 라우터와 도착지처럼 근접한 사이)

```TCP/IP``` 는 OSI 7 계층에서 Network(3계층), Transport(4계층) 을 다루는 프로토콜이다. ```IP``` 는 목적지를 제대로 찾아가는 것에 중점을 둔다면 ```TCP``` 는 서로가 통신 가능한 상태인지(3-way handshake), 패킷이 모두 전송되었는지 등 데이터 전송의 보장에 초점을 둔다.

## 흐름 제어

송신자는 수신자가 받을 수 있는 정도의 데이터만 보내야 한다. TCP header 의 ```window size``` 로 받고 보낼 수 있는 데이터의 양을 정한다.

데이터 송수신에서 수신하는 입장의 상태가 더 중요하기 때문에 window size 는 수신자가 3-way handshake 과정에서 설정한다. 그 후 통신과정에서 상황에 맞게 조절 가능하다.

<br>

## 혼잡 제어

데이터 송수신을 위해 네트워크 상태도 중요하다. 여러 방법이 존재하는데 아래 링크를 참고하자.

<br>

### reference

- TCP/IP 란: https://aws-hyoh.tistory.com/entry/TCPIP-%EC%89%BD%EA%B2%8C-%EC%9D%B4%ED%95%B4%ED%95%98%EA%B8%B0
- 흐름제어, 혼잡제어에 대한 내용: https://gyoogle.dev/blog/computer-science/network/%ED%9D%90%EB%A6%84%EC%A0%9C%EC%96%B4%20&%20%ED%98%BC%EC%9E%A1%EC%A0%9C%EC%96%B4.html
