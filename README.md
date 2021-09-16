
## Port forwarding

공유기 외부에서 공유기 내부로 접속하기 위해 네트워크 주소를 변환해주는 것을 port forwarding 또는 port mapping이라고 한다.<br>![png](/_img/ubuntu_ip.png)<br>**ifconfig**명령어로 Host IP가 ```127.0.0.1```임을 알 수 있고, mac os에서 검색해도 당연히 동일한 결과를 보여준다.<br><br>![png](/_img/chap1_hello_result.png)<br>서버를 가상머신에서 돌리기 때문에 port forwarding 설정을 해야한다.<br>9190 port로 설정했으며 외부에서 Host PC로 들어오는 socket 중 port 번호가 9190 인 것은 Host에서 처리하지 않고 Guest로 넘겨준다.<br>넘어오는 socket이 Host의 것인지, Guest의 것인지 구분하는 것은 NIC(Network Interface Card)가 처리한다.<br><br>VMnet8에 연결된 상태이므로 Guest가 외부로 데이터를 보내도 최종적으로 Host의 IP로 나간다.<br>받은 데이터에 응답을 하기 위해 Guest로 socket을 보내도 Guest IP를 전달받은 것이 아닌 Host IP만 전달되었으므로 Host PC에게 socket이 전달된다.<br>NIC가 Host PC가 받은 socket의 port를 확인해 Guest쪽으로 넘겨준다.<br>Guest로 넘어온 socket은 NAT(Network Address Translation)에 의해 해당되는 application으로 전달된다.<br>전달 가능한 이유는 Guest가 외부로 데이터를 보내면서 변경되는 IP를 기록해 두기 떄문이다.<br><br>

## TCP가 전송하는 데이터는 경계가 존재하지 않는다.

TCP로 전달되는 데이터의 특징은 경계가 존재하지 않는다.<br>하지만 UDP로 전달되는 데이터는 경계가 존재한다.<br>즉, TCP로 전달되는 데이터는 읽고 싶은 만큼 나눠서 받아도 되고, UDP로 전달되는 데이터는 무조건 주는 만큼 다 받아야 한다.<br>
```c
while(read_len=read(sock, &message[idx++], 1))
	{
		if(read_len==-1)
		{
			error_handling("read() error!");
			break;
		}
		str_len+=read_len;
	}
```

tcp_client.c 의 일부코드이다.<br>해당 client는 전달받은 데이터를 1byte씩 읽는 방식이다.<br>![png](/_img/chap@_tcp_result.png)<br>몇번 읽었는지 확인하기 위해 strlen을 출력한 결과 13이 출력되었다.<br>![png](/_img/helloworld.png)<br>null까지 읽고 ```EOF(End Of File)``에 접근한 순간 return 0 하기 때문에 13이 출력되었다.<br><br>
```c
char message[30];

str_len=read(sock, message, sizeof(message)-1);
```

hello_client.c 의 일부코드이다.<br>해당 client는 전달받고자 하는 크기인 30보다 1개 적은 29만큼만 읽는 방식이다.<br>여기서 1을 제외한 이유는 null을 처리해야 되기 떄문이며, 해당 방식은 1번에 모두 읽는다.<br>tcp_client는 13번동안 데이터를 읽었다면, hello_client는 1번에 데이터를 읽었다.<br>
```c
sock=socket(PF_INET, SOCK_STREAM, 0);       // client
serv_sock=socket(PF_INET, SOCK_STREAM, 0);  // server
```
client와 server 모두 TCP를 사용하기 위해 SOCK_STREAM으로 설정했기 때문에 자신이 원하는 사이즈만큼 읽을 수 있던 것이다.<br>
