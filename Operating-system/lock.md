나의 모든 통장 잔고를 출금하려고 하는 순간 카드사에서 카드 결제를 위해 모든 잔고를 가져간다면 나는 잔고를 출금할 수 없는 상태가 된다. 이런 상황은 병행 프로그래밍 (concurrent programming)에서 발생하는 문제 중 하나로 **멀티 스레드 환경에서 Interrupt가 발생할 경우 명령어를 원자적으로 실행시키는데 어려움**이 있다. 여기서 원자적인 실행이란 어떤 동작의 묶음이 시작부터 성공적인 종료까지 보장하는 것을 의미한다.

위와 같은 문제를 해결하려면 내가 통장 잔고를 출금할 때까지 카드사가 나의 통장 잔고에 접근하는 것을 일시적으로 막으면 된다. 이 상황을 OS 로 설명하자면 다음과 같다.

- Critical section (임계영역) : 잔고에 접근해 출금까지를 처리하는 영역으로 내가 접속해있을 때 다른 누군가의 접근을 막아야 하는 영역
- Critical resource (임계자원) : 서로 접근하려고 하는 대상인 잔고
- Interrupt : 내가 잔고를 처리하는 중인데 카드사가 나의 잔고에 접근하려고 하는 상태
- Lock : 다른 누군가의 접근을 일시적으로 막는 것
- Mutual exclusion (상호배제) : 단 한명만 접근하는 것을 보장해주는 것
<br>

### Critical resource 임계 자원

2개 이상의 스레드가 동시에 사용할 수 없는 자원을 Critical resource(임계 자원)이라고 한다. 

같은 프로세스내 여러 스레드는 프로세스의 Stack을 제외한 Code, Data 그리고 Heap 영역을 공유한다. 그러므로 Thread를 기준으로 Stack에 할당된 지역 변수는 임계 자원이 아니고 **전역 변수와 동적인 객체들이 임계 자원**이다.
<br>

### Critical section 임계 영역

**임계 자원에 접근**하는 코드의 일부 영역으로 특정 스레드가 해당 영역을 수행하는 동안 다른 스레드가 수행하지 않아야 하는 코드 영역이다.
<br>

### Mutual exclusion 상호 배제

한 시점에 단 1개의 스레드만 임계 영역에 들어갈 수 있음을 보장하는 것을 상호 배제라고 한다. 일반적으로 상호 배제는 개발자가 직접 작성하며 상호 배제의 구체적인 내용은 프로그램마다 다르다.

상호 배제의 요구 조건은 다음과 같다.

- 특정 시점에 반드시 단 1개의 스레드만 임계 영역에 진입할 수 있음을 보장 (Mutual Exclusion)
- 다른 스레드는 임계 영역을 사용 중인 특정 스레드를 간섭하면 안됨
- 임계 영역에 접근하고자 하는 스레드의 수행이 무한히 미뤄지면 안됨
- 임계 영역을 수행하는 스레드가 없을 때 임계 영역에 진입하고자 하는 스레드를 바로 처리
- 임계 영역을 수행하는 스레드는 일정 시간 내에 임계 영역에서 나와야 함
<br>

### Race condition 경쟁 조건

2개 이상의 스레드가 공유 데이터에 접근하려는 상황에서 어느 스레드가 먼저 수행되냐에 따라 최종 결과가 달라지는 것을 의미한다. Mutual exclusion, Deadlock 그리고 Starvation 문제를 해결해야 한다.
<br>

### Deadlock 교착 상태

다른 스레드가 서로 다른 일을 처리해줄 것이라고 의존하며 기다리는 상태로 모든 스레드가 이런 상태여서 더이상 진행되지 않는 상태를 말한다. 
더 쉽게 설명하자면 여러 트랜잭션이 임계 자원에 대해 Shared-Lock을 획득한 후 수정 작업을 위해 Exclusive-Lock 획득을 시도한다. 하지만 Exclusive-Lock을 획득하기 위해선 해당 객체에 어떠한 락도 걸려있으면 안되는데, 모든 트랜잭션이 해당 객체에 대해 Shared-Lock을 가지고 있으므로 모든 트랜잭션은 Exclusive-Lock을 획득하지 못하고 계속 대기하는 상태가 된다. 이 상태를 교착 상태라고 하며 일정 시간이 지나면 시스템에서 특정 트랜잭션을 Abort 시켜 교착 상태에서 빠져나오게 한다.
<br>

