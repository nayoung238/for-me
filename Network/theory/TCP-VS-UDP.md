# TCP 와 UDP

Transport 계층은 **출발지와 최종 목적지까지(end to end)** 의 데이터 송수신하는 계층으로 주로 TCP 나 UDP 를 사용한다.

<br>

## TCP (Transmission Control Protocol)

- 3-way handshake로 서로의 연결을 확인한 후 데이터를 전송하는 **연결 지향형** 프로토콜로 해당되는 모든 정보는 TCP Header에 존재
- 데이터의 전송을 보장하는 신뢰성있는 프로토콜 (분실, 중복, 순서 등의 문제를 해결)
- 데이터의 전송을 보장하기 때문에 UDP보다 복잡하고 속도가 느림
- 소켓끼리 일대일 연결 후 통신하므로 1번만 서로의 목적지를 설정
- 소켓 전달시 SEQ, ACK 같은 신호로 데이터의 전송을 보장
- Receiver에게 패킷을 전송했을 때 Receiver로부터 전송한 패킷에 대한 응답을 받지 못하면 Time-out으로 처리해 다시 보냄 (데이터 전송 보장)

<br>

## UDP (User Datagram Protocol)

- 서로의 연결을 확인하지 않고 한쪽에서 보내는 방식으로 **비연결** 프로토콜
- 데이터의 전송을 보장하지 않기 때문에 SEQ, ACK 같은 신호가 없음 (신뢰도 떨어짐)
- 데이터의 전송을 보장하지 않기 때문에 TCP 보다 구조가 단순 (용량이 가벼움)
- 전송을 보장하지 않아 전송 체크를 할 필요가 없어 TCP 보다 상대적으로 속도가 빠름
- 안전성보다 성능이 중요하다면 UDP 를 사용 (실시간 방송)
- 비연결형 통신으로 여러 소켓과 일대다 통신이 가능 (broadcast, multicast)
- 비연결형 특성이므로 connection 이라는 개념이 적용되지 않아 통신할 때마다 수신자를 지정해야함

<br>

### DNS (Domain name service) 는 UDP 사용

- DNS Request 에 사용되는 용량을 UDP segment 가 처리 가능
- UDP 제한 크기인 512 를 넘어가면 TCP 사용해야함
- UDP 는 데이터 전송을 보장하지 않기 때문에 3-way handshake 과정을 거치지 않고 connection 을 유지하지 않음
- DNS 도 connection 을 유지할 필요없기 떄문에 UDP 사용이 적합

하지만 DNS 서버 간의 요청을 주고 받을 때 사용하는 zone transfer 에서는 TCP 를 사용한다.

