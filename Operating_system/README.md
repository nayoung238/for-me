## OS가 뭔데?

- 다양한 Applicattion의 실행을 위해 **컴퓨터를 실행**하고 **Hardware를 관리**하는 소프트웨어가 Operating system 
- Computer Hardware를 Software로 abstract(추상화)하여 관리
- ```CPU```를 ```Process```로 관리
- ```RAM```을 ```Memeory management```로 관리
- ```Disk```를 ```File System```으로 관리
<br>

# 메모리 영역

![png](/Operating_system/_img/memory_structure.png)

- Code: 프로그램 실행 코드로 메모리에 반드시 존재, 컴파일 시 크기가 결정되고 중간에 코드를 바꿀 수 없게 READ-Only
- Data: 전역변수, static 변수 같은 고정된 데이터가 할당되는 공간
  - 전역변수, static 값을 참조한 코드는 컴파일 후 Data 영역의 주소 값을 가르키도록 변경
  - 실행도중 전역변수의 값이 변경될 수 있기 때문에 Read-Write
  - 초기화되지 않는 전역 변수는 BSS(Block Started by Symbol)에 할당
- Stack: 함수내의 지역변수, 매개변수, 리턴값 등이 저장되고 함수가 종료되면 해당 데이터도 제거
  -LIFO 방식을 따름
  - Heap 영역이 있는 방향으로 추가 할당
  - stack이 Heap 영역 침범시 Stack overflow
- Heap: 런타임에 결정되는 영역으로 malloc(cpp), new 객체(java) 와 같이 동적할당으로 객체를 생성
  - Stack 영역이 있는 방향으로 추가 할당
  - Heap이 Stack 영역 침범시 Heap overflow
  - 포인터로 메모리 영역을 접근하기 때문에 다른 자료구조에 비해 데이터 읽고 쓰는게 느림
<br>

- Code와 Data 영역은 Compile 시 메모리 할당되어 크기가 고정
- Heap과 Stack 영역은 Runtime 시 메모리가 할당되어 가변적
<br>

## 메모리 고정 분할

- Internal fragmentation 발생
- 절대 번역: 고정된 크기에 따른 여러 큐 존재 
  - 특정 큐에는 계속 프로세스가 들어와 대기하고, 다른 큐에는 들어오지 않는 경우 효율성 떨어짐
- 재배치: 큐 1개 존재
  - 비어있는 메모리를 계속 체크하며 대기 중인 프로세스 중 1개를 선택
  - 절대 번역의 문제를 해결할 수 있지만 여전히 Internal fragmentation 발생
<br>

## 메모리 가변 분할

- External fragmentation 발생
- 특정 프로세스가 끝나면 그만큼의 공간이 생김
- 생긴 공간보다 작거나 같은 크기를 가진 프로세스가 들어가면 됨
- 생긴 공간보다 작은 크기의 프로세스가 들어갈 경우 공간이 남는 External fragmentation 발생
- 이를 Compaction(메모리를 쭉 올림) 또는 Coalescing(생긴 공간의 인접한 것만 붙임) 으로 해결
<br>

## 재배치

- 비어있는 공간 중 어디로 들어갈 것인가?
- First Fit: 비어있는 위쪽부터
- Best Fit: Size가 가장 잘 맞는 곳부터
- Worst Fit: 비어있는 공간이 가장 큰 곳부터
- Next Fit: 현재 Pointer의 밑에 비어있는 곳부
<br>

# PCB (Process Control Block)

- 프로세스의 정보를 저장한 struct형 자료구조
- 수행 중 Interrupt가 발생해도 프로그램 수행이 제대로 재개할 수 있도록 충분한 정보를 유지
- PID, state, nice(우선순위), PC(Program counter), Memory pointer, HW Context data 등으로 구성
- PC(program counter)는 프로세스 실행을 위한 다음 명령의 주소를 표시
- nice(우선순위)와 PC(program counter)는 ```schedule()``` 에서 사용됨
- Memory pointer는 메모리 영역에서 struct된 Memory(code, data, stack)을 가리키고 있음
- HW context data는 현재 프로세스가 어디까지 진행했는지의 정보로 Context switching시 HW context data를 register에 올리고 내림
<br>

## Context

