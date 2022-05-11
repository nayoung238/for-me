# PCB(Process Control Block)

CPU를 추상화한 것을 Process 라고 한다. 모든 프로세스는 자신만의 process struct를 갖는데 이를 PCB(Process Control Block) 라고 한다.<br>
Linux에서 PCB는 **task_struct** 으로 정의되어 있으며 처리기에 할당되어 수행되는 Entity이다.<br>
PCB를 통해 수행 프로세스를 interrupt한 후 나중에 interrupt가 발생되지 않은 것처럼 프로세스 수행을 재개 수 있도록 충분한 정보를 유지한다.<br>

- PID : 프로새스 식별자
- state : 현재 상태
- nice : 우선 순위
- program counter
- *mm : 메모리 포인터로 가상 주소를 가리키고 있다. (code, data, stack 로 구성되지만 연속적이지 않을 수 있음. 시작주소로 찾아냄.)
- register : 문맥 데이터로 어디까지 수행했는지 저장
- I/O information
- accounting information

## Context (문맥)

PCB에서 context는 HW(cpu), system, memory 문맥으로 나눌 수 있다.<br>

- HW(cpu) : register 정보
- memory : memory pointer 가 가리키는 struct (가상주소 -> 실제 주소)
- system : register, memory 문맥을 제외한 모든 것

어떤 프로세스가 interrupt 되면 program counter 및 문맥 데이터(register)의 현재 값이 해당 프로세스 제어블록의 적절한 필드에 저장되면서 프로세스의 상태가 변한다.(Save the old context)<br>
OS는 다른 프로세스를 선택해 Run 상태로 변경하고 해당 프로세스의 program counter와 문맥 데이터를 처리기 레지스터(cpu register)로 적재하여 새로운 프로세스를 수행한다.(Execution of the new context)<br>
<br>

## Process API

- Create : fork() 로 프로세스를 생성한다. 부모 프로세스에서 fork()하면 자식 프로세스가 생성되면서 자식 PID가 리턴되고 자식에겐 0이 리턴된다.
- Destroy : exit() 로 프로세스를 종료한다.
- Wait : wait(), wait2() 로 프로세스를 멈춘다.
  - if no parent waiting(did not invoke wait()) process is a zombie
  - if parent terminated without invoking wait, process is an orphanj

프로그램은 실행 가능한 형식(executable binary format)으로 디스크에 존재한다.<br>
디스크에 있는 프로그램을 메모리에 모두 또는 일부를 load 한다.<br>

CPU utilization을 올리기 위해선 메모리에 일부분만 load 한다.<br>
I/O 작업을 하는 프로세스는 CPU를 차지하지 않고 I/O controller 가 수행한다. 여러 프로세스의 일부분만 load 하면 I/O 작업이 아닌 연산을 필요로 하는 프로세스를 찾을 수 있는 경우가 증가하기 때문이다.<br>
이는 현재 메모리 가상화 정책 중 하나이다.<br><br>

# Process state

PCB의 program counter(PC register) 값에 의해 결정된 순서에 따라 CPU가 명령어를 수행한다.<br>
개별 프로세스 행위의 특성은 해당 프로세스를 위해 수행되는 일련의 명렁어 리스트로 표현하는데 이를 프로세스의 궤적(trace)이라고 한다.<br>

![png](/Operating_system/_img/process_state.png) <br>

## Create

- fork()실행시 새로운 프로세스가 생성된다.
- 메모리가 충분할 경우 Memory로 올라간다.
- 그렇지 않을 경우, Disk내 Swap영역으로 올라간다.
- 요즘 PC는 대부분 메모리가 충분하므로 모두 메모리로 올라간다.

## Swap In / Out (Swapping)

- Swapping : 프로세스 전체를 swap 영역으로 보낸다.
- Swap In : swap 영역에 있는 프로세스를 메모리로 load한다.
- Swap Out : 메모리 확보를 위해 사용 빈도수가 적은 프로세스를 swap 영역으로 이동시킨다.

> In / Out 개념이 헷갈릴 수 있는데 메모리 기준으로 생각하면 된다.<br>

## Page In / Out

- Page in : swap 영역에 있는 프로세스의 일부를 physical memory로 load해 page frame을 제공한다.
- Page out : 프로세스의 일부만 swap 영역으로 다른 프로세스에게 page frame을 넘겨준다.

프로그램이 수행되기 위해서 프로세스에게 page frame을 제공해야 하며 사용되지 않은 page frame을 free page라고 한다.<br>
free page가 부족하게 되면 page out이 발생한다.<br>

## Page fault

메모리에 프로세스의 일부만 올렸을 경우 CPU가 접근하려고 하는 page frame이 메모리에 없는 경우가 발생한다.<br>
이때 free page를 제공해 swap 영역에서 프로세스를 가져와 PTE(page table entry)를 만든다.<br>

## Ready to Run

Ready Queue에 있던 프로세스가 Scheduler에 의해 CPU 권한을 받게 되면 Running 상태로 변한다.<br>

## Preempted

running 상태인 프로세스에 time interrupt가 걸리면 다른 프로세스에게 cpu권한을 넘겨주는 것을 말한다.

> 프로세스끼리 cpu권한을 주고받는 것을 multi programming 이라고 한다.<br>
> 반대의 경우 non-preemted로 single programming이라고 한다.<br>
> 예를들면 MS-DOS 가 있는데 실제 메모리에 1개의 프로세스만 실행했으며, 일반 application도 하드웨어에 직접 접근이 가능한 위험한 구조였다.<br>

## Interrupt

2가지 interrupt가 존재한다.

- user mode에서 interrupt 걸려 kernel mode로 변경되는 경우 : application에서 system call 호출
- kernel mode에서 interrupt 걸려 다시 kernel mode로 유지되는 경우

## Zombie

exit()시 좀비상태가 되며 실행이 불가능한 상태이지만 부모 Process가 지울 때까지 메모리에 남아있다.<br>
부모 proccess가 먼저 죽은경우 PCB를 통해 자신의 ancestor를 찾아 지운다.<br>

## Sleep

scanf, printf 같은 I/O를 수행하면 Sleep 상태로 변경된다.<br>
I/O 수행과정이 길어져 메인 메모리에서 Sleep 상태가 지속된다면 swapper가 Disk로 swap out 한다.<br>
이때 Disk에서 Sleep 상태를 유지하며 I/O를 수행한다.<br>

## Wake up

Sleep 시간이 짧아(I/O 수행시간이 짧아) 바로 메인 메모리에서 Wake up 하면 Ready queue 에 들어간다.<br>
Sleep 시간이 길어 Disk에 존재한 프로세스가 Wake up 하면 swapper가 실제 메모리의 ready queue에 넣는다.<br>

## top 명령어

```top``` 명령어로 프로세스의 상태를 확인할 수 있다.

> top 대한 자세한 설명 : https://github.com/evelyn82ny/Computer_science/Operating_System/blob/main/theory/commend.md


### S(state) 종류

- D : uninterruptible sleep(디스크 I/O 처리대기)
- R : running
- S : sleeping(네트워크 I/O 처리대기)
- I : idle(sleep 중인 kernel thread)
- T : stopped by job control signal
- t : stopped by debugger during trace
- Z : zombie
