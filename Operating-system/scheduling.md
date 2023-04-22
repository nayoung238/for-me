## Scheduling

- 다음에 실행할 프로세스와 대기해야할 프로세스를 결정하는 과정은 시스템의 전체 성능에 영향을 미침
- 대기 중인 프로세스들에 대해 대기 시간을 줄이는 것이 목표
- **Preemption** : OS가 CPU의 사용권을 선점할 수 있는 경우로 강제 회수가 가능
  - 다른 프로세스에게 양도가 가능해 처리시간 예측 불가능
- **Non-preemption** : 프로세스 종료 및 I/O 등의 event가 있을 때까지 실행 보장
  - 다른 프로세스에게 양도 하지 않으므로 처리시간 예측 가능
- Turn around time(반환 시간) = completion(작업 완료 시간) - arrival(작업 도착 시간)
- Turn around time은 수행 시간이 아님에 주의
<br>

- 장기(long-term) 스케줄링: 프로세스가 CPU에 의해 실행될 수 있는지 자격을 부여하는 결정
- 중기(medium-term) 스케줄링: 프로세스 전부 혹은 일부가 main memory에 올라갈 수 있는지 자격을 부여하는 결정
- 단기(short-term) 스케줄링: CPU의 권한을 부여할 다음 프로세스를 선택하는 결정
  - FIFO, SJF, RR, HRRN, MLFQ scheduling이 단기 스케줄링

<br>

## Dispatcher

- schedular는 다음으로 CPU 제어권을 갖게할 프로세스를 선택
- Dispatcher는 선택된 프로세스에게 CPU 제어권을 넘김
- 프로세스의 Register를 적재 (Context switching)

<br>

## FIFO 

- First In First Out으로 FCFS(First Come First Service)라고도 함
- Non-preemption (작업 끝날 때까지 넘겨주지 않음)
- Convoy effect(호위병 효과): 바로 다음에 들어온 프로세스는 0.001s의 시간으로 CPU 사용이 끝나지만 앞에 있는 프로세스의 사용이 끝나야만 처리 가능하므로 비효율적
- 즉, 실행시간이 짧은 프로세스가 뒤에 있을수록 평균 대기 시간이 길어지는 단점

<br>

## SJF

- Shortest Job First로 SPN(Shortest Process Next)라고도 함
- Non-preemption (작업 끝날 때까지 넘겨주지 않음)
- 프로세스의 CPU 사용이 끝나면 대기 중인 프로세스 중 service 시간이 제일 짧은 프로세스를 선택
- FIFO에서 발생한 Convey effect을 해결할 수 있음
- 먼저 들어온 긴 작업의 프로세스는 계속해서 뒤로 밀리기 떄문에 Starvation 현상 발생

<br>

## STCF

- Shortest Time to Completion First로 SRT(Shortest Remaining Time)라고도 함
- Preemption (작업 도중에 다른 프로세스에게 권한을 넘겨줌)
- 실행 중인 프로세스의 남은 Service 시간과 대기 중인 프로세스의 Service 시간을 비교해 CPU 사용 권한을 계속 변경
- 남은 Service 시간이 긴 프로세스는 계속 뒤로 밀려 Starvation 현상 발생

<br>

## HRRN

- Highset Response Ratio Next
- (Time spent waiting + expected service time) / expected service time
- Non-preemption
- SJE의 starvation도 해결하고 기다린 시간도 보장

<br>

## Round Robin

![png](/Operating-system/_img/round_robin.png)

- Preemption (작업 도중에 다른 프로세스에게 권한을 넘겨줌)
- 실제 Scheduler 기반
- Time slicing = Scheduling Quantum = Time Quantum
- Time Slicing의 길이 중요
  - 너무 길면 Non-preemption
  - 너무 짧으면 Schduling하는 시간이 더 소요될 수 있음
