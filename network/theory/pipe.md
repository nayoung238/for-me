# pipe

IPC (Inter Process Communication) 이란 프로세스간 데이터를 주고받는 방식을 말한다. 
주고 받는 방식에는 메모리 공간을 공유하거나 일반 파일, 파이프 파일로 공유하는 방법이 존재한다.<br>

모든 프로세스는 자신만의 가상주소공간을 갖기 때문에 프로세스 A가 프로세스 B의 메모리 공간에 접근이 불가능하다. 
접근하기 위해선 OS가 별도의 메모리 공간을 마련해 줘야 프로세스 통신이 가능하다.<br>

일반 파일로 공유하면 데이터가 계속 쌓여 용량이 커지는 문제가 발생한다. 
이와 다르게 파이프 파일은 읽으면 지워지기 때문에 일반 파일의 단점을 해결할 수 있다.

<br>

## 파이프 기반의 통신

```c
#include <unistd.h>

int pipe(int filedes[2]);
// 성공 시 0, 실패 시 -1 return
```

- ```filedes[0]``` : 데이터를 수신하는데 사용되는 파일 디스크립터가 저장 (파이프의 출구)
- ```filedes[1]``` : 데이터를 전송하는데 사용되는 파일 디스크립터가 저장 (파이프의 입구)

pipe 함수를 호출하면 **os는 서로 다른 프로세스가 함께 접근할 수 있는 메모리 공간을 만든다.**

<br>

## 파이프 1개로 통신

![png](/_network/_img/single_pipe.png) 

1개의 파이프를 부모와 자식 프로세스가 함께 사용하며, 자식 프로세스가 파이프에 데이터를 작성하면 부모 프로세스가 읽고 출력하는 코드이다.

```c
1  int main(){
2      char str[] = "who are you?";
3      char buf[BUF_SIZE];
4
5      int fds[2];
6      pipe(fds);
7      pid_t pid = fork();
8
9      if(pid == 0) 
10        write(fds[1], str, sizeof(str));
11     else {
12        wait(NULL);
13        read(fds[0], buf, BUF_SIZE);
14        puts(buf);
15     }return 0;
16 }
```
> 전제 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/pipe/single_pipe.c

자식 프로세스가 먼저 실행된다는 보장이 없으니 line 12에 ```wait(NULL)``` 코드를 추가해 자식 프로세스가 죽으면 부모 프로세스가 실행되도록 작성했다.<br>

만약 의도와 다르게 부모 프로세스가 먼저 실행되면 읽을 데이터가 없어 block 상태로 변한다. 
즉, ```read()```에서 block 된 상태이므로 부모 프로세스는 새로운 ```write()```를 시도할 수 없어 문제가 된다.

<br>

이번엔 1개의 파이프로 ```자식 -> 부모``` -> ```부모 -> 자식``` 와 같이 서로가 대화를 주고 받는 것처럼 구현해봤다.

![png](/_network/_img/single_pipe_issue.png) 

```c
1  int fds[2];
2  pipe(fds);
3  pid_t pid = fork();
4
5  if(pid == 0) {
6     write(fds[1], str, sizeof(str));
7     sleep(2);
8     read(fds[0], buf, BUF_SIZE);
9     printf("Child proc output : %s\n", buf);
10 }
11 else {
12    read(fds[0], buf, BUF_SIZE);
13    printf("Parent proc output : %s\n", buf);
14    write(fds[1], str2, sizeof(str2));
15    sleep(3);
16 }
17 return 0;
```

- line 6 : 자식 프로세스가 먼저 데이터를 보내고
- line 7 : CPU 권한이 부모 프로세스에게 넘어가도록 2초간 sleep
- line 12 : 자식 프로세스가 sleep 되어 CPU 권한이 부모 프로세스에게 넘어왔다면 버퍼를 읽는다.
- line 14 : 자식 프로세스에게 보낼 데이터를 작성한다.
- line 15 : CPU 권한이 자식 프로세스에게 넘어가도록 3초간 sleep
- line 8 : 부모 프로세스가 작성한 데이터를 읽는다.

위와 같은 순서로 실행되면 서로간의 통신에 문제 없다. 
하지만 실제로는 context switching 이 어느 시점에 일어나는지 예상할 수 없어 위와 같은 순서를 보장할 수 없다. 
즉, 1개의 파이프로 양방향 통신하는 것은 적절한 방법이 아니다.

<br>

## 파이프 2개로 통신

![png](/_network/_img/double_pipe.png)<br>

```c
int fds1[2], fds2[2];
pipe(fds1);
pipe(fds2);

pid_t pid = fork();
if(pid == 0) {
    close(fds1[0]);
    close(fds2[1]);

    write(fds1[1], str1, sizeof(str1));
    read(fds2[0], buf, BUF_SIZE);
    printf("Child proc output : %s\n", buf);
}
else {
    close(fds1[1]);
    close(fds2[0]);

    read(fds1[0], buf, BUF_SIZE);
    printf("Parent proc output : %s\n", buf);
    write(fds2[1], str2, sizeof(str2));
    sleep(2);
}
return 0;
```
> 전제 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/pipe/double_pipe.c


- fds1는 자식이 전송하고 부모가 수신하는 파이프이다. 그러므로 자식은 수신 역할인 **fds1[0]** 를 닫고 부모는 발신 역할인 **fds1[1]** 를 닫는다.
- fds2는 부모가 전송하고 자식이 수신하는 파이프이다. 그러므로 자식은 발신 역할인 **fds2[1]** 를 닫고 부모는 수신 역할인 **fds2[0]** 을 닫는다.

파이프 1개로 양방향 통신을 구현할 땐 ```wait(NULL)```, ```sleep()``` 으로 CPU 권한을 어느 정도 조절했지만 이는 사실상 정확한 컨트롤이 불가능했다.<br>
하지만 파이프 2개로 양방향 통신을 하면 데이터의 입출력의 타이밍에 따라서 데이터의 흐름이 영향 받지 않는다.

<br>

## pipe 사용하는 echo server 구현

- pipe 사용하는 echo server 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/pipe/pipe_echo_serv.c
- pipe 사용하는 echo client 코드 : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/pipe/pipe_echo_client.c

echo server 는 10개의 메시지만 받도록 설정했다. 즉, 여러 client 가 10개 이상의 메시지를 보내도 10개의 메시지만 저장한다.<br>

```close()```가 호출될 때 파일에 메시지가 저장되므로 10개 이상을 보내야 파일에 저장할 수 있다.
만약 여러 client 가 총 10개의 메시지를 보내기 전에 모두 종료했다면 해당 파일에는 메시지가 **아직 저장되지 않는 상태**이다.<br>