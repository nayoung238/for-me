# Iterative server

1. socket()
2. bind()
3. listen()
4. accept()
5. read()/write()
6. close(client)
7. close(server)

과정6에서 클라이언트가 close()를 호출하면 서버도 close()를 호출하는 것이 아니고 다시 과정3과 과정4 사이에서 머무르다가 클라이언트의 연결 요청이 들어오면 accept()하여 데이터를 주고 받는다. 이것을 Iterative server고 한다.<br>

## Iterative server 코드

```c
1 for(int i = 0; i < 5; i++) {
2    
3    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &clnt_adr_sz);
4    if(clnt_sock == -1)
5        error_handling("accept() error");
6    else
7        printf("Connected client %d\n", i + 1);
8        
9    while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
10        write(clnt_sock, message, str_len);
11   
12   close(clnt_sock);
13 }
```
위 코드는 TCP Iterative server 코드의 일부로 5번의 서비스(line1)를 제공한다.<br>
클라이언트가 보낸 데이터가 서버의 입력버퍼에 계속 쌓이고, 서버는 입력버퍼를 일정 크기로 읽으며 출력버퍼에 쌓는다.<br>

- line 9 : read가 0을 리턴할 때까지 read buffer를 BUF_SIZE만큼 읽는다.
- line 10 : read가 읽은만큼 서버의 출력버퍼에 보낸다.

TCP의 데이터는 경계가 없기 때문에 read buffer에 있는 데이터를 읽고 싶은 만큼만 읽어도 된다.<br>
또한, 서버는 데이터의 경계를 구분하지 않고 수신한 데이터를 그대로 전송할 의무만 갖는다.<br>
blocking mode에서 read()가 0을 리턴하는 경우는 클라이언트에서 close()를 호출해 EOF를 보냈을 때이다.<br><br>

### Echo client 코드

echo client란 클라이언트에서 write 한 것을 server에 보내고, server는 해당 데이터를 다시 클라이언트에게 보내 그대로 출력하는 것을 말한다.<br>

```c
1 while(1) {
2    
3    fputs("Input message(Q to quit) : ", stdout);
4    fgets(message, BUF_SIZE, stdin);
5        
6    if(!strcmp(message, 'q\n') || !strcmp(message, "Q\n"))
7        break;
8        
9    write(sock, message, strlen(message));
10   str_len = read(sock, message, BUF_SIZE - 1);
11   message[str_len] = 0;
12   printf("Message from server : %s", message);
13 }
```
위 코드는 TCP echo client 코드의 일부로 **q** 나 **Q**가 입력을 들어올 때까지 읽는다.<br>

- line 9 : write()를 호출해 message를 클라이언트 출력버퍼에 쌓으면 서버의 입력버퍼로 이동하는데 해당 코드는 write()를 1번 호출했지만 내부적으로는 여러 소켓으로 나눠 전송될 수도 있다.
- line 10 : 서버가 보낸 데이터는 클라이언트의 입력버퍼에 쌓이는데 해당 클라이언트는 BUF_SIZE - 1 크기만큼 한번만 읽었다.
- line 11 : read()가 BUF_SIZE - 1 크기만큼 읽은 이유는 NULL을 추가해야 string이 되기 때문이다.

line6 에서 **q** 나 **Q**가 입력으로 들어오면 **while문을 빠져나가는데 이때 close()를 호출**한다.<br>

해당 코드에서 line 10은 문제가 된다.<br>
서버는 데이터의 경계를 구분하지 않고 수신한 데이터를 그대로 전송하기만 하면 되고, TCP는 데이터의 경계가 존재하지 않기 때문에 write()를 여러번 호출해도 문제되지 않는다.<br>
이와 다르게 클라이언트는 line9처럼 문장 단위로 데이터를 송수신하기 때문에 데이터의 경계가 존재한다.<br>
즉, 클라이언트가 한번에 보낸다해도 서버가 한번에 보내준다는 보장이 없어 write()를 한번만 호출할 경우 데이터의 일부만 읽게 될 수도 있 .<br>

서버 코드의 line9, line10을 보면 read한 만큼 출력버퍼에 데이터그램 단위로 쌓여 클라이언트에게 전송한다.<br>
즉, 클라이언트도 1번의 read() 호출이 아닌 while문을 사용해야 한다.<br>
클라이언트 측의 read()가 return 하는 누적값이 서버에게 보내기 위해 호출했던 write() return 값과 동일할 때까지 while문을 돌린다.<br><br>

```c
1  str_len = write(sock, message, strlen(message));
2    
3  recv_len = 0;
4  while(recv_len < str_len) {
5        
6       recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
7       if(recv_cnt == -1)
8           error_handling("read() error!");
9       
10      recv_len += recv_cnt;
11  }
12  message[recv_len] = 0;  // null 추가해 string으로 만든다.
13  printf("Message from server : %s", message);
```
위의 문제를 해결하기 위해 while문을 적용했다.<br>
보낸 데이터의 크기인 ```str_len```만큼 읽을 때까지 read()의 return 값을 ```recv_len```에 누적시키며 같아질 떄까지 while문을 실행한다.<br><br>

## 서버와 클라이언트간의 데이터 송수신 명세

위 내용을 보면 상대가 보내는 데이터의 크기를 모르기 때문에 EOF를 마주할 때까지 while문을 실행해야한다.<br>

```c
input count : 3
input 1 : 10
input 2 : 20
input 3 : 24
// 프로그램 종료
```

input count가 출력되고 **사용자는 3번의 입력을 하겠다**고 입력했다.<br>
그럼 앞으로 3번의 입력을 한 다음 프로그램은 종료한다.<br>

클라이언트에서 앞으로 3번의 입력을 하겠다라고 명시했기 때문에 서버 측에서는 3 x 4byte = 12byte를 읽으면 모든 데이터를 읽을 수 있다고 파악 가능하다.<br>
이렇게 서버와 클라이언트 사이에서 데이터 송수신 명세가 **프로토콜**이다.<br>