- HW(CPU) context: register 정보들
- Memory context: Memory Pointer가 가리키는 Struct
- System context: HW와 Memory 문맥을 제외한 모든 것
<br>

## Context Switching

- 어떤 프로세스가 수행 중 Interrupt가 발생했다면 Program counter와 Context data인 Register의 현재 값이 해당 프로세스의 PCB의 적절한 field에 저장 (Save the old context)
- 저장 이후 프로세스의 상태를 Block 또는 Ready로 변경
  - ```exit()``` 호출에 의해 Context switching 되었다면 해당 Process는 **Zombie 상태**
  - ```I/O()``` 호출에 의해 Context switching 되었다면 해당 Process는 **Block(Sleep) 상태**
  - ```TQ(Time Quantum)``` 에 의해 Context switching 되었다면 해당 Process는 Ready 상태 (memory가 부족하면 Swap 상태 = suspend 상태)
- OS는 다른 프로세스를 선택해 수행 상태로 변경하고 해당 프로세스의 PC, Context data를 처리기 Register에 적재하여 새로운 프로세스를 수행시킴 (Execution of the New Context)
<br>

# Process state

![png](/Operating_system/_img/process_state_comment.jpg)

## Swap in? out?

- Swapping: **Process 전체**를 주기억장치로부터 디스크로 옮기는 것
- Swap out: 종료되지 않은 프로세스 전체를 Swap 영역으로 이동
- Swap in: Disk에 있는 프로세스 전체를 Memory에 load
<br>

## Page out

- Page out: **Process의 일부**인 Page Frame의 내용을 Swap 영역으로 방출
- 해당 Page frame에 현재 필요로하는 프로그램에게 할당
<br>

## Page fault

- 프로그램에 접근하고자 하는 PageFrame이 메모리에 없는 경우 Swap 영역에서 찾음
- free page(page frame)에 실행해야하는 부분을 올리며 PTE(Page table entry)가 만들어진다.
<br>

## Kernel

- 기본적으로 메모리에 상주하는 Core가 되는 Software
- Hardware를 Software적으로 Abstraction 하여 개발자가 programming 또는 HW에 쉽게 접근하도록 도와줌
<br>

## Kernel mode vs User mode

![png](/Operating_system/_img/modes_of_execution(1).png)

![png](/Operating_system/_img/modes_of_execution(2).png)

- 사용자 프로그램의 간섭으로부터 OS 및 PCB와 같은 주요 운영체제 테이블을 보호할 필요가 있고 이를 위해 2가지 모드로 나뉨
- User mode: 사용자 Application Program이 대부분 수행되는 모드
  - mode bit = 1
- Kernel mode: 프로세스가 수행되다가 Interrupt가 걸려 OS가 호출되어 수행되는 mode
  - OS는 시스템의 모든 리소스에 엑세스 가능
  - kernel mode에서 수행되는 Software는 처리기와 모든 명령, register 그리고 메모리 제어가 가능
  - mode bit = 0
<br>

- User mode -> Kernel mode
  - 사용자가 System call
  - Interrupt가 OS kernel을 수행을 촉발(trigger)시킬 때
- Kernel mode -> User mode
  - 제어가 OS 서비스에서 사용자 프로세스로 복귀할 때
<br>

## Interrupt & Trap

- Interrupt: 현재 수행중인 프로세스와는 독립적으로 외부에서 유발되는 여러 종류의 사건(IO 발생)에 의해 발생
- Trap: 불법적인 파일 접근 시도와 같이 현재 수행되고 있는 프로세스에서 생성되는 오류나 예외 조건 때문에 발생 (Kernel 내부에서 자체적으로 call)
<br>

- HW Interrupt
  - HW Clock Interrupt(시간 할당량을 모두 사용했는지 여부 결정), HW IO Interrupt
  - 현재 명령어 수행의 외부에서 발생
  - 비동기적인 외부 사건에 반응
- SW Interrupt
  - Trap: 현재 명령어 수행과 관련되며 오류나 예외 상황 처리하기 위해 Kernel 함수 수행
    - page fault, 0으로 나누기 -> 비동기적
  - System call(Supervisor 호출): 사용자가 명시적으로 ```open()```, ```read()```, ```write()```, ```fork()``` 등을 요청
    - OS 기능에 대한 호출
