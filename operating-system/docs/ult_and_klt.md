## ULT vs KLT

스레드는 **사용자 수준 스레드 (ULT: User Level Thread)** 와 **커널 수준 스레드 (KLT: Kernel Level Thread)** 2가지 유형을 나눠진다.
<br>

## Kernel Level Thread

순수한 KLT 구현에서는 Thread 관리와 관련된 모든 작업이 커널에서 이루어진다.
- 라이브러리에 대한 모든 코드와 자료구조가 Kernel 공간에 존재하기 때문에 라이브러리의 함수 호출을 System call 이라고 함
- **OS에서 Kernel 스레드는 스케줄링의 단위**

### KLT 장점

- Kernel은 여러 처리기에게 같은 Process 내의 여러 스레드를 동시에 (정확하게는 빠르게 번갈아가며) 스케줄링 가능
- **특정 Process의 스레드가 Block 되면 Kernel은 같은 Process의 다른 스레드로 교환 가능**
- Kernel routine 자체가 Multi-Thread로 구성 가능하다.

### KLT 단점

- 같은 Process내의 스레드에서 다른 스레드로 제어를 넘길 때 Kernel 모드로 전환이 필요해 비용 발생

<br>

## User Level Thread

Application에서 스레드 라이브러리를 호출해 사용하는 스레드를 ULT 라고 한다. 대표적인 예로 P-thread (POSIX Threads), Java thread는 Thread 라이브러리에서 제공하는 함수를 호출해 사용한다.

ULT는 사용자 공간에 존재하므로 스레드와 관련된 일은 당연히 사용자 공간에서 처리된다.
- OS는 ULT를 자원할당의 관점에서 바라보기 때문에 해당 스레드가 몇개인지, 어떻게 돌아가는지 등에 대한 관련 정보를 파악하지 않는다.
- 즉, Kernel은 스레드의 존재를 알 수 없기 때문에 순수한 ULT 환경에서는 **프로세스가 스케줄링의 대상**이 된다.

### ULT 장점

- 스레드 관리를 위한 모든 자료 구조가 사용자 주소 공간에 존재해 스케줄링이나 동기화를 위해 커널 모드의 전환이 요구되지 않아 오버헤드가 적다.
- 기반이 되는 OS 스케줄러에 의존하지 않고 Application에 맞게 스케줄링을 구성 가능
- Kernel에 독립적으로 스케줄링을 할 수 있어서 어떤 OS에도 적용 가능

### ULT 단점

- 스레드가 아닌 프로세스 단위로 CPU를 할당하기 때문에 **특정 스레드가 Block 상태가 될 경우 같은 Process 내의 모든 스레드가 Block 상태**가 된다. 즉, 순수한 ULT 기반의 Multi-thread application은 다중 처리의 장점을 살릴 수 없다.
- Kernel은 한번에 1개의 처리기에 1개의 Process를 할당하므로 임의 시점에 단일 Process 내에서 1개의 스레드만 수행한다. 즉, Multi-thread를 사용해도 동시 처리라고 말할 뿐이지 사실상 한 부분씩 진행되기 때문에 특정 부분에서 Block 상태가 되면 모든 부분이 Block 되면서 Multi-processing 장점을 살릴 수 없다.

<br>

## ULT 와 KLT 의 결합

스레드 생성과 Application 내에서의 스레드 스케줄링 및 동기화를 사용자 공간에서 한다. 그 후 여러 ULT를 ULT 수보다 작거나 같은 KLT에 매핑하여 사용하며 현재 가장 많이 쓰이는 방식이다. 
- Application의 여러 스레드가 다수의 처리에서 병렬 수행 가능하며
- **블록형 시스템 호출 시 전체 Process를 Block 상태로 만들지 않는다**.
- 대표적인 예로 Solaris, BSD
<br>

ULT 와 KLT 의 결합에는 다음과 같은 여러 연관 관계가 존재한다.
<br>

## Pure user-level (Many-to-One)

![](/operating-system/_img/pure_user_level_thread.png)

- ULT 간 스케줄링이 발생해도 Kernel과 관련없는 작업이므로 모드 전환이 일어나지 않음
- 특정 스레드가 Block 상태가 되면 전체 Process가 Block 되는 병목 현상 발생
- 한번에 1개의 스레드만 처리할 수 있어 병렬 처리 불가능
<br>

## Pure thread-level (One-to-One)

![](/operating-system/_img/pure_kernel_level_thread.png)

- Kernel Thread 1개가 User Thread 1개를 처리하는 구조
- 병렬 처리 가능
- 특정 스레드의 상태가 다른 스레드에게 영향을 미치지 않음
- 즉, ULT를 생성할 때 KLT도 생성하기 때문에 성능 저하 발생 가능성 있음
<br>

## Combined (Many-to-Many)

![](/operating-system/_img/combined_thread.png)

- ULT 보다 작거나 같은 수의 KLT가 결합된 구조
- One-to-One 구조만큼 완벽한 병렬 처리는 아니지만 병렬 처리 가능
- 특정 스레드가 Block 되어도 전체 Process가 Block 될 가능성이 낮음