### Livelock 라이브락

2개 이상의 스레드가 다른 스레드의 상태 변화에 따라 자신의 상태를 변화시키는 작업만 수행하고 있는 상태이다. 작업은 하나 수행되는 작업이 유용한 것이 아니고 반복적인 상태 변화일 뿐이다.
<br>

# Lock

Critical section의 동시 접근을 해결하는 방법에는 Lock, Semaphore, monitor, message passing이 있다. 여러 방법 중 Lock은 말 그대로 임계 영역을 사용하고 있다면 Lock을 걸어 다른 누군가가 못들어오게 막는 방법이다.

```c
lock_t mutex;

lock(&mutex);

// critical section 시작
balance = balance + 1;
// critical section 종료

unlock(&mutex);
```

임계 영역인 ```balance = balance + 1;``` 에 상호 배제를 적용하기 위해 Lock 변수인 mutex를 사용한다.

다른 스레드가 Lock을 가지고 있지 않다면 임계 영역에 접근할 수 있는 상태이므로 ```lock(&mutex);``` 코드로 Lock을 갖고 임계 영역에 들어간다. 임계 영역에서 모든 처리를 다 끝났다면 다른 스레드가 접근할 수 있도록 ```unlock(&mutex);``` 코드로 Lock을 해제한다.

만약 thread_A가 Lock을 걸고 임계 영역에 들어갔다. 이때 thread_B가 임계 영역에 접근을 시도하지만, **이미 다른 스레드가 Lock을 획득한 상태이므로 임계 영역에 들어갈 수 없다**.

위 코드를 POSIX 라이브러리로 바꾸면 다음과 같다.

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // default 설정

pthread_mutex_lock(&lock);

// critical section 시작
balance = balance + 1;
// critical section 종료

pthread_mutex_unlock(&lock);
```
<br>


## 문제점

```cpp
typedef struct __lock_t {
	int flag = 0;
} lock_t;

void lock(lock_t *mutex) {
	while(mutex -> flag);  // spinlock
    mutex -> flag = 1;
}