<br>

# Scheduling

- 다음에 실행할 프로세스와 대기해야할 프로세스를 결정하며 이로인해 시스템의 전체 성능에 영향을 미침
- 대기 중인 프로세스들에 대해 대기 시간을 줄이는 것이 목표
- ```Preemption``` : OS가 CPU의 사용권을 선점할 수 있는 경우로 강제 회수가 가능(다른 프로세스에게 양도 가능) -> 처리시간 예측 불가능
- ```Non-preemption``` : 프로세스 종료 및 IO 등의 event가 있을 때까지 실행 보장(다른 프로세스에게 양도 안함) -> 처리시간 예측 가능
- Turn around time(반환 시간) = completion(작업 완료 시간) - arrival(작업 도착 시간)
- Turn around time은 수행시간이 아님
<br>

- 장기(long-term) 스케줄링: 프로세스가 CPU에 의해 실행될 수 있는지 자격을 부여하는 결정
- 중기(medium-term) 스케줄링: 프로세스 전부 혹은 일부가 main memory에 올라갈 수 있는지 자격을 부여하는 결정
- 단기(short-term) 스케줄링: CPU에 의해 실행된 다음 번 프로세스로 어떤 프로세스를 선택할 것인지 결정
  - FIFO, SJF, RR, HRRN, MLFQ 의 scheduling이 단기 스케줄링
<br>

## FIFO 

- First In First Out으로 FCFS(First Come First Service)라고도 함
- Non-preemption
- Convoy effect(호위병 효과): 바로 다음에 들어온 프로세스는 0.001S 면 CPU 사용이 끝나지만 앞에 있는 프로세스의 사용이 끝나야만 처리가능 -> 비효율적
- 즉, 실행시간이 짧은 프로세스가 뒤에 있을수록 평균 대기 시간이 길어지는 단점
<br>

## SJF

- Shortest Job First로 SPN(Shortest Process Next)라고도 함
- Non-preemption
- 프로세스의 CPU 사용이 끝나면 대기중인 프로세스 중 service 시간이 제일 짧은 프로세스를 선택
- FIFO에서 발생한 Convey effect을 해결할 수 있음
- 먼저 들어온 긴 작업의 프로세스는 계속해서 뒤로 밀리기 떄문에 Starvation 현상 발생
<br>

## STCF

- Shortest Time to Completion First로 SRT(Shortest Remaining Time)라고도 함
- Preemption
- 실행 중인 프로세스의 남은 Service 시간과 대기 중인 프로세스의 Service 시간을 비교해 CPU 사용 권한을 계속 변경
- 즉, 남은 Service 시간이 긴 프로세스는 계속 뒤로 밀려 Starvation 현상 발생
<br>

## HRRN

- Highset Response Ratio Next
- (Time spent waiting + expected service time) / expected service time
- Non-preemption
- SJE의 starvation도 해결하고 기다린 시간도 보장해줌
<br>

## RR

![png](/Operating_system/_img/round_robin.png)

- Round Robin
- Preemption
- 실제 Scheduler 기반
- Time slicing = Scheduling Quantum = Time Quantum
- Time Slicing의 길이 중요
  - 너무 길면 Non-preemption
  - 너무 짧으면 Schduling하는 시간이 더 소요될 수 있음
<br>

# Abstraction & Virtualization

- Computer Hardware(CPU, RAM, Disk)를 Software로 **abstract**하여 ```Process```, ```Memeory management``` 그리고 ```File System```으로 관리
- ```CPU```와 ```RAM```에 Virtualization(가상화)를 적용해 사용자가 착각하도록 만듦 (1개가 여러개처럼 보이는 환상을 제공)
- ```CPU```를 추상화한 ```Process```들은 아주 빠르게 번갈아가면서 실행하는데 이는 사용자가 CPU가 여러개가 동시에 처리한다는 착각을 하게 만듦
- ```RAM```을 추상화한 ```Memeory management```에 대해 각각의 ```Process```가 자신만의 physical memory를 가지고 있다라는 환상을 제공
<br>

## Persistence

