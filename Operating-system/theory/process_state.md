## Process API

- Create : ```fork()```로 프로세스 생성
  - 부모 프로세스에서 ```fork()```하면 자식 프로세스가 생성되면서 **자식 PID가 리턴**되고 **자식에겐 0이 리턴**
- Destroy : ```exit()``` 로 프로세스 종료
- Wait : ```wait()```, ```wait2()``` 로 프로세스 중단
  - if no parent waiting(did not invoke wait()) process is a zombie
  - if parent terminated without invoking wait, process is an orphanj
<br>

프로그램은 실행 가능한 형식(executable binary format)으로 디스크에 존재한다. 디스크에 있는 프로그램을 메모리에 모두 또는 일부를 load 해야 실행 가능한 상태가 된다.<br>

CPU utilization을 올리기 위해선 메모리에 일부분만 load 한다. I/O 작업을 하는 프로세스는 CPU를 차지하지 않고 I/O controller 가 수행한다. 그러므로 여러 프로세스의 일부분만 load 하면 I/O 작업이 아닌 연산을 필요로 하는 프로세스를 찾을 수 있는 경우가 증가하기 때문이다. 이는 현재 메모리 가상화 정책 중 하나이다.

<br>

## Process state

PCB의 program counter(PC register) 값에 의해 결정된 순서에 따라 CPU가 명령어를 수행한다. 개별 프로세스 행위의 특성은 해당 프로세스를 위해 수행되는 일련의 명렁어 리스트로 표현하는데 이를 **프로세스의 궤적(trace)** 이라고 한다.

![png](/Operating_system/_img/process_state.png)

### Create

- ```fork()``` 실행 시 새로운 프로세스 생성
- 메모리가 충분할 경우 Memory에 올라감
- 그렇지 않을 경우 Disk내 **Swap영역**으로 올라감
- 요즘 PC는 대부분 메모리가 충분하므로 모두 메모리로 올라감

<br>

## Ready

-  실행중인 프로세스가 ```TQ(Time Quantum)``` 에 의해 Context switching 되었다면 해당 Process는 Ready 상태 
- memory가 부족하면 Swap 상태 (suspend 상태)
- 대기큐에 있던 프로세스 중 Scheduler에 의해 선택된 프로세스는 Running 상태가 되면서 CPU를 할당하는데 이를 Dispatcher가 수행
<br>

### Swap In / Out (Swapping)

- Swapping : **프로세스 전체**를 swap 영역으로 이동
- Swap **In** : swap 영역에 있는 프로세스를 **메모리로 이동**
- Swap Out : 메모리 확보를 위해 사용 빈도수가 적은 프로세스를 swap 영역으로 이동

> In / Out 개념이 헷갈릴 수 있는데 메모리 기준으로 생각

<br>

## Page In / Out

- Page in : swap 영역에 있는 **프로세스의 일부**를 메모리로 load해 page frame을 제공
- Page out : 프로세스의 일부만 swap 영역으로 이동시켜 다른 프로세스에게 page frame을 넘겨줌
- 프로그램이 수행되기 위해서 프로세스에게 page frame을 제공해야 하며 사용되지 않은 page frame을 free page라고함
- free page가 부족하게 되면 page out 발생

<br>

## Page fault

- 메모리에 프로세스의 일부만 올렸을 경우 CPU가 접근하려고 하는 부분이 메모리가 아닌 swap 영역에 존재하는 경우에 발생
  - 디스크에 존재하면 실행할 수 없음
- 이때 free page를 제공해 swap 영역에서 프로세스를 가져와 PTE(page table entry) 생성

<br>

## Preempted

- Running 상태인 프로세스에 interrupt가 걸리면 다른 프로세스에게 cpu권한을 넘겨주는 상태
- 프로세스끼리 cpu권한을 주고받는 것이 multi programming
- **non-preemted**은 모두 실행할 때까지 CPU 권한을 넘겨주지 않는 것으로 single programming
  - 예를들면 **MS-DOS**가 있는데 실제 메모리에 1개의 프로세스만 실행했으며 일반 application도 하드웨어에 직접 접근이 가능한 위험한 구조

<br>

## Interrupt

- 현재 수행중인 프로세스와는 독립적으로 I/O 같이 외부에서 유발되는 여러 종류의 사건에 의해 발생
- user mode에서 interrupt 걸려 kernel mode로 변경되는 경우 (application에서 system call 호출)
- kernel mode에서 interrupt 걸려 다시 kernel mode로 유지되는 경우

<br>

## Zombie

- ```exit()``` 호출에 의해 Context switching 되었다면 해당 프로세스는 좀비상태가 되며 실행이 불가능한 상태
- 부모 Process가 지울 때까지 메모리에 남아 있음
- 부모 proccess가 먼저 죽은경우 PCB를 통해 자신의 ancestor를 찾아 지움

<br>

## Sleep

- scanf, printf 같은 ```I/O()``` 호출에 의해 Context switching 되었다면 해당 Process는 **Block(Sleep) 상태**로 변경
- I/O 수행과정이 길어져 메인 메모리에서 Sleep 상태가 지속된다면 swapper가 Disk로 swap out
- 이때 Disk에서 Sleep 상태를 유지하며 I/O를 수행

<br>

## Wake up

- Sleep 시간이 짧아(I/O 수행시간이 짧아) 바로 메인 메모리에서 Wake up 하면 Ready queue 로 push
- Sleep 시간이 길어 Disk에 존재한 프로세스가 Wake up 하면 swapper가 메모리의 ready queue에 push

<br>

## top 명령어

- ```top``` 명령어로 프로세스의 상태를 확인할 수 있다.
- top 대한 자세한 설명 : https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/command.md


### S(state) 종류

- D : uninterruptible sleep(디스크 I/O 처리대기)
- R : running
- S : sleeping(네트워크 I/O 처리대기)
- I : idle(sleep 중인 kernel thread)
- T : stopped by job control signal
- t : stopped by debugger during trace
- Z : zombie
