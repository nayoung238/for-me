# top 명령어 (Linux)

운영중인 서버의 cpu와 메모리 상태를 알 수 있는 명령어이다.

![png](/_img/Operating-system/top_command_1.png)

- 상단 : cpu 사용률과 메모리 사용률
- 하단 : 프로세스별 cpu & 메모리 사용률
- `top -d 1` 하단 정보가 1s마다 바뀜
- top 실행중 `h` 입력시 관련 명령어 출력

## 상단

![png](/_img/Operating-system/top_command_2.png)

- uptime : 시간 정보(현재 시간, 컴퓨터 실행시간)
- load average : 시스템 부하 평균값(EMA, 지수 이동 평균값)

cpu 싱글코어 기준으로 시스템 상태 확인하는 법은 다음과 같다.
- 1.0 -> 100% load
- 0.5 -> 50% load
- 1.7 -> 170% CPU가 처리하지 못하고 대기 중인 프로세스가 존재
<br>
 
### task(process) 정보

![png](/_img/Operating-system/top_command_task.png)

- state `i (idle)` 는 counting 하지 않음 (`idle` : user, kernel task가 멈춰있는 상태)
- `ps -eo comm,state` 모든 프로세스의 이름과 상태 출력 (idle 상태의 프로세스까지 포함)
- `ps --no-headers -eo comm,state | grep I | wc -l`  상태가 idle인 개수 출력
<br>

#### `ctrl + z` stopped 상태로 변경

![png](/_img/Operating-system/top_command_stop.png)
<br>

#### `fg` 작성시 stop 해제 (foreground)

![png](/_img/Operating-system/top_command_foreground.png)
<br>

### cpu 사용비율

![png](/_img/Operating-system/top_command_cpu.png) 

- us(user-model) : 유저 영역에서의 CPU 사용률
- sy(system-mode) : 커널 영역에서의 CPU 사용률
- ni(user-mode) : 프로세스의 우선순위(nice) 설정에 사용하는 CPU 사용률
- id(idle) : 사용하고 있지 않는 비율
- wa(I/O wait) : I/O 처리 완료를 기다리고 있는 CPU 비율 (값이 높을수록 I/O 처리가 지연됨을 의미)
- hi(hard irq) : HW 인터럽트에 사용되는 CPU 사용률, 전반부처리(빠른 처리)
- si(soft irq) : SW 인터럽트에 사용되는 CPU 사용률, 후반부처리(지연 처리), 나중에 처리해도 상관 없는것
- st : CPU를 VM에서 사용하여 대기하는 CPU 비율 (Guest OS를 위한 VM 처리)
<br>

### 메모리 사용률

![png](/_img/Operating-system/top_command_memory.png) 

첫 번째 줄(Mem)은 RAM의 메모리 영역, 두 번째 줄(Swap)은 Swap 메모리 영역을 나타낸다. 일반적으로 Mem의 사용량이 가득차면 Swap 영역을 사용한다. Swap 영역은 디스크에 존재하므로 RAM 보다 속도가 느리다.<br>

memory 사용률 모두 KB단위는 다음과 같다.<br>

- total : 총 메모리 공간
- free : 남는 메모리 공간
- used : 사용중인 메모리 공간
- buff / cache : I/O 관련되어 사용되는 버퍼에서의 사용되는 메모리
  - buff : 커널 버퍼에서 사용되는 메모리
  - cache : 디스크의 페이지 캐시
  - ```total = free + used + buff/cache```
- avail Mem : swap 영역을 제외하고 사용 가능한 메모리 공간 (회수하면 얻을 수 있는 공간까지 포함)
<br>

```free``` 명령어로도 사용률을 확인할 수 있음

![png](/_img/Operating-system/command_free.png) 

<br>

## 하단

### 프로세스 세부정보

![png](/_img/Operating-system/top_command_process.png)

- PID : process ID
- USER : 해당 프로세스를 실행한 USER 이름 또는 효과를 받는 USER 이름
- PR : 커널에 의해 스케줄링되는 우선순위, nice + 20
- NI : nice
- VIRT : virtual memory space, 프로세스가 소비하는 총 메모리 (프로그램이 실행 중인 코드, Heap, Stack과 같은 메모리, I/O buffer 메모리 포함)
- RES : 실제 사용중인 physical memory space (RAM에서 사용 중인 메모리)
- SHR : 공유 메모리 공간
- S : process state
- %CPU : cpu 사용률
- %MEM : 메모리 사용률
- TIME+ : 프로세스가 사용한 총 CPU 시간
- COMMAND : 해당 프로세스를 실행한 command
<br>

### state 상태

- D : uninterruptible sleep(디스크 또는 네트워크 I/O 처리 대기)
- R : running
- S : sleeping
- I : idle (sleep 중인 kernel thread)
- T : stopped by job control signal
- t : stopped by debugger during trace
- Z : zombie