void unlock(lock_t *mutex) {
	mutex -> flag = 0;
}
```
아무도 Lock 을 걸지 않았다면 thread_A가 Lock 을 걸기 위해 ```lock()```을 호출한다. 이때 ```flag == 0``` 이므로 while 문을 탈출해 ```flag = 1``` 로 설정한다.

그 후 thread_B 가 Lock 을 걸기 위해 lock method를 호출하지만 ```flag == 1``` 이므로 무한루프를 돈다. thread_A가 Lock을 풀기 위해 unlock() 을 호출해 ```flag == 0```으로 만들었을 때 thread_B는 무한루프(Spinlock)에서 빠져나올 수 있다. 
<br>

하지만 위 과정은 **Interrupt 발생없이 원자적으로 처리될 때만** 제대로 Lock이 걸린다. 만약 thread_A가 while문을 탈출했을 때 Interrupt 가 발생하면 어떻게 될까?

![](/_img/Operating-system/interrupt_occurs.png)

아무도 Lock을 걸지 않았기 때문에 thread_A는 while문을 탈출한다. 하지만 이때 Context switching되어 thread_B에게 CPU 권한이 넘어간다. thread_A는 ```flag = true``` 로 설정하지 못한채 Context switching 되었기 때문에 ```flag == false``` 이므로 thread_B는 while문을 탈출하고 ```flag = true```로 설정한다.

thread_B가 ```flag = true``` 로 설정하니 다시 Context switching되어 thread_A에게 CPU 권한이 넘어갔다. thread_A는 while문을 이미 탈출하고 ```flag = true``` 로 설정할 순서이므로 ```flag == true```로 설정한다.

thread_A가 **lock 을 거는 중간에 Interrupt가 발생**했기 때문에 둘 다 Lock을 걸고 임계 영역에 들어갈 수 있는 상태가 되었다. 이는 임계 영역에는 오직 1개의 스레드만 들어갈 수 있다는 Lock의 요구 조건에 어긋난다.

이처럼 Interrupt는 언제든 발생할 수 있기 때문에 위와 같은 상황을 고려해야 한다. 요구 조건을 지키며 lock 을 만드는 방법은 ```인터럽트를 제어하는 방법```, ```소프트웨어적 방법론으로만 제작하는 방법(Software-only algorithm)```, ```하드웨어의 지원을 받아 제작하는 방법(Hardware atomic instructions)``` 이 있다.

<br>

# Interrupt 제어하기

병행 프로그래밍에서 발생하는 문제 중 하나는 **Interrupt 발생시 명령어를 원자적으로 실행시키는 것이 어렵다**는 점이다. 이를 해결하기 위해 **Lock을 갖으면 Interrupt를 비활성화한다**. Interrupt가 발생하지 않는 동안 임계영역을 수행하는 프로세스의 **지속적인 수행을 보장할 수 있기 때문**이다. 

### 문제점

Interrupt를 비활성화하는 방법은 **스레드가 Interrupt 활성 및 비활성을 제어하므로** Kernel에서 Interrupt를 제어할 수 있는 Interface를 제공해야 한다. Interrupt 제어 권한을 주려면 신뢰성이 필요하다. 하지만 Thread는 프로그램의 실행 흐름일 뿐이고 **Lock을 악의적으로 호출해 독점할 가능성이 있기 때문**에 Interrupt 제어 권한을 줄 만큼 신뢰성이 높지 않다.
 
또한, Interrupt를 비활성화 한다는 자체가 큰 문제가 된다. 외부 이벤트에 대한 처리와 다른 프로세스에 대한 스케줄링 등 모든 기능이 중지되므로 프로세스의 계속적인 수행을 보장할 수 없어 시스템의 수행 효율이 감소한다.
<br>

# Software-only algorithm

## Failed Attempt 1: Busy waiting

가게가 1개의 팻말을 뒤집어가며 ```open``` 과 ```closed``` 상태를 나타내는 원리를 이용한다. Flag 변수가 1개의 팻말이고 ```thread_A는 flag == 0 일 때```, ```thread_B는 flag == 1 일 때``` 임계 영역에 들어갈 수 있다고 가정한다. 

```cpp
// thread_A : (i == 0), thread_B : (i == 1)
void lock(int i) {
	while(flag != i);
    // critical_section !!
    int other = 1 - j;
    flag = other;
}
```
thread_A는 ```flag == 0```이 될 때까지 무한루프를 돌다가 ```flag == 0``` 이 되는 순간 임계 영역으로 들어간다. 임계 영역에서의 작업이 끝나면 thread_B가 임계 영역에 들어갈 수 있도록 ```flag = 1```로 설정한다.

thread_B도 마찬가지로 ```flag == 1``` 이 될 때까지 무한루프를 돌다가 ```flag == 1``` 이 되는 순간 임계 영역으로 들어간다. 임계 영역에서의 작업이 끝나면 thread_A가 임계 영역에 들어갈 수 있도록 ```flag = 0``` 으로 설정한다.

이렇게 flag 변수로 해당 Thread가 임계 영역에 들어가고 **나왔을 때 다른 Thread에게 임계 영역에 접근하는 순서를 넘겨주는** 로직으로 상호 배제를 구현할 수 있을 것 같지만 결론은 그렇지 않다.

### 문제점

- 임계 영역에 들어간 thread_A가 일정 시간 이후 임계 영역을 나와 thread_B에게 순서를 넘겨주기위해 ```flag = 1``` 로 설정했다. 
- ```flag == 1``` 이므로 **thread_B만 임계 영역에 접근**할 수 있지만, thread_B가 임계 영역에 접근할 필요가 없다면 ```flag == 1``` 인 상태가 유지된다.

thread_B가 임계 영역에 들어갔다가 나와 ```flag = 0```으로 설정해야 thread_A가 다시 임계 영역에 접근할 수 있다. 하지만 thread_B가 임계 영역에 접근할 필요가 없어 ```flag == 1``` 인 상태를 유지한다면 thread_A는 더이상 임계 영역에 들어갈 수 없다. 

이 방식을 여러 Thread에 적용하면 결국 여러 스레드가 무한 대기 상태에 빠지는 Deadlock 문제가 발생한다. 또한 thread_A는 자신이 Lock을 갖을 수 있을 때까지 while문을 도는데 이를 **Spin-lock**이라고 하며 성능상 좋지 않다. 자신이 Lock을 얻을 때까지 while문을 돌며 최대한 다른 Thread에게 CPU를 양보하지 않으려 하기 때문에 Busy-waiting이라고도 한다.
<br>

## Failed Attempt 2: Busy waiting

Flag 변수로 실패했으니 Flag 배열을 사용해 ```flag[0] == true 이면 thread_A 가```, ```flag[1] == true 이면 thread_B 가``` **임계 영역에 들어가 있음**을 나타낸다.

다른 의미로 ```thread_A 는 flag[1] == false 일 때```, ```thread_B 는 flag[0] == false 일 때``` 임계 영역에 들어갈 수 있다.

```cpp
// thread_A : (i == 0), thread_B : (i == 1)
void lock(int i) {
	int other = 1 - j;
    while(flag[other]);  // 상대방이 나올때까지 무한루프
    
    // 상대방이 임계영역에서 나옴 == 내가 임계영역에 들어갈 수 있는 상황
    flag[i] = true;
    // critical section !!
   	flag[i] = false;
}
```
- thread_A 기준으로 설명하자면 ```flag[1] == true```이면 thread_B가 임계 영역에 들어가 있으므로 ```flag[1] == false``` 가 될 때까지 무한루프를 돈다.
- ```flag[1] == false```가 되면 thread_B 가 임계 영역에서 나왔다는 뜻이므로 ```flag[0] = true```로 설정하고 자신이 임계 영역에 들어간다.
- thread_A가 임계 영역에서 나왔다면 자신이 임계 영역에서 들어가 있지 않음을 표시하기 위해 ```flag[0] = false```로 설정한다.

Flag 배열을 사용하기 때문에 Flag 변수를 사용했을 때 발생한 Deadlock이 발생하지 않는다. 하지만 완벽한 상호 배제를 구현할 수 없다.

### 문제점

![](/_img/Operating-system/failed_attempt2_interrupt.png)

- thread_A가 while문을 빠져나온 순간 스케줄링 되어 thread_B로 CPU 권한이 넘어갔다.
- thread_A는 ```flag[0]``` 을 True로 설정하지 못한 상태에서 Context switching이 이루어졌기 때문에 thread_B는 while문을 탈출해 자신이 임계 영역에게 들어가도록 ```flag[1] = true```로 설정한다.
- 설정 후 thread_A으로 다시 스케줄링 된다면 thread_A는 ```flag[0] = true``` 로 설정할 단계이므로 True로 설정한다.

위 과정은 Interrupt가 발생하여 원자적으로 수행하지 못해 두 스레드가 임계 영역에 들어갈 수 있는 상태가 되었다. 결론은 Flag 배열로도 상호 배제를 완벽히 구현하지 못한다.
<br>

## Peterson's algorithm

```cpp
bool flag[2] = {false, false};
int turn;

