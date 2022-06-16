# Iterative server

1. ```socket()```
2. ```bind()```
3. ```listen()```
4. ```accept()```
5. ```read()``` / ```write()```
6. ```close(client)```
7. ```close(server)```

과정6에서 클라이언트가 ```close()```를 호출하면 서버도 ```close()```를 호출하는 것이 아니고 **다시 과정3과 과정4 사이에서 머무르다가 클라이언트의 연결 요청이 들어오면** ```accept()```하여 데이터를 주고 받는다.
동시에 둘 이상의 클라이언트 요청을 받진 못하고 하나씩 처리하는데 이것을 Iterative server 라고 한다.

<br>

## Iterative server

```c
1  for(int i = 0; i < 3; i++) {
2    
3    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &clnt_adr_sz);
4    if(clnt_sock == -1)
5        error_handling("accept() error");
6    
7    while(read_len = read(clnt_sock, &buf, BUF_SIZE)) {
8        if(read_len == -1)     
9           error_handling("read() error");
10       write(clnt_sock, &buf, read_len);
11  }
12  close(clnt_sock);
13 }
```

- 전체 코드 : [https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/iterative-server/iterative-server.c](https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/iterative-server/iterative-server.c)

클라이언트가 보낸 데이터가 서버의 입력버퍼에 계속 쌓이고 서버는 입력버퍼를 일정 크기로 읽으며 출력버퍼에 쌓는다.

- line 1 : TCP Iterative server 는 3번의 서비스를 제공
- line 7 : ```read()``` 가 **0** 을 리턴할 때까지 read buffer 에서 BUF_SIZE 만큼 read
- line 10 : 읽은 만큼 서버의 출력버퍼에 저장

TCP 통신에서 데이터는 경계가 없기 때문에 read buffer에 있는 데이터를 읽고 싶은 만큼만 읽어도 된다.<br>
또한, 서버는 데이터의 경계를 구분하지 않고 수신한 데이터를 그대로 전송할 의무만 갖는다.<br>
blocking mode에서 ```read()```가 **0** 을 리턴하는 경우는 클라이언트에서 ```close()```를 호출해 **EOF**를 보냈을 때이다.

<br>

## Echo client

echo client란 클라이언트에서 write 한 것을 server에 보내고 server는 해당 데이터를 다시 클라이언트에게 보내 그대로 출력하는 것을 말한다.<br>

```c
1 while(1) {
2    
3    fputs("Input message(Q to quit) : ", stdout);
4    fgets(buf, BUF_SIZE, stdin);
5        
6    if(!strcmp(buf, 'q\n') || !strcmp(buf, "Q\n"))
7        break;
8        
9    write_len = write(sock, buf, strlen(buf));
10 }
11      
12 while(read_total_len < write_len) {
13      read_len = read(clnt_sock, &buf[read_total_len], BUF_SIZE - 1);
14      read_total_len += read_len;
15 }
```

- 전체 코드 : [https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/iterative-server/echo-client.c](https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/iterative-server/echo-client.c)

- line 6 : TCP echo client 는 **q** 나 **Q**가 입력을 들어올 때까지 read
- line 9 : ```write()```를 호출해 읽은 데이터를 클라이언트 출력버퍼에 쌓으면 서버의 입력버퍼로 이동
  - 해당 코드는 ```write()```를 1번 호출했지만 내부적으로는 여러 소켓으로 나눠 전송될 수도 있음
- line 12 : 클라이언트는 자신이 보낸 데이터의 크기만큼 읽을 때까지 while 문을 수행
- line 13 : 서버가 보낸 데이터는 클라이언트의 입력버퍼에 쌓이는데 해당 클라이언트는 BUF_SIZE - 1 크기만큼씩 read

서버 코드의 ```line 7: while(read_len = read(clnt_sock, &buf, BUF_SIZE))``` 부분에서 클라이언트가 ```EOF``` 를 보낼 때까지 읽은 데이터를 출력버퍼에 데이터그램 단위로 쌓아 클라이언트에게 전송한다.<br>
즉, 클라이언트가 한번에 보낸다해도 서버가 한번에 보내준다는 보장이 없어 ```write()```를 한번만 호출할 경우 데이터의 일부만 읽게 될 수도 있기 때문에 while 문을 사용해 ```read()``` 해야한다.<br>
클라이언트 측의 ```read()```가 return 하는 누적값이 서버에게 보내기 위해 호출했던 ```write()``` return 값과 동일할 때까지 while 문을 돌린다.

<br>

## 서버와 클라이언트간의 데이터 송수신 명세

위 내용을 보면 상대가 보내는 데이터의 크기를 모르기 때문에 **EOF**를 마주할 때까지 while 문을 실행해야한다.<

```c
input count : 3
input 1 : 10
input 2 : 20
input 3 : 24
// 프로그램 종료
```

input count가 출력되고 **사용자는 3번의 입력을 하겠다**고 입력했다. 그럼 앞으로 3번의 입력을 한 다음 프로그램은 종료한다.<br>

클라이언트에서 앞으로 3번의 입력을 하겠다라고 명시했기 때문에 서버 측에서는 3 x 4byte = 12byte 를 읽으면 모든 데이터를 읽을 수 있다고 파악 가능하다.<br>
이렇게 서버와 클라이언트 사이에서 데이터 송수신 명세가 **프로토콜**이다.<br>
