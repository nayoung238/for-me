# top 명령어

운영중인 서버의 cpu와 메모리 상태를 알 수 있는 명령어이다.

- 상단 : cpu 사용률과 메모리 사용률
- 하단 : 프로세스별 cpu, 메모리 사용률(rld)

`top -d 1` 하단 정보가 1s마다 바뀜<br>
top 실행중 `h` 입력시 관련 명령어 출력<br>

## 상단

![png](/Operating-system/_img/command(1).png) <br>

- uptime : 시간정보(현재 시간, 컴퓨터 실행시간)
- load average : 시스템 부하평균값(EMA, 지수 이동 평균값)

cpu 싱글코어 기준으로 시스템 상태 확인하는 법은 다음과 같다.<br>
- 1.0 -> 100% load
- 0.5 -> 50% load
- 1.7 -> 170% (시스템 상태 확인해야됨)
 
### task(process) 정보

![png](/Operating-system/_img/command_task.png) <br>

state `i` 는 counting 안됨 ( `idle` user, kernel task가 멈춰있는 상태 )<br>
`ps -eo comm,state`모든 프로세스의 이름과 상태 출력(i까지 포함)<br>
`ps --no-headers -eo comm,state | grep I | wc -l`  상태가 i인 개수출력<br><br>
`ctrl + z` stopped상태로 변경<br>

![png](/Operating-system/_img/command_stop.png) <br>

`fg` 작성시 stop된 것 풀림(foreground)<br>

![png](/Operating-system/_img/command_foreground.png) <br>

### cpu 사용비율

![png](/Operating-system/_img/command_cpu.png) <br>

- us(user-model) : 높은 우선순위의 cpu 점유율(nice 0 또는 음수)
- sy(system-mode) : kernel mode를 의미
- ni(user-mode) : 낮은 우선순위 (nice 양수)의 cpu 점유율
- id(idle) : 시스템 얼마나 여유로운지(공회전)
- wa(I/O wait) : I/O 처리완료를 기다린 시간(값이 높을수록 i/o처리가 지연됨을 의미)
- hi(hard irq) : 인터럽트 전반부처리(빠른 처리)
- si(soft irq) : 인터럽트 후반부처리(지연 처리), 나중에 처리해도 상관없는것
- st : 가상 cpu 구동, guest os를 위한 vm처리

### MEM 사용률

![png](/Operating-system/_img/command_memory.png) <br>

memory 사용률 모두 KB단위는 다음과 같다.<br>

- total : 총 메모리 공간
- free : 남는 메모리 공간
- used : 사용중인 메모리 공간
- buff / cache : 디스크에 있는 내용을 메모리에 캐시

```total = free + used + buff/cache```<br>

- avail Mem : 프로세스가 처음 시작시 사용가능한 메모리공간(회수하면 얻을 수 있는 공간까지 포함)
- swap : 메모리의 내용을 저장해둘 수 있는 Disk 공간

```free``` 명령어로도 사용률을 확인할 수 있음<br>

![png](/Operating-system/_img/command_free.png) <br>

<br>

## 하단

### 프로세스 세부정보

![png](/Operating-system/_img/command(2).png) <br>

- PID : process ID
- PR : nice + 20
- NI : nice
- VIRT : virtual memory space
- RES : 실제 사용중인 physical memory space
- SHR : 공유메모리 공간
- S : process state
- %CPU : cpu 사용률
- %MEM : 메모리 사용률
- TIME+ : 프로그램 실행한 시간

state 상태는 다음과 같다.

- D : uninterruptible sleep(디스크 I/O 처리대기)
- R : running
- S : sleeping(네트워크 I/O 처리대기)
- I : idle(sleep 중인 kernel thread)
- T : stopped by job control signal
- t : stopped by debugger during trace
- Z : zombie