- ```Disk```는 데이터의 지속적이고 안전한 저장이 최우선이므로 가상화 개념을 도입하지 않음
- 전원이 꺼지거나 시스템이 손상되어도 데이터는 유지되어야 함
  - Hardware: 하드 드라이브, SSD(Solid-state Drive) 같은 IO 장치 필요
  - Software: 파일 시스템은 사용자가 생성한 모든 파일을 안전하게 저장
<br>

## CPU virtualization

- 여러 프로세스가 번갈아가며 CPU를 차지 (Interleaved execution)
- OS가 시스템에 대한 제어를 유지하면서 효율적인 방식으로 CPU를 가상화하는 것이 목표 -> HW와 OS의 지원이 필요
<br>

## Memory virtualization

- ```CPU가 32비트``` == ```Register의 크기가 32비트```
- 이는 총 2^32개 즉, 4GB의 가상 주소 공간을 갖는다는 의미이다.
- 현대 OS에선 2^48 까지 사용
<br>

## 주소 공간

- 가상 주소 공간: Register 크기에 비례
- 실제 주소 공간(물리적): Memory에서 줄 수 있는 모든 주소 공간으로 Page frame 개수
- 주소 공간은 실행 프로그램의 모든 메모리 상태를 포함
<br>

## 가상 메모리 vs 가상 메모리 주소 공간

- 가상 메모리 = 실제 메모리 + Swap 영역
  - Swap 영역은 Disk에 존재 (속도 느림을 해결하기 위해 Linked list로 구현)
- 가상 메모리 주소 공간은 register 크기에 따라 Process가 주소를 처리할 수 있는 영역의 크기
- 가상 메모리 주소 공간은 register 크기에 비례하며 2^(register 크기) 값의 가상 주소 공간을 갖게 됨
<br>

- Process 전체가 Swap 영역에 올라간 경우 -> 해당 프로그램이 실행될 수 없는 상태
- Process 일부만 Swap 영역에 있는 경우 -> Memory에서 실행하기 위한 Page가 없을 경우 Swap 영역에서 찾는다.(page fault)
- P(present bit): 0(swap 영역에 존재) /  1(memory에 존재)
<br>

## 가상 주소 공간 왜쓰는데?

- 자원 낭비 최소화: 프로세스 전체를 올리지 않고 필요한 부분만 올렸다 내렸다하며 자원 낭비를 최소화
- 프로세스 보호: 가상 주소 공간을 통해 Physical memory로 mapping 되는데 프로세스 간의 침범 방지
  - 만약 가상 주소 공간을 사용하지 않고 바로 Physical memory로 프로세스를 올리면 프로세스간의 경계 지점을 계속 확인해야 함
<br>

## Page? Page Frame?

- Page: Virtual address space를 일정한 크기(4KB)로 분할한 단위
- Page Frame: Main memory를 일정한 크기(4KB)로 분할한 단위
- Page와 Page Frame의 크기가 4KB로 같음 -> 서로 matching 해야하기 때문
<br>

## Page -> Page Frame

- 가상 주소 공간에 있는 특정 Page를 실행하고 싶다면 해당 Page에 있는 내용을 Main memory의 비어있는 Page frame에 올리며 이 방식을 Paging 기법이라 함
- Main memory에 존재하는 Page table에서 해당 Page가 어디있는지 파악
  - P(present bit) 가 1이면 해당 Page가 Memory에 있고
  - P(present bit) 가 0이면 Swap 영역에 존재있으니 Swap 영역에서 가져옴 (page fault)
- Page table에서 해당 Page의 가상주소를 찾고 MMU(memory management unit)의 도움을 받아 가상주소를 물리주소로 변경
- 물리주소로 Main memory에서 해당 Page의 내용이 있는 Page frame을 찾아 실행
<br>

## Program? Process?

- Disk에 존재하면 Program
- Memory에 load 되었다면 Process
<br>

# 메모리 관리

- 여러 프로세스를 수용하기 위해 주기억장치를 동적 분할하는 메모리 관리 작업이 필요하며 Paging 과 Segmentation 기법 존재
- 현재 Paging 과 Segmentation 기법을 합쳐서 사용
- External fragmentation: 프로세스의 크기와 고정된 사이즈 차이에서 생기는 낭비
- Internal fragmentation: 프로세스가 끝나고 생긴 빈 공간과 새로 들어갈 프로세스 사이즈 차이에서 생기는 낭비
<br>

## Paging

