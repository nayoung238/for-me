## Port forwarding

공유기 외부에서 공유기 내부로 접속하기 위해 네트워크 주소를 변환해주는 것을 port forwarding 또는 port mapping이라고 한다.<br><br>
![png](/Network/_img/ubuntu_ip.png)<br>
**ifconfig**명령어로 Host IP가 ```127.0.0.1```임을 알 수 있다.<br><br>
![png](/Network/_img/chap1_hello_result.png)<br>
서버를 가상머신에서 돌리기 때문에 port forwarding 설정을 해야한다.<br>
9190 port로 설정했으며 외부에서 Host PC로 들어오는 socket 중 port 번호가 9190 인 것은 Host에서 처리하지 않고 Guest로 넘겨준다.<br>
넘어오는 socket이 Host의 것인지, Guest의 것인지 구분하는 것은 NIC(Network Interface Card)가 처리한다.<br><br>
VMnet8에 연결된 상태이므로 Guest가 외부로 데이터를 보내도 최종적으로 Host의 IP로 나간다.<br>
받은 데이터에 응답을 하기 위해 Guest로 socket을 보내도 Guest IP를 전달받은 것이 아닌 Host IP만 전달되었으므로 Host PC에게 socket이 전달된다.<br>
NIC가 Host PC가 받은 socket의 port를 확인해 Guest쪽으로 넘겨준다.<br>
Guest로 넘어온 socket은 NAT(Network Address Translation)에 의해 해당되는 application으로 전달된다.<br>
전달 가능한 이유는 Guest가 외부로 데이터를 보내면서 변경되는 IP를 기록해 두기 떄문이다.<br><br>
