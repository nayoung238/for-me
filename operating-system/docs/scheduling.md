## Scheduling

- 대기 중인 프로세스들의 대기 시간을 줄이는 것이 목표
- **Preemption**
  - OS가 CPU 강제 회수 가능
  - 다른 프로세스에게 양도 가능해 처리시간 예측 불가능
- **Non-preemption**
  - 프로세스 종료 및 I/O 등의 event가 있을 때까지 실행 보장
  - 다른 프로세스에게 양도하지 않으므로 처리시간 예측 가능
- Turn around time(반환 시간) = completion(작업 완료 시간) - arrival(작업 도착 시간)
<br>

- 장기(long-term) 스케줄링: 프로세스가 CPU에 의해 실행될 수 있는가
- 중기(medium-term) 스케줄링: 프로세스 전부 혹은 일부가 main memory에 올라갈 수 있는가
- 단기(short-term) 스케줄링: CPU의 권한을 부여할 다음 프로세스를 선택하는 결정
  - FIFO, SJF, RR, HRRN, MLFQ scheduling이 단기 스케줄링

<br>

## First In First Out

- FCFS(First Come First Service)라고도 함
- Non-preemption
- Convoy effect(호위병 효과) 발생해 실행시간이 짧은 프로세스가 뒤에 있을수록 평균 대기 시간이 길어지는 단점

<br>

## Shortest Job First

- SPN(Shortest Process Next)라고도 함
- Non-preemption
- 프로세스의 CPU 사용이 끝나면 대기 중인 프로세스 중 service 시간이 제일 짧은 프로세스를 선택
- 작업 시간이 짧은 프로세스 유리
- 먼저 들어온 긴 작업의 프로세스는 계속해서 뒤로 밀려 Starvation 현상 발생

<br>

## Shortest Time to Completion First

- SRT(Shortest Remaining Time)라고도 함
- Preemption (작업 도중에 다른 프로세스에게 권한을 넘겨줌)
- 실행 중인 프로세스의 남은 Service 시간과 대기 중인 프로세스의 Service 시간을 비교해 CPU 사용 권한을 계속 변경
- 남은 Service 시간이 긴 프로세스는 계속 뒤로 밀려 Starvation 현상 발생

<br>

## Highest Response Ratio Next

- 응답성을 높이기 위해 대기 시간 고려
- (Time spent waiting + expected service time) / expected service time
- Non-preemption (작업 끝날 때까지 넘겨주지 않음)
- SJF와 STCF 방식에서 발생하는 starvation 현상 해결

<br>

## Round Robin

- 실제 Scheduler 기반
- Preemption
- Time Slicing의 길이 중요
  - 너무 길면 Non-preemption
  - 너무 짧으면 Scheduling하는 시간이 더 소요될 수 있음
  - 실제로 Scheduling 오버헤드는 I/O에 비해 trivial 하다고 함
- 작업 시간이 짧은 프로세스 유리
- 긴 작업이 계속해서 나눠져 응답 시간이 증가하는 문제는 MLFQ로 해결