- Page(가상 주소 공간을 일정한 크기로 자른 단위)를 Page Frame(메모리를 일정한 크기로 자른 단위)에 올림
- Page와 Page frame의 크기는 4KB로 같아 External fragmentation 발생하지 않음
- 프로세스를 Page 단위로 분할해도 마지막 Page는 4KB를 채우지 못할 수 있으며 이때 4KB 미만의 Internal fragmentation 발생
<br>

- 프로세스 전체를 메모리에 올리지 않고 필요한 부분만 Page Frame에 올리므로 여러 프로세스를 적재할 수 있음
- 즉, 임의의 시점에 준비 상태인 프로세스가 많을 가능성이 높아져 CPU utilization 높아
<br>

## iOS와 Android 차이

- OS
  - iOS: BSD
  - Android: Linux
- Block의 default size
  - iOS: 16KB
  - Android: 4KB
<br>

- iOS는 block의 default size가 16KB
- 즉, Android 보다 더 많은 양을 읽어올 수 있어 사용자는 상대적으로 속도가 빠르다고 느낄 수 있음
- 하지만 Internal fragmentation 으로 Memory 낭비가 심함
<br>

## Page table

- 프로세스마다 Page table을 가지고 있음
- Page table은 메모리에 존재하며 Page table로 Page frame에 할당
- Page table로 각 Page가 어느 Page frame에 할당되었는지 체크 (Frame 번호를 가지고 있음)
- 그렇기 떄문에 한 프로세스의 여러 Page는 연속적인 Page frame에 할당될 필요 없음
- 하지만 한 프로세스의 Page table의 크기가 커서 여러 Page frame이 필요하다면 이는 연속적으로 할당됨
<br>

## Address translation

![png](/Operating_system/_img/address_translation.png)

- Logical address(논리 주소): 프로그램의 시작위치로부터 상대적인 값으로 MMU가 논리주소를 물리주소로 변환함 
  - MMU의 도움을 받아 주소를 변경하는 것을 Dynamic relocation 이라고 함
- 프로그램 안에서 각 논리주소는 Page 번호와 offset으로 구성
- Page 번호로 Page table의 해당 PTE 를 찾고
- PTE의 Page frame 번호와 offset으로 Main memory에 접근
- 가상주소를 물리주소로 바꿀때마다 위 과정이 일어나는데 OS가 처리하면 느리므로 MMU가 처리
<br>

## Offset

![png](/Operating_system/_img/offset.png)

- Page와 Page frame의 크기 offset에 비례
  - offset 12bit -> Page 크기 2^12 = 4096 = 4KB
  - 즉, Page 크기가 4KB이면 Offset은 12bit
<br>

## Dynamic relocation

- Base, Limit register 를 사용해 가상 주소를 물리 주소로 변환
- 물리 주소 = 가상 주소 + base 
- 이 방식은 메모리 중간에 빈공간이 생김 -> Segmentation 기법이 해결
<br>

## Segmentation

- 프로세스는 여러 Segment로 나뉨
- Segment는 연관된 기능을 수행하는 하나의 모듈 프로그램을 다루며 Code, Data, Stack 그리고 Heap 을 Segment라고 함
  - 정적 세그먼트: Code, Data(+BSS)는 컴파일 시 사이즈가 정해지며 변경이 불가능
  - 동적 세그먼트: Stack, Heap은 Runtime 과정에서 메모리 할당이 이루어짐
- 현재 사용하는 방식이 Paged Segmentation으로 먼저 Segmentation을 수행하고 각 Segment별로 Paging 수행
- 즉, Code, Data, Stack 그리고 Heap을 나누는 방식이 Segmentation 이므로 모두 크기가 다르고 모든 Segment가 메모리 내에서 Paging 방식으로 처리
<br>

- Segment는 의미 단위로 나눠지기 때문에 공유와 보안에 있어서 Paging 기법보다 훨씬 효과적
  - Protection: 각 Segment 별로 Protection bit가 존재해 의미 단위별로 Read/Write 권한 부여
  - Sharing: 의미 단위로 공유하는 것이 훨씬 효과적
