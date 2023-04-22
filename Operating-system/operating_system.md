## operating system 이란

- 다양한 Application의 실행을 위해 **컴퓨터를 실행**하고 **Hardware를 관리**하는 Software를 operating system이라고 함
- 여러 프로세스가 메모리를 공유하고 디바이스와 상호 작용하는 등의 작업을 대신함으로써 사용자가 프로그램을 쉽게 사용할 수 있게 하는 Software를 operating system이라고 함
<br>

- Computer Hardware를 Software로 abstract(추상화)하여 관리
- ```CPU```를 ```Process```로 관리
- ```RAM```을 ```Memeory management```로 관리
- ```Disk```를 ```File System```으로 관리

<br>

## Abstraction & Virtualization

- Computer Hardware(CPU, RAM, Disk)를 Software로 **abstract(추상화)** 하여 ```Process```, ```Memeory management``` 그리고 ```File System```으로 관리
- ```Process```와 ```Memeory management```는 사용자에게 환상을 제공하는데 이를 **Virtualization(가상화)** 라고 함
- ```CPU```를 추상화한 ```Process```들은 아주 빠르게 번갈아가면서 실행하는데 이는 사용자가 **CPU가 여러가지 일을 동시에 처리**한다고 착각하게 만듦
- ```RAM```을 추상화한 ```Memeory management```에 대해 각각의 ```Process```가 **자신만의 physical memory를 가지고 있다**라는 환상을 제공

<br>

## CPU virtualization

- 여러 프로세스가 마치 병렬적으로 동시에 실행되는 것 같은 환상을 제공
- 실제로는 여러 프로세스가 번갈아가며 CPU를 차지 (**Interleaved** execution)
  - register에 프로세스의 상태를 올려 연산하고 일정 시간이 지나거나 이벤트로 인해 다른 프로세스에게 CPU 권한을 넘겨줌
- OS가 시스템에 대한 제어를 유지하면서 효율적인 방식으로 CPU를 가상화하는 것이 목표 -> HW와 OS의 지원이 필요
- 여러 프로세스가 1개의 CPU를 interleave하게 차지하며 수행하는데, 차지하는 기준은 Time Quantum(Time slicing)인 CPU를 차지할 수 있는 시간으로 보통 6, 10ms

### Concurrency

- 많은 프로세스를 번갈아가며 수행하기 때문에 **공유 데이터**에 접근하는 순서가 문제
- 여러 pthread를 생성해 전역 변수에 접근할 경우 생성된 순으로 접근한다는 보장 없음
- 여러 스레드가 동시에 접근하는 자원을 Critical resource(임계 자원)이라고 함
- 임계자원에 동시에 쓰기 작업을 하는 경우에 갱신 손실(Lost update)이 발생할 수 있으며 이는 락, 세마 포어 등으로 해결

<br>

## Memory virtualization

- CPU가 32비트라면 Register의 크기가 32비트라는 의미
- Register 크기가 32비트라면 ```2^32 = 약 4GB```의 **가상 주소 공간**을 갖게됨 (현대 OS에선 2^48 까지 사용)
- 즉, 프로세스마다 약 4GB 크기의 가상 주소 공간 소유하고 이 가상 주고 공간이
- 가상 주소 공간을 통해 가상 메모리에 프로세스의 일부를 load
- 프로세스의 일부를 load해야 가상 메모리에 더 많은 프로세스를 올릴 수 있음
- 디스크 I/O는 CPU를 사용하지 않으며, 메모리에 많은 프로세스가 올라갈수록 CPU가 필요한 프로세스를 찾을 가능성이 높아져 CPU의 utilization을 최대화

### 가상 메모리 

- 물리 메모리와 디스크의 Swap 영역을 가상 메모리라고 함
- 프로세스가 디스크의 Swap 영역에 있을 경우 실행할 수 없는 상태
- 그러므로 프로세스의 일부라도 물리 메모리에 load하려고 함
- Page fault: 메모리에서 실행하기 위한 프로세스의 Page가 Swap 영역에 있는 경우 

<br>

## Persistence

- 휘발성인 DRAM에 데이터를 저장할 수 없기 때문에 전원이 꺼지고 시스템이 손상되더라도 데이터를 저장하기 위한 HW, SW가 필요
- Disk는 데이터의 지속적이고 안전한 저장이 최우선이므로 가상화 개념을 도입하지 않음
  - Hardware: 하드 드라이브, SSD(Solid-state Drive) 같은 IO 장치 필요
  - Software: 파일 시스템은 사용자가 생성한 모든 파일을 안전하게 저장
- **tmp** directly 내에 있는 모든 파일은 모든 user가 공유 가능한데 delete는 file 소유자만 가능

<br>

## OS 목표

- 가상화를 통해 시스템을 편리하고 사용하기 쉽게 만듦
- kernel mode, user mode로 나눈 것처럼 자원을 보호
- 최소한의 overhead를 위해