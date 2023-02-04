## Kernel

- 기본적으로 메모리에 상주하는 Core가 되는 Software
- Hardware를 소프트웨어적으로 Abstraction하여 개발자가 programming 또는 HW에 쉽게 접근하도록 도와줌

<br>

## Kernel mode vs User mode

![png](/Operating-system/_img/modes_of_execution(1).png)

![png](/Operating-system/_img/modes_of_execution(2).png)

- 사용자 프로그램의 간섭으로부터 OS 및 PCB와 같은 주요 운영체제 테이블을 보호할 필요가 있고 이를 위해 2가지 모드로 나뉨
- User mode(mode bit = 1) : 사용자 Application Program이 대부분 수행되는 모드
- Kernel mode(mode bit = 0) : 프로세스가 수행되다가 Interrupt가 걸려 OS가 호출되어 수행되는 mode
  - OS는 시스템의 모든 리소스에 엑세스 가능
  - kernel mode에서 수행되는 Software는 처리기와 모든 명령, register 그리고 메모리 제어가 가능

<br>

- User mode -> Kernel mode
  - 사용자가 **System call**
  - Interrupt가 OS kernel을 수행을 촉발(trigger)시킬 때
- Kernel mode -> User mode
  - 제어가 OS 서비스에서 사용자 프로세스로 복귀할 때

<br>

## Interrupt & Trap

- Interrupt: 현재 수행중인 프로세스와는 독립적으로 I/O 같은 **외부에서 유발**되는 여러 종류의 사건에 의해 발생
- Trap: 불법적인 파일 접근 시도와 같이 현재 수행되고 있는 프로세스에서 생성되는 오류나 예외 조건 때문에 발생 (Kernel 내부에서 자체적으로 call)
<br>

### HW Interrupt

- HW Clock Interrupt(시간 할당량을 모두 사용했는지 여부 결정), HW IO Interrupt
- 현재 명령어 수행의 외부에서 발생
- 비동기적인 외부 사건에 반응
<br>

### SW Interrupt

- Trap: 현재 명령어 수행과 관련되며 오류나 예외 상황 처리하기 위해 Kernel 함수 수행
  - page fault, 0으로 나누기 -> **비동기적**
- System call(Supervisor 호출): 사용자가 명시적으로 ```open()```, ```read()```, ```write()```, ```fork()``` 등을 호출 (OS 기능에 대한 호출)

<br>

## Polling & Interrupt

- 컨트롤러가 입력을 받아들이는 방법에는 Polling 과 Interrupt 방식
- Polling 방식: 사용자가 명령어를 사용해 지속적으로 변화를 알아내는 방식
  - Polling 하는 시간에는 원래 하던 일을 수행하지 못한다는 단점
  - 우선순위가 가장 높은 Interrupt 자원을 찾아 Interrupt Service routine을 수행
- Interrupt 방식: MCU(Microcontroller Unit)가 Hardware의 변화를 체크하면서 변화 시에만 일정한 동작을 하는 방식
  - Hardware의 지원을 받지만 Polling에 비해 신속하게 대응