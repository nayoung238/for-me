# TCP/IP (Transmission Control Protocol / Internet Protocol)

- ```TCP/IP``` 프로토콜 스택이란 **인터넷 기반의 데이터 송수신을 목적으로 설계된 스택**
- IP 주소체계를 따르고 IP Routing을 이용해 목적지에 도달하며 TCP의 특성을 활용해 송신자와 수신자의 연결과 데이터 전송을 보장
<br>

- OSI 7계층에서 상위 3개의 계층을 Application 계층
- 나머지 하위 4계층을 Data flow 계층 (데이터 송수신 과정을 전문적으로 나눔)
- Transport(4계층): TCP/UDP 프로토콜로 end to end (출발지와 최종 도착지)의 논리적 연결(3-way handshake)과 데이터 전송 상태를 체크
- Network(3계층): end to end 사이의 **최적의 라우터 경로**를 제공하며 이를 Routing 이라고 함
- Data link(2계층): Link 계층으로 P2P (출발지와 라우터, 라우터와 라우터, 라우터와 도착지처럼 근접한 사이)
<br>

- ```TCP/IP``` 는 OSI 7 계층에서 Network(3계층), Transport(4계층) 을 다루는 프로토콜
-  ```IP``` 는 **목적지를 제대로 찾아가는 것**에 초점두며 비연결성, 비신뢰성의 문제가 있음
- ```TCP``` 는 **서로가 통신 가능한 상태인지(3-way handshake), 패킷이 모두 전송되었는지 등 데이터 전송의 보장**에 초점

<br>

## IP Protocol의 한계

- 비연결성: 상대가 없거나 서비스 불능 상태여도 패킷 전송
- 비신뢰성: 패킷이 사라지거나 순서를 보장하지 않음
- IP Protocol의 한계를 해결하기 위해 TCP 같이 사용

<br>

## 흐름 제어

- 송신자는 수신자가 받을 수 있는 정도의 데이터만 보내야 함
- TCP header 의 ```window size``` 로 받고 보낼 수 있는 데이터의 양을 정함
- 데이터 송수신에서 수신하는 입장의 상태가 더 중요하기 때문에 window size 는 수신자가 3-way handshake 과정에서 설정
- 그 후 통신과정에서 상황에 맞게 조절 가능

<br>

## 혼잡 제어

- 데이터 송수신을 위해 네트워크 상태도 중요 
- 여러 방법이 존재하는데 아래 링크 참고
- https://gyoogle.dev/blog/computer-science/network/%ED%9D%90%EB%A6%84%EC%A0%9C%EC%96%B4%20&%20%ED%98%BC%EC%9E%A1%EC%A0%9C%EC%96%B4.html

<br>

### reference

- TCP/IP 란: https://aws-hyoh.tistory.com/entry/TCPIP-%EC%89%BD%EA%B2%8C-%EC%9D%B4%ED%95%B4%ED%95%98%EA%B8%B0