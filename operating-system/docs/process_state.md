## Process state

PCB의 program counter(PC register) 값에 의해 결정된 순서에 따라 CPU가 명령어를 수행한다. 
개별 프로세스 행위의 특성은 해당 프로세스를 위해 수행되는 일련의 명렁어 리스트로 표현하는데 이를 **프로세스의 궤적(trace)** 이라고 한다.

![png](/operating-system/img/process_state.png)

### Create

- ```fork()``` 실행 시 새로운 프로세스 생성
- 메모리가 충분할 경우 메모리에 올라감
- 그렇지 않으면 디스크 내 **Swap 영역**에 올라감


### Ready

- 실행 중인 프로세스가 ```TQ(Time Quantum)``` 에 의해 Context switching 되었다면 해당 프로세스는 Ready 상태 
- 메모리가 부족하면 Swap 상태 (suspend 상태)
- 대기 큐에 있던 프로세스 중 Scheduler에 의해 선택된 프로세스는 Running 상태가 되면서 CPU를 할당하는데 이를 Dispatcher가 수행


### Swap In / Out (Swapping)

- Swapping : **프로세스 전체**를 swap 영역으로 이동
- Swap In : swap 영역에 있는 프로세스를 **메모리로 이동**
- Swap Out : 메모리 확보를 위해 사용 빈도가 적은 프로세스를 swap 영역으로 이동


### Page In / Out

- Page in : swap 영역에 있는 **프로세스의 일부**를 메모리로 로드
- Page out : 프로세스의 일부만 swap 영역으로 이동시켜 free page 확보

### Page fault

- 메모리에 프로세스의 일부만 올렸을 경우 CPU가 접근하려고 하는 부분이 메모리가 아닌 swap 영역에 존재하는 경우에 발생
- 디스크에 존재하면 실행 불가
- 이때 free page를 제공해 swap 영역에서 프로세스를 가져와 PTE(page table entry) 생성


### Preempted

- Running 상태인 프로세스에 interrupt가 걸리면 다른 프로세스에게 cpu권한을 넘겨주는 상태
- 프로세스끼리 cpu 권한을 주고받는 것이 multi programming
- **non-preemted**: 모두 실행할 때까지 CPU 권한을 넘겨주지 않는 것으로 single programming
  - 예를들면 **MS-DOS**: 실제 메모리에 1개의 프로세스만 실행했으며 일반 애플리케이션이 하드웨어에 직접 접근이 가능한 위험한 구조


### Interrupt

- 현재 수행 중인 프로세스와는 독립적으로 외부에서 유발되는 여러 종류의 사건에 의해 발생
- 유저 모드에서 interrupt 걸려 커널 모드로 변경되는 경우 (애플리케이션에서 system call 호출)
- 커널 모드에서 interrupt 걸려 다시 커널 모드로 유지되는 경우


### Zombie

- ```exit()``` 호출에 의해 Context switching 되었다면 해당 프로세스는 좀비 상태가 되며 실행이 불가능한 상태
- 부모 프로세스가 제거할 때까지 메모리에 남아 있음
- 부모 프로세스가 먼저 죽으면 PCB를 통해 자신의 ancestor를 찾아 제거


### Sleep

- scanf, printf 같은 ```I/O()``` 호출에 의해 Context switching 되었다면 해당 프로세스는 **block(sleep) 상태**로 변경
- **I/O 수행 과정이 길어져 메인 메모리에서 sleep 상태가 지속된다면 swapper가 disk로 swap out**
- 이때 디스크에서 sleep 상태를 유지하며 I/O를 수행


### Wake up

- Sleep 시간이 짧아(I/O 수행시간이 짧아) 바로 메인 메모리에서 Wake up하면 Ready queue로 push
- Sleep 시간이 길어 disk에 존재한 프로세스가 Wake up하면 swapper가 메모리의 ready queue에 push

<br>

### S(state) 종류

```top``` 명령어로 프로세스의 상태를 확인 가능
- D : uninterruptible sleep(디스크 I/O 처리대기)
- R : running
- S : sleeping (네트워크 I/O 처리대기)
- I : idle(sleep 중인 kernel thread)
- T : stopped by job control signal
- t : stopped by debugger during trace
- Z : zombie
