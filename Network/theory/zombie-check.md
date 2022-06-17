## 프로세스

- 프로세스을 쉽게 말하면 실행 중인 프로그램
- 실행중인 프로그램에 관련된 메모리, 리소스 등을 총칭하는 의미
- PID : 프로세스의 고유한 식별자

```c
#include <unistd.h>
pid_t fork();

// 성공 시 PID, 실패 시 -1 return
// 더 정확하게는 성공 시 부모 프로세스에겐 자식의 PID, 자식에겐 0 return
```

<br>

## 좀비 프로세스

```exit(1)```, ```return 0``` 등 을 만나면 해당 프로세스는 죽는다. 
죽을 때 자신의 PCB에 자신이 어떻게 죽었는지 **exit_status** 에 기록한다.<br>

부모 프로세스는 죽은 자식 프로세스를 소멸시켜야 한다. 
이를 위해 자식 프로세스가 종료되면서 반환하는 상태 값이 부모 프로세스에게 전달되어야 한다. 
만약 부모 프로세스에게 **죽은 자식의 상태가 전달되지 않으면 좀비상태로 변한다.**

- **실행이 완료되었음에도 불구하고 소멸되지 않은 프로세스를 좀비 프로세스**라고 함
- 소멸되지 않았다는 것은 **프로세스가 사용한 리소스가 메모리 공간에 여전히 존재**
<br>

## wait(), waitpid()

```wait()``` 으로 자식 프로세스가 종료할 때 까지 기다렸다가 자식 프로세스의 종료 상태를 받는다.

```c
#include <sys/wait.h>

pid_t wait(int *statloc);
// 정상적인 경우 종료한 자식 프로세스 PID, 오류 시 -1 return
```

- zombie_wait.c 코드 : https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/signal/zombie_wait.c

<br>

pointer로 넘겨준 매개변수에 자식이 종료한 상태를 **2byte 로 저장**한다.

- 정상 종료인 경우 -> **상위 8비트**에 프로세스 반환 값을 저장
- 비정상 종료인 경우 -> **하위 8비트**에 종료 시킨 시그널 번호를 저장
  - 비정상 종료에서 특정 시그널은 core dumped 파일을 생성하는데 이 떄 하위 8비트 중 최상위 1비트를 1로 설정
  - 예로 kill 3번 시그널 SIGQUIT (ctrl + backspace)

```c
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int* statloc, int options);
// 성공 시 종료된 자식 PID(또는 0), 실패 시 -1 return
```

- pid : 종료를 확인하고자 하는 자식 프로세스 PID
  - ```-1``` 을 전달하면 모든 프로세스의 상태를 확인 (임의의 프로세스가 죽으면 return)
- statloc : ```wait()```의 statloc 과 같은 기능
- options : sys/wait.h 에 선언된 **WNOHANG** 을 전달하면 **종료된 자식 프로세스가 존재하지 않으면 block 되지 않고 0을 리턴하며 함수를 빠져나옴**

```wait()``` 은 blocking 상태에 빠질 수 있지만, ```waitpid()``` 에 **WNOHANG** 옵션을 주면 **blocking 상태에 놓이지 않게 된다**는 차이점이 있다.

- zombie_waitpid.c 코드 : [https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/signal/zombie_waitpid.c](https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/signal/zombie_waitpid.c)

<br>

### process 종료 상태 확인 매크로 

- WIFEXITED(statloc) : 자식 프로세스가 정상 종료한 경우 return **non-zero**
- WEXITSTATUS(statloc) : return 이나 ```exit()```에서 전달된 **종료 코드** return
  - ```WIFEXITED()```의 return **non-zero** 일 때만 사용
- WIFSIGNALED(statloc) : 시그널에 의해 종료 시 return **non-zero**
- WTERMSIG(statloc) : 프로세스를 종료시킨 **시그널 번호** return
  - ```WIFSIGNALED()``` return **non-zero** 일 때만 사용
<br>

## signal()

OS가 프로세스에게 특정 상황이 발생했음을 비동기적으로 알리는 일종의 메시지를 시그널이라고 한다.<br>
```signal()``` 보단 ```sigcation()``` 사용을 권장한다.

![png](/Network/_img/signal.jpeg)<br>

> 출처 : https://blockdmask.tistory.com/23 <br>

<br>

```c
#include <signal.h>

void (* signal(int signo, void(*func)(int))) (int);
// 시그널 발생시 호출되도록 이전에 등록된 함수의 포인터 리턴
```
- signo : 시그널 번호
- void (*func)(int) : 시그널을 처리할 핸들러

- signal.c 코드 : [https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/signal/signal.c](https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/signal/signal.c)

signal.c 코드를 보면 for문 안에서 ```sleep(20)``` 을 수행하도록 작성했다.<br>
하지만 for문 전 ```alarm(2)``` 에 의해 시그널이 발생해 시그널 처리로 인해 20초를 기다리지 못하고 끊긴다.<br>

또한, for문 수행 중 ```CTRL + C``` 를 입력하면 ```keycontrol()``` 이 수행되는 동시에 for문도 계속돈다.<br>
이를 통해 시그널은 **비동기적으로 처리**됨을 알 수 있다.

<br>

## sigaction()

```sigaction()```은 ```signal()```의 상위 버전이다.

```c
#include <signal.h>

int sigaction(int signo, const struct sigaction* act, struct sigaction* oldact);
// 성공 시 0, 실패 시 -1 return
```

- signo : 시그널 번호
- act : 첫번째 매개변수인 시그널 발생시 호출될 함수(시그널 핸들러)
- oldact : 이전에 등록되었던 시그널 핸들러의 함수 포인터를 얻는데 사용되는 인자로 필요없으면 0

```sigcation()```의 두번째 매개변수 타입은 sigaction 구조체이다.<br>

```c
struct sigaction {
  void (*sa_handler)(int);
  sigset_t sa_mask;
  int sa_flags
}
```

- sa_handler : 시그널 발생시 실행할 핸들러
- sa_mask : sa_handler 가 수행될 때는 sa_mask 에 등록한 signal 을 막는다. -> block 
- sa_flags : sa_mask, sa_flags 시그널 관련 정보의 추가 전달에 사용

- sigaction.c 에 대한 코드 : https://github.com/evelyn82ny/Computer-science/blob/master/Network/code/signal/sigaction.c