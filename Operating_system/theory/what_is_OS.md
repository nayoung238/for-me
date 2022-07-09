## OS가 뭔데?

- 다양한 Applicattion의 실행을 위해 **컴퓨터를 실행**하고 **Hardware를 관리**하는 소프트웨어가 Operating system 
- Computer Hardware를 Software로 abstract(추상화)하여 관리
- ```CPU```를 ```Process```로 관리
- ```RAM```을 ```Memeory management```로 관리
- ```Disk```를 ```File System```으로 관리

<br>

## Abstraction & Virtualization

- Computer Hardware(CPU, RAM, Disk)를 Software로 **abstract(추상화)**하여 ```Process```, ```Memeory management``` 그리고 ```File System```으로 관리
- ```Process```와 ```Memeory management```는 1개가 여러개라고 착각하는 환상을 사용자에게 제공하는데 이를 **Virtualization(가상화)**라고 함
- ```CPU```를 추상화한 ```Process```들은 아주 빠르게 번갈아가면서 실행하는데 이는 사용자가 CPU가 **여러가지 일을 동시에 처리**한다는 착각을 하게 만듦
- ```RAM```을 추상화한 ```Memeory management```에 대해 각각의 ```Process```가 **자신만의 physical memory를 가지고 있다**라는 환상을 제공

<br>

## CPU virtualization

- 여러 프로세스가 번갈아가며 CPU를 차지 (**Interleaved** execution)
- OS가 시스템에 대한 제어를 유지하면서 효율적인 방식으로 CPU를 가상화하는 것이 목표 -> HW와 OS의 지원이 필요
- 여러 프로세스가 1개의 CPU를 interleave 하게 차지하며 수행하는데 차지하는 기준은 Time Quantum(Time slicing)인 CPU를 차지할 수 있는 시간으로 보통 6, 10ms

<br>

## Memory virtualization

- ```CPU가 32비트``` == ```Register의 크기가 32비트```
- 이는 총 2^32 인 약 **4GB**의 가상 주소 공간을 제공
- 현대 OS에선 2^48 까지 사용

<br>

## Persistence

- ```Disk```는 데이터의 지속적이고 안전한 저장이 최우선이므로 가상화 개념을 도입하지 않음
- 전원이 꺼지거나 시스템이 손상되어도 데이터는 유지되어야 함
  - Hardware: 하드 드라이브, SSD(Solid-state Drive) 같은 IO 장치 필요
  - Software: 파일 시스템은 사용자가 생성한 모든 파일을 안전하게 저장