// thread_A : (i == 0), thread_B : (i == 1)
void lock(int i) {
	
	flag[i] = true; // i 가 임계 영역에 들어가고 싶다는 신호를 표시
    
    // 다른 스레드인 j 도 임계 영역에 들어가고 싶은 상태라면
    // j 가 먼저 임계영역에 들어가도록 양보한다.
	int other = 1 - i;
  	turn = other;
    
    // 다른 스레드가 임계 영역에 들어가고 싶은 상태이고
    // 다른 스레드가 임계 영역에 들어갈 순서인 경우 while 문을 돈다.
    while(flag[other] && turn == other);
    
    // critical section !!
    
    flag[i] = false; // 임계 영역에서 나왔기 때문에 false 로 바꾼다. 
}
```
<br>

### 임계 영역 접근

![](/_img/Operating-system/peterson1.png)

```cpp
void lock(int i) {
	
	flag[i] = true; // i 가 임계 영역에 들어가고 싶다는 신호를 표시
    
    // 다른 스레드인 j 도 임계 영역에 들어가고 싶은 상태라면
    // j 가 먼저 임계영역에 들어가도록 양보한다.
	int other = 1 - i;
  	turn = other;
```

thread_A와 thread_B가 lock method를 호출했지만 thread_A의 처리 속도가 조금 더 빨랐다고 가정한다. 그럴경우 thread_A가 먼저 **turn 변수를 other인 B로 설정**하고, 그다음 thread_B가 **turn 변수를 B에서 A로 변경**한다.

![](/_img/Operating-system/peterson2.png)

turn 변수가 A로 변경되었기 때문에 thread_A는 while문을 탈출하고 임계영역에 들어간다. 반면에 thread_B는 thread_A가 더이상 임계 영역에 있는 것을 원하지 않아 ```flag[A] == false``` 가 되거나 ```turn == B```로 자신의 순서가 될때 while문을 탈출할 수 있다.

thread_A가 임계 영역에서 나오면 더이상 임계 영역에 들어갈 필요가 없다고 ```flag[A] = false```로 설정해주면 thread_B는 while 문을 탈출하고 임계 영역에 들어간다.
<br>

### 무한 대기 방지

1개의 스레드만 Lock을 걸었을 때 무한정 대기가 발생하는지 확인한다. 

![](/_img/Operating-system/peterson3.png)

thread_A가 Lock을 걸기 위해 lock method를 호출해 ```turn = B``` 으로 설정했다. 하지만 thread_B는 Lock을 걸지 않았기 때문에 ```flag[B] == false``` 이다. 즉, thread_A는 자신의 Turn이 아니어도 while문을 탈출해 임계영역에 들어간다.

즉, Peterson's algorithm으로 임계 영역에 단 1개의 Thread만 들어가는 것을 보장하고 임계 영역에 들어가고 싶은 Thread가 무한 대기하지 않음을 보장한다.

<br>

# Hardware atomic instructions

Failed attempt 2에서 Lock을 거는 도중 Interrupt가 걸리면 완벽한 상호 배제를 구현하지 못한다. 즉, Lock을 획득하는 과정이 원자적으로 실행된다면 해결가능하다.

## Test-And-Set

Test-And-Set 명령어는 Hardware API를 이용한다. 즉, Hardware에서 원자적으로 처리되기 때문에 수행 도중 Interrupt를 당하지 않는다.

```cpp
int TestAndSet(int *ptr) {
	int old = *ptr;
    *ptr = 1;
    return old;
}
```
ptr이 가리키는 과거 값 old를 반환하고 ptr은 새로운 값으로 Update하는 로직이다. 

```cpp
typedef struct __lock_t { int flag; } lock_t;

void lock(lock_t *lock) {
	// TestAndSet 이 0 을 반환할 때까지 무한 루프
	while(TestAndSet(&lock -> flag) == 1);
    
    // lock 을 얻는다.
    lock -> flag = 1;
}

void unlock(lock_t *lock) {
	lock -> flag = 0;
}
```
Lock을 얻기 위해 ```lock()```을 호출하고 ```TestAndSet(&lock -> flag, 1);``` 이 **0을 반환**할 때 while문을 탈출한다. ```TestAndSet()```가 0 반환했다는 것은 다른 Thread가 ```unlock()```을 호출해 **flag가 0이 되었다**는 뜻이다. 즉, 0을 반환하면 while문을 탈출하고 **flag를 1로 설정**해 Lock을 갖는다.

처음 Lock을 거는 Thread는 ```flag == 0```으로 초기 설정되어있기 때문에 while문을 바로 탈출한다. 하지만 이후에 Lock 거는 모든 Thread는 ```flag == 1``` 인 상태로 들어오기 때문에 while문을 탈출하지 못한다. 임계 영역에 있던 Thread가 unlock method로 ```flag = 0```으로 설정하면 while문을 도는 Thread 중 1개가 임계 영역으로 들어간다. 

TestAndSet 명령어는 Hardware에서 원자적으로 처리되기 때문에 중간에 Interrupt가 걸리는 문제를 신경쓰지 않아도 된다. 하지만 unlock method가 호출되어 ```flag == 0```으로 되는 순간 while문을 수행중인 여러 스레드 중 어떤 스레드를 선택해야 되는지에 대한 기준이 없다면 기아 상태가 발생할 수 있다.
<br>

## Compare-And-Swap

```c
int CompareAndSwap(int *ptr, int expected, int new) {
	int original = *ptr;
    if(original == expected)
    	*ptr = new;
    return original;
}

void lock(lock_t *lock) {
	while(CompareAndSwap(&lock -> flag, 0, 1) == 1);
    // critical section
}
```
기대하는 값과 기존 값을 비교하는 방식으로 동작한다. 기대하는 값과 기존 값이 같다면 새로운 값으로 갱신한다.

- Lock이 걸려있지 않다면 new 값으로 갱신하고 이전 값인 0을 반환한다. 
- 반대로 Lock이 걸려있다면 기대하는 값과 다르므로 갱신하지 않고 1을 반환한다.

Hardware가 지원하는 Test-And-Set 과 Compare-And-Swap 으로 Lock을 구현할 수 있다. 하지만 두 방식 모두 **Spin-lock**으로 Lock을 얻을 때까지 무한루프를 수행해 CPU가 낭비된다.
<br>

## Bounded waiting

while문을 돌며 대기중인 Thread가 기아상태가 되는것을 막기위해 적절한 순서를 정한다.

```c
do {
	waiting[i] = true;  // flag
    key = true;         // turn
    while(waiting[i] && key)
    	key = test_and_set(&lock);
        
    waiting[i] = false;
    
    // !! critical section !!
    
    // 다음 순서 지정
    j = (i + 1) % n;
    while(j != i && !waiting[i]) 
    	j = (j + 1) % n;
        
    if(j == i)	// 대기 중인 스레드 없음
    	lock = false;
    else 
    	waiting[j] = false;
} while(true);
```
현재 Lock 상태를 읽고 쓰는 것을 원자적으로 실행하기 위해 test-and-set 명령어를 사용한다. 누군가 ```unlock()```을 호출해 Lock이 false가 되었다면 while문을 탈출하고 더이상 기다리는 상태가 아님을 표시하기 위해 ```waiting[i] = false``` 로 설정 후 임계 영역에 들어간다. 

임계 영역을 나왔다면 대기 중인 Thread 중 임계 영역에 들어갈 다음 순서를 정한다.

- ```i == j``` 라면 임계 영역에 들어가고자 하는 Thread가 없다는 뜻이므로 Lock을 해제한다. 
- 하지만 임계 영역에 들어갈 Thread가 존재한다면 ```waiting[j] = false```로 설정한다. 

임계 영역에 들어갈 새로운 Thread(j)가 존재하면 현재 j는 ```waiting[j] == true``` 이기 때문에 ```while(waiting[i] && key)``` 을 수행중이다. 여기서 탈출시키고 임계 영역에 진입시키기 위해 ```waiting[j] = false```로 설정한다.

또한, 계속해서 Lock을 사용하지 때문에 Lock을 해제하지 않는다.

<br>

## Reference

- https://jhnyang.tistory.com/36
- https://jhnyang.tistory.com/37
- https://blackinkgj.github.io/Locks/
- https://icksw.tistory.com/158