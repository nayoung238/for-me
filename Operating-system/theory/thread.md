## Thread

프로세스 내에서 작업 단위로 시스템의 여러 자원을 할당받아 실행하는 프로그램 단위를 thread 라고 한다.

![](/_img/Operating-system/thread1.png)

프로세스 A 는 1부터 100 까지 더하는 연산을 처리해야한다. 하지만 10개를 더할때마다 I/O 가 발생해 context switching 이 일어난다면 프로세스 A 가 처리하기 위해 소요되는 시간은 점점 증가할 것이다.

![](/_img/Operating-system/thread2.png)

하지만 ```1 ~ 10```, ```11 ~ 20``` 처럼 10개 단위로 병렬 연산 후 모든 값을 더해도 상관없다. 즉, **I/O 발생으로 Block 상태가 되어도 다음 연산에 영향을 미치지 않기 때문에 병렬 연산이 가능하다.** 그러므로 프로세스 A 가 처리해야할 연산을 **일정단위로 나눠 연산** 후 다른 프로세스에게 CPU 권한을 넘겨주면 이전보다 더 빠른 속도로 처리가 가능하다. 이러한 단위를 thread 라고 한다.
<br>

## 병행성 vs 병렬성

병행성과 병렬성은 동일한 시간 동안 여러 프로세스를 동시에 실행한다는 의미를 담고있지만 전혀 다른 개념이다.

- 병행성: 다중 프로세서 시스템에서 동일한 시간에 별로의 프로세서에서 실행하는 것
- 병렬성: 프로세스 수명이 겹치는 것이지 모두 동일한 시간에 처리되는 것이 아님
<br>

## Multi Process

- 프로세스는 자신만의 주소공간을 갖는다.
- IPC (Inter Process Communication) 을 통해 프로세스 간의 통신이 가능하다.
<br>

## Multi Thread

- multi thread 환경에서 프로세스는 하나 이상의 스레드를 갖는다. 또한 프로세스를 **자원을 보호하는 단위**, **메모리 할당의 단위**로 정의한다. 
- multi thread 환경에서 스레드는 context switching 단위이다.
<br>

## Thread 생성

Process 내에서 Thread를 생성하는 것은 비교적 간단하다. Thread도 자신만의 실행 상태를 저장하기 위한 TCB (Thread Control Block), User stack 그리고 Kernel stack 공간을 할당받는다. 그리고 같은 Process 내의 모든 Thread가 Process의 **Data, Heap을 공유**하기 때문에 문맥 교환에서 발생하는 오버헤드가 적다.

Thread가 자신만의 Stack 공간을 할당받는다는 것은 서로 독립적이며 독립적인 함수 호출 그리고 독립적인 실행 흐름이 가능하다는 의미이다. 또한 프로세스는 여러 스레드를 갖기 때문에 PCB는 TCB의 리스트를 가리키고 있다.

Multi-Process에서 자식 Process를 생성하기 위해 ```fork()``` 호출하면 부모 Process의 메모리 영역을 모두 copy 하기 위한 시간 및 공간이 소요된다. 이를 해결하기 위해 Thread가 만들어졌으며 Thread를 LWP (Light Weight Process) 라고도 한다.
<br>

## 메모리 공유

Multi-Process 환경에서 각 프로세스는 독립적인 메모리 영역을 갖으며 서로 공유하지 않는다. 즉, 프로세스 간 통신으로 공유해야할 경우 커널이 개입해야하는 번거로움이 있다.

Thread의 경우 같은 프로세스 내에서 모든 스레드는 Stack 을 제외한 **Code, Data 그리고 Heap 영역을 공유**한다. 그렇기 때문에 Multi-Thread 환경에서 Thread 간 데이터 공유가 쉽다. 

