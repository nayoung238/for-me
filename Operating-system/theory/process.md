## Process

- CPU를 Process로 추상화하여 관리
- Linux에서 프로세스를 **task_struct** 라고, BSD와 UNIX에서는 **proc +u** 라고 명칭
<br>

- 실행 중인 프로그램이라고 정의
- 프로그램이 실행 중이라는 의미는 디스크에 있던 프로그램을 메모리에 load하여 OS의 제어를 받는 상태가 되는 것
- 프로세스는 프로그램 실행의 인스턴스로 크롬이라는 프로그램에서 1개의 탭을 열면 1개의 프로세스가, 2개의 탭을 열면 2개의 프로세스가 생기는 개념

<br>

## Virtualization

- 여러 프로세스가 아주 빠른 속도로 **Interleave**하게 CPU를 차지
- 이렇게 여러 프로세스가 **병렬적으로 동시에 수행**되는 것처럼 느끼게 해주는 것을 CPU의 가상화라고 함
- CPU 차지 후 다른 프로세스에게 양도하는 기준은 **event 발생** 또는 **Time Quantum**
- 보통 10ms이 지나거나 ```I/O```, ```exit()``` 같은 event가 발생하면 10ms 채우지 못했어도 다른 프로세스에게 CPU 양도

<br>

## Context Switch

- 프로세스간의 CPU 양도를 Context Switch 라고 하며 오버헤드 발생
- 다른 프로세스에게 양도하기 전에 자신이 어디까지 처리했는지 기억하기위해 자신의 PCB에 상태를 저장 (Save the old context)
- ```schedule()```로 다음 프로세스의 PID 를 얻고 해당 프로세스의 상태를 레지스터에 올림 (Execution of the New Context)
- 문맥 교환을 요청하면 사용자 모드에서 커널 모드로 변경되고 새로운 프로세스에 CPU를 할당하고 실행시키기 위해 커널모드에서 사용자 모드로 변경하기 때문에 오버헤드 발생
- a, b, c, d 프로세스 순서로 실행해도 문맥 교환이 a, b, c, d 순으로 일어나는 것을 보장할 수 없음

<br>

## PCB (Process Control Block)

- 프로세스의 정보를 저장한 **struct형 자료구조**
- 수행 중 Interrupt가 발생해도 다시 CPU 권한을 받았을 때 프로그램 수행이 제대로 재개할 수 있도록 충분한 정보를 유지
- PID, state, nice(우선순위), PC(Program counter), Memory pointer, HW Context data 등으로 구성
<br>

- PID : 프로세스 고유 식별자
- state : 현재 상태
- PC(program counter) : 프로세스 실행을 위한 다음 명령의 주소를 표시
- nice(우선순위)와 PC(program counter)는 ```schedule()``` 에서 사용됨
- Memory pointer: 메모리 포인터로 메모리 영역에서 struct된 Memory(code, data, stack)의 가상 주소를 가리키고 있음
    - code, data 그리고 stack은 연속적이지 않을 수도 있음
    - 시작 주소로 찾아냄
<br>

- Register : 문맥 데이터로 어디까지 수행했는지 저장
    - HW context data는 현재 프로세스의 진행 상태 정보로 Context switching시 HW context data를 register에 올리고 내림

<br>

## Context

- HW(CPU) context: register 정보들
- Memory context: Memory Pointer가 가리키는 Struct
- System context: HW와 Memory 문맥을 제외한 모든 것

<br>

## 역할

- 사용자 프로세스: 사용자 코드를 수행하는 프로세스
- 시스템(커널) 프로세스: 시스템 메모리와 프로세서의 명령에 접근할 수 있는 프로세스
    - 프로세스의 실행 순서를 제어
    - 다른 사용자 및 커널 영역에 침범하는 것을 막음
    - 사용자 프로세스를 생성