- 가변 분할이므로 Internal fragmentation 발생하지 않음
- 가변 분할이므로 Swapping이 힘듦
- 프로세스마다 크기가 다르므로 Segment 크기도 달라 External fragmentation 발
- 평균 Segmentation 크기가 작을수록 External fragmentation 적음
<br>

## Address translation

- 왼쪽 n비트를 세그먼트 번호로, 나머지 오른쪽 비트를 Offset 으로 사용
- 왼쪽 비트로 세그먼트 번호를 추출해 Segment table에서 Segment의 물리 주소를 찾음
  - 세그먼트 번호로 Code, Data, Stack 그리고 Heap 을 구분
- 나머지 오른쪽 비트인 Offset으로 세그먼트의 길이와 비교
  - 만약 offset이 세그먼트의 길이보다 크거나 같다면 해당 주소는 유효하지 않음 -> Trap (addressing error, Segmentation fault)
- 접근하고자 하는 물리주소는 세그먼트의 시작물리주소와 offset의 합
- STBR(Segment Table Base Register) 와 STLR(Segment Table Length Register) 2개의 Register 사용
<br>

# Page Placement Policy

- Page fault: Data가 Main memory의 Page frame에 적재되지 않아 실행할 수 없음
- 해당 Data를 Main memory에 올리기 위해서 Free page frame이 필요
- 만약 Free page frame이 없다면 Page frame 중 1개를 Swap 영역으로 보내야함
- 이때 out 되는 Page를 Victim page라고 하며 수정되지 않은 Page를 내보내는 것이 유리
- 수정된 것을 내보낼 경우 하드디스크에서 수정과정을 거쳐야하기 때문
- 이러한 경우를 모두 고려하며 교체를 진행하며 주로 LRU(Least Recently Used) algorithm 사용
<br>

## 교체 정책

- Page fault 해결을 위해 현재 메모리에 적재되어있는 Page 중 어떤 것을 내릴지 결정
- 교체 정책은 교체되는 페이지가 가까운 미래에 참조될 가능성이 가장 적어야 함
- 지역성의 원리로 최근 참조와 가까운 미래의 참조 패턴이 높은 상관관계
  - 지역성: 기억 장치 내의 정보를 균일하게 접근하는 것이 아니라 한 순간에 특정부분을 집중적으로 참조하는 특성
  - 시간 지역성: 최근에 참조된 주소의 내용이 곧 다시 참조되는 특성
  - 공간 지역성: 실제 프로그램이 참조된 주소와 인접한 주소의 내용이 다시 참조되는 특성
- 대부분 교체 정책은 과거를 근거로 미래를 예측
  - LRU(Least Recently Used): 최근에 참조되지 않은 Page를 out
  - LFU(Least Frequently Used): 최근에 자주 참조되지 않은 Page를 out
  - Clock(NUR-not used recently): 최근에 참조 패턴, read, write가 없던 Page를 out
- Global 교체: 메모리에 적재된 모든 프로세스 page를 교체
- Local 교체: 각 프로세스가 자신에게 할당된 Frame 중 교체될 것을 선택
- 다중 프로그래밍의 경우 Main memory에 다양한 프로세스가 존재하므로 Global 교체가 더 효율적
<br>

## LRU

- Least Recently Used
- 가장 오랫동안 참조되지 않은 주기억장치 상의 Page 교체
- 지역성에 따라 오랫동안 참조되지 않은 페이지는 가까운 미래에 참조될 가능성이 적다고 판단
- 즉, 참조에 대한 기록을 해야하며 Overhead가 아주 큼
<br>

## FIFO

- Round-robin 처럼 메모리에 할당된 Frame을 순환버퍼처럼 다룸
- 메모리에 가장 오래 있었던 Page 교체
- 가장 오래있었지만 계속 이용되는 코드라면 문제가 됨
- FIFO abnormally: Frame을 늘린다해서 Fault가 줄어들지 않음
<br>

## Clock

![png](/Operating_system/_img/clock.png)

- Use bit 사용하며 해당 Page가 참조될 때마다 Use bit 1로 설정
- 1개의 pointer가 첫 교체 후보를 가르킴
- 한 페이지를 교체해야 할때 포인터가 가리키는 버퍼부터 시작해 Use bit 가 0인 frame을 찾음
- 찾는 과정에서 Use bit가 1이라면 0으로 바꿔주고 다음 frame으로 넘어감