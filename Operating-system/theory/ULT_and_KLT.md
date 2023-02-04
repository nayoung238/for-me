## ULT vs KLT

Thread는 **사용자 수준 스레드 (ULT: User Level Thread)** 와 **커널 수준 스레드 (KLT: Kernel Level Thread)** 2가지 유형을 나눠진다.
<br>

## Kernel Level Thread

순수한 KLT 구현에서는 Thread 관리와 관련된 모든 작업이 Kernel에서 이루어진다. 라이브러리에 대한 모든 코드와 자료구조가 Kernel 공간에 존재하기 때문에 라이브러리의 함수 호출을 System call 이라고 한다. **OS 에서 Kernel Thread는 스케줄링의 단위**이다.

### KLT 장점

- Kernel은 여러 처리기에게 같은 Process 내의 여러 Thread를 동시에 (정확하게는 빠르게 번갈아가며) 스케줄할 수 있다.
- **특정 Process의 Thread가 Block 되면 Kernel은 같은 Process의 다른 Thread를 교환 가능하다.**
- Kernel routine 자체가 Multi-Thread로 구성 가능하다.

### KLT 단점

- 같은 Process내의 Thread에서 다른 Thread로 제어를 넘길 때 Kernel 모드로 전환이 필요해 비용이 발생한다.

<br>

## User Level Thread

Application에서 Thread 라이브러리를 호출해 사용하는 Thread를 ULT 라고 한다. 대표적인 예로 P-thread (POSIX Threads), Java thread는 Thread 라이브러리에서 제공하는 함수를 호출해 사용한다.

ULT는 사용자 공간에 존재하므로 Thread와 관련된 일은 당연히 사용자 공간에서 처리된다. OS는 ULT를 자원할당의 관점에서 바라보기 때문에 해당 Thread가 몇개인지, 어떻게 돌아가는지 등에 대한 관련 정보를 파악하지 않는다. 즉, Kernel은 Thread의 존재를 알 수 없기 때문에 순수한 ULT 환경에서는 **프로세스가 스케줄링의 대상**이된다.

### ULT 장점

- Thread 관리를 위한 모든 자료구조가 사용자 주소공간에 존재하기 때문에 스케줄링이나 동기화를 위해 커널 모드의 전환이 요구되지 않아 오버헤드가 적다.
- 기반이 되는 OS 스케줄러에 의존하지 않고 Application에 맞게 스케줄링을 구성할 수 있다.
- Kernel에 독립적으로 스케줄링을 할 수 있어서 어떤 OS에도 적용 가능하다.

### ULT 단점

- 스레드가 아닌 프로세스 단위로 CPU를 할당하기 때문에 **특정 Thread가 Block 상태가 될 경우 같은 Process 내의 모든 Thread가 Block 상태**가 된다. 즉, 순수한 ULT 기반의 Multi-thread application은 다중 처리의 장점을 살릴 수 없다.
- Kernel은 한번에 1개의 처리기에 1개의 Process를 할당하므로 임의 시점에 단일 Process 내에서 1개의 Thread만 수행한다. 즉, Multi-thread를 사용해도 동시 처리라고 말할 뿐이지 사실상 한 부분씩 진행되기 때문에 특정 부분에서 Block 상태가 되면 모든 부분이 Block 되면서 Multi-processing 장점을 살릴 수 없다.

<br>

## ULT 와 KLT 의 결합

Thread 생성과 Application 내에서의 Thread 스케줄링 및 동기화를 사용자 공간에서 한다. 그 후 ULT들을 ULT 수보다 작거나 같은 KLT에 Mapping하여 사용한다. 현재 가장 많이 쓰이는 방식이다. 

Application의 여러 Thread가 다수의 처리에서 병렬 수행 가능하며 **블록형 시스템 호출시 전체 Process를 Block 상태로 만들지 않는다**. 대표적인 예로 Solaris, BSD 등이 있다.
<br>

ULT 와 KLT 의 결합에는 다음과 같은 여러 연관 관계가 존재한다.
<br>

## Pure user-level (Many-to-One)

![](/_img/Operating-system/pure_user_level_thread.png)

- ULT 간 스케줄링이 발생해도 Kernel과 관련없는 작업이므로 모드 전환이 일어나지 않는다.
- 특정 Thread가 Block 상태가 되면 전체 Process가 Block 되는 병목현상이 나타난다. 
- 한번에 1개의 Thread만 처리할 수 있어 병렬 처리가 불가능하다.
<br>

## Pure thread-level (One-to-One)

![](/_img/Operating-system/pure_kernel_level_thread.png)

- 병렬 처리가 가능하다.
- 특정 Thread의 상태가 다른 Thread에게 영향을 미치지 않는다.
- Kernel Thread 1개가 User Thread 1개를 처리하는 구조이다. 
- 즉, ULT를 생성할 때 KLT도 생성해야하며 과도한 생성시 성능 저하가 발생한다.
<br>

## Combined (Many-to-Many)

![](/_img/Operating-system/combined_thread.png)

- ULT 보다 작거나 같은 수의 KLT가 결합된 구조이다.
- One-to-One 구조만큼 완벽한 병렬 처리는 아니지만 병렬 처리가 가능하다.
- 특정 Thread가 Block 되어도 전체 Process가 Block 될 가능성이 낮다.