처리 속도가 빠른 프로세서와 상대적으로 느린 메인 메모리의 속도 차이를 보완하는 고속 버퍼를 캐시라고 한다. 캐시 메모리를 통해 CPU에서 읽은 데이터를 저장하고 CPU가 다시 요구하는 경우 메인메모리를 거치지 않고 캐시에 저장된 데이터를 바로 전달해 빠른 속도로 처리가 가능하다. 프로세스의 경우 서로 메모리를 공유하지 않기 때문에 문맥 교환이 일어나면 **캐시 메모리에 있는 모든 데이터를 리셋**한다. 캐시는 Data와 Heap영역을 공유하기 때문에 문맥 교환이 일어나도 **캐시 메모리를 리셋할 필요가 없다**. 이러한 이유로 문맥교환시 스레드의 속도가 더 빠르다.

하지만 공유한다고 좋은 점만 있는 것은 아니다.

### 예외 처리

Multi-Process 환경은 프로세스 간 메모리를 공유하지 않으므로 하나의 프로세스가 오류가 나더라도 다른 프로세스에게 영향을 주지 않는다.

하지만 Multi-Thread 는 Thread 간의 메모리를 공유하기 때문에 하나의 Thread가 예외를 발생시키면 같은 Process에 있는 다른 Thread에게 영향을 미치기 때문에 적절한 예외 처리가 필요하다.

### 동시성 문제

Thread의 실행 순서를 파악할 수 없어 디버깅이 어렵고 동시성(concurrency) 문제가 발생한다.

Thread는 Data, Heap 영역을 공유하기 때문에 여러 Thread가 같은 변수에 접근해 읽고 수정하는 과정에서 동시성 문제가 발생한다. 여러 변수가 같은 변수에 접근해 **읽기만 한다면** 문제가 되지 않는다. 하지만 **수정을 할 경우 동기화 문제가 발생**되므로 Multi-Threading 환경에서는 동기화 작업이 필요하다.
<br>

# Thread 상태

![](/_img/Operating-system/thread_state.png)

Java thread 를 기준으로 설명한다.

### NEW: Thread 생성

- Process가 생성되면 해당 Process를 위한 Thread가 함께 생성된다.
- 각 Thread는 자신의 Register 문맥과 Stack 공간을 갖는다.
- 필요할 때마다 Thread를 생성하면 비용이 들기 때문에 **어느정도의 Thread를 미리 만들어두는 Thread Pool**이 존재한다.

### RUNNABLE: 실행 대기

- Thread 생성 후 ```start()```를 호출하면 RUNNABLE 상태가 된다.
- 언제든지 실행 상태가 될 수 있는 상태이다.

### BLOCK 일시 정지

Thread가 실행 대기가 아닌 일시 정지 상태가 되기도 하는데 Java thread 에서 Block 상태를 3가지로 나눈다.

- WAITING : 다른 Thread가 통지할 때까지 기다리는 상태
- TIMED_WAITING : 주어진 시간 동안 기다리는 상태
- BLOCKED : 사용하고자 하는 객체의 Lock이 풀릴 때까지 기다리는 상태

❓특정 Thread가 Block 되면 해당 Process도 Block 될까? 그렇지 않다.
특정 Thread가 Block 되어도 같은 Process 내의 다른 Thread는 영향받지 않기 때문에 Process가 Block 되지 않는다. 만일 특정 Thread로 인해 프로세스가 Block 된다면 Thread가 지닌 장점과 유연성이 사라지게 된다.

### suspend: 보류

- 보류 상태는 Thread가 아닌 Process에만 해당 된다.
- 보류는 Page 단위로 이루어지기 때문에 Process가 보류된다는 것은 **Swap 영역으로 방출되어 실행할 수 없는 상태**를 의미한다. 
- **Thread는 메모리를 공유하기 때문에 Process가 보류상태가 되면 모든 Thread가 보류 상태가 된다.** 
- 즉, 보류 상태는 각 Thread에 적용할 수 업기 때문에 보류 상태를 Thread에서 사용하지 않는다.