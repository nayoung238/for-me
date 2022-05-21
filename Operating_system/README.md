## OS가 뭔데?

- 다양한 Applicattion의 실행을 위해 **컴퓨터를 실행**하고 **Hardware를 관리**하는 소프트웨어가 Operating system 
- Computer Hardware를 Software로 abstract(추상화)하여 관리
- ```CPU```를 ```Process```로 관리
- ```RAM```을 ```Memeory management```로 관리
- ```Disk```를 ```File System```으로 관리
<br>

## Kernel

- 기본적으로 메모리에 상주하는 Core가 되는 Software
- Hardware를 Software적으로 Abstraction 하여 개발자가 programming 또는 HW에 쉽게 접근하도록 도와줌
<br>

## Abstraction? Virtualization?

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

## Program? Process?

- Disk에 존재하면 Program
- Memory에 load 되었다면 Process
<br>

## CPU virtualization

- 여러 프로세스가 번갈아가며 CPU를 차지 (Interleaved execution)
- OS가 시스템에 대한 제어를 유지하면서 효율적인 방식으로 CPU를 가상화하는 것이 목표 -> HW와 OS의 지원이 필요
<br>

## Memory virtualization

- ```CPU가 32비트다.``` == ```Register의 크기가 32비트이다.```
- 이는 총 2^32개 즉, 4GB의 가상 주소 공간을 갖는다는 의미이다.
- 현대 OS에선 2^48 까지 사용
<br>

## 가상 메모리 vs 가상 메모리 주소 공간

- 가상 메모리 = 실제 메모리 + Swap 영역
  - Swap 영역은 Disk에 존재 (속도 느림을 해결하기 위해 Linked list로 구현)
- 가상 메모리 주소 공간은 register 크기에 따라 Process가 주소를 처리할 수 있는 영역의 크기
- 가상 메모리 주소 공간은 register 크기에 비례하며 2^(register 크기) 값의 가상 주소 공간을 갖게 됨
<br>

- Process 전체가 Swap 영역에 올라간 경우 -> 해당 프로그램이 실행될 수 없는 상태
- Process 일부만 Swap 영역에 있는 경우 -> Memory에서 실행하기 위한 Page가 없을 경우 Swap 영역에서 찾는다.(page fault)
<br>

## 메모리 영역

- Code: 프로그램 실행 코드, 컴파일때 결정되고 중간에 코드를 바꿀 수 없게 READ-Only
- Data: 전역변수, static 변수 같은 고정된 데이터가 할당되는 공간
  - 전역변수, static 값을 참조한 코드는 컴파일 후 Data 영역의 주소 값을 가르키도록 변경
  - 실행도중 전역변수의 값이 변경될 수 있기 때문에 Read-Write
  - 초기화되지 않는 전역 변수는 BSS(Block Started by Symbol)에 할당
- Stack: 함수내의 지역변수, 매개변수, 리턴값 등이 저장되고 함수가 종료되면 해당 데이터도 제거
  -LIFO 방식을 따름
  - Low address 방향으로 추가 할당(Heap 쪽으로)
  - stack이 Heap 영역 침범시 Stack overflow
- Heap: 런타임에 결정되는 영역으로 malloc(cpp), new 객체(java) 와 같이 동적할당으로 객체를 생성
  - High address 방향으로 추가 할당 (stack 쪽으로)
  - Heap이 Stack 영역 침범시 Heap overflow
  - 포인터로 메모리 영역을 접근하기 때문에 다른 자료구조에 비해 데이터 읽고 쓰는게 느림
<br>

- Code와 Data 영역은 Compile 시 메모리 할당되어 크기가 고정
- Heap과 Stack 영역은 Runtime 시 메모리가 할당되어 가변적
<br>

## PCB (Process Control Block)

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

## Process state

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
- free page(page frame)에 실행해야하는 부분을 올리며 PTE가 만들어진다.
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

