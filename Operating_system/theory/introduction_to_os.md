# Operating system

프로세스들이 메모리를 공유하고 application이 실행할 수 있는 common service인 API, system call을 제공해 작업을 대신해줌으로써 사용자가 프로그램을 쉽게 사용할 수 있게 하는 SW를 Operating System 이라고 한다.<br>
여러 프로그램이 동시에 실행되는 것처럼 보여 여러 CPU가 존재한다고 생각할 수 있지만, 실제로는 1개의 CPU가 여러 프로세스를 interleave하게 수행한다. 이렇게 CPU가 여러개 있는 것처럼 느끼게 해주는 것을 가상화라고 한다.<br>
여러 프로세스가 1개의 CPU를 interleave 하게 차지하며 수행하는데, 차지하는 기준은 Time Quantum(Time slicing)인 CPU를 차지할 수 있는 시간으로 보통 6, 10ms 이다.<br>

## Kernel mode vs User mode

kernel 이란 기본적으로 메모리에 상주하는 중요한 core가 되는 SW이다. HW를 SW로 abstraction 하여 사용자가 programming 또는 HW에 쉽게 접근하게 한다. 즉, Operating system을 커널이라고 생각해도 된다.<br>
프로세스의 상태를 보호하기 위해 커널 모드와 사용자 모드로 나눠진다. 터미널에서 sudo 로 시작하면 커널 모드로 바뀐다.<br>

## System call vs API

- system call : 커널에 있는 함수를 호출
- API : windows 에서 제공하는 system call을 호출

interface인 API, system call을 통해 하드웨어에 접근할 수 있게 된다.<br>
API, system call 둘다 interface 이지만 API가 더 큰 개념이다.<br><br>

## Abstraction

HW인 CPU, RAM, Disk를 추상화하여 SW(OS)로 관리한다.

- CPU -> Process
- RAM -> memory management
- Disk -> file system

process, memory management, file system으로 추상화하여 SW로 HW를 관리하는데 이렇게 자원을 관리하는 SW(resource manager)가 os 이다.<br>
SW로 추상화된 것을 struct 라고 하며 이를 system call를 이용해 사용한다.<br><br>

## Virtualization

OS를 처음 공부할 때 추상화, 가상화를 명확하게 구분하지 못했는데 둘은 비슷한 개념이 아니다.<br>

- Abstraction 추상화 : HW를 SW로 관리하기 위함
- Virtualization 가상화 : SW로 struct 된 Process, Memory management에 가상화 기법을 적용해 사용자가 '모든 프로그램이 동시에 처리되는구나.', '프로세스들은 자신만의 메모리가 존재하는구나.' 라는 환상을 느끼며 편리하게 사용하도록 도와준다. 즉, 환상이기 때문에 올바른 이론이 아니다.

가상화란 사용자가 편리함을 느끼도록 환상을 제공해주기 때문에 OS를 추상화 머신이라고도 한다.<br>

### CPU -> Process

HW인 CPU를 추상화하여 Process로 관리한다. linux에서 Process를 **task_struct**라고 명칭하고, BSD나 UNIX에선 proc +u라고 명칭한다.<br>
process가 아주 빠른 속도로 interleave 하게 CPU를 차지하며 운영되는데 여러 CPU가 process가 병렬적으로 동시에 수행되는 것처럼 느끼게 해주는 것을 **CPU 가상화**라고 한다.<br>

cpu를 차지하고 양도하는 기준은 event 발생 또는 time quantum이다.<br>
보통 10ms이 지나거나 I/O, exit()이 발생하면 10ms 채우지 못했어도 다음 process에게 CPU를 양도한다.<br>

1. 양도하기전 자신이 어디까지 수행했는지 기억해야 하므로 memory에 register 값을 저장한다.
2. Schedule() 를 통해 다음 process의 PID를 얻는다.
3. 수행될 process의 상태를 register에 올린다.

이렇게 프로세스간의 양도를 **context switch(문맥 교환)** 이라고 하며 사용자가 a, b, c, d 프로세스 순으로 실행했다 하더라도 문맥 교환이 a, b, c, d 순으로 일어나지 않는다.<br><br>

### RAM -> Memory management

HW인 RAM을 추상화하여 Memory management로 관리한다. task_strcut에는 mm(memory management)라는 멤버 변수가 존재한다. 이 mm 멤버변수는 vm_area_struct 를 가리키고 있다.<br>
이는 각각의 process(tast_struct)가 자신만의 physical memory를 가지고 있는 환상을 느끼게 하는데 이를 **메모리 가상화**라고 한다.<br>

여러 프로세스 중 memory pointer를 출력했을 때 같은 값이 나온다면 '같은 부분을 사용하고 있구나.'라고 생각할 수 있지만 그렇지 않다.<br>
프로세스는 자신만의 physical memory를 갖고 있는 것이 아니라 **자신만의 가상 주소 공간**을 가지고 있다. 즉, 같은 값이라 해도 자신만의 가상 주소 공간의 값이기 때문에 별개이다.<br>
실제 메모리와 mapping 하기 위한 가상 주소 공간은 모든 프로세스가 동일하게 가지고 있기 때문에 같은 주고 값이 출력될 수 있다. 하지만 그 값이 실제 physical memory를 가리키고 있는 부분은 다르다.<br>

CPU가 64비트라는 것은 register의 크기가 64비트라는 의미이다. 현대 OS에선 48비트만 사용한다.<br>
register의 크기가 32비트이면 2^32 = 약 4GB의 가상 주소 공간을 갖는다. 즉, 4GB의 가상 주소로 실제 physical memory를 가리킬 수 있다는 뜻이다.<br>
가상 주소를 실제 주소로 바꿔주는 것을 예전에는 OS가 수행했는데 너무 느렸고, 지금은 HW인 cpu의 mmu(memory management unit)이 수행한다.<br>

그럼 실제 메모리만 사용하니깐 실제 주소를 사용하면 되지, 굳이 가상 주소를 사용하는 이유가 궁금해진다.<br>
실제 주소를 사용하면 user program 마다 주소가 모두 다르게 되며, 실제 주소는 사용자가 보기 힘들기 때문에 작성하기도 힘들다.<br>
즉, mmu가 가상 주소를 실제 주소로 알아서 바꿔주기 때문에 user는 같은 가상 주소로 쉽게 예측이 가능하다.<br><br>

## 가상 메모리 vs 가상 메모리 주소 공간

Disk에 존재하는 것을 프로그램, 실행시키면 memory로 load 되는 것을 프로세스라고 하는데 데이터 구조가 memory 상에 존재해야 실행가능한 상태이다.<br>
프로세스는 메모리 내 데이터 구조를 보관하고 load, store 등 다양한 instruction을 fetch하여 접근한다.<br>

### 가상 메모리

가상 메모리는 실제 physical memory와 swap 영역을 말한다. swap 영역은 Disk에 존재하기 때문에 프로그램 전체가 Disk에 있다는 것은 실행 불가능한 상태를 의미한다.<br>
Disk내 file system은 /.../.../... 처럼 계층구조이므로 속도가 느리다. 느린 속도를 해결하기 위해 swap 영역은 Disk에서 linked list 구조로 되어있다.

### 가상 메모리 주소 공간

위에서 말했듯이 레지스터 크기에 비례하는, 레지스터의 크기에 따라 실제 physical memory를 가리킬 수 있는 크기를 말한다.<br>
레지스터가 32비트이면 2^32 크기의 가상 주소 공간을 갖고, 2^32 만큼의 실제 physical memory를 가리킬 수 있다는 뜻이다.<br> 
Linux 기준으로 2^32 = 약 4GB 크기의 가상 주소 공간에서 1GB는 kernel이 사용한다. 나머지 3GB 중 일부를 제외하고는 user가 사용하는 공간이다.<br>

사용자가 사용하는 약 3GB는 code, data, stack, heap 영역으로 나눠진다.<br>

- code : 프로세스가 실행할 코드와 매크로 상수가 기계어로 저장. 컴파일 단계에서 결정되고 중간에 코드를 바꿀 수 없게 READ-ONLY 로 지정
- data : 전역변수, static 변수가 저장되는 공간. 실행 중간 전역변수는 변경이 가능하므로 READ-WRITE 로 지정
- stack : 런타임 시 함수 내의 지역변수, 매개변수, 리턴값, 재귀함수에서 사용된 돌아올 주소 등이 할당. 함수 종료시 모두 삭제. 기록하고 종료되는 매커니즘은 LIFO 방식. 컴파일 시 크기가 지정되기 때문에 stack 영역을 초과하면 stack overflow 에러가 발생.
- heap : 런타임에 결정되며 malloc, calloc으로 사용. 사용하고 난 다음 해제해야 하는데 해제하지 않을 경우 memory leak 발생. stack보다 할당할 수 있는 공간이 크지만 포인터로 접근해야 하기 때문에 속도가 느림.<br><br>

## Concurrency

많은 프로세스를 번갈아가며 수행하기 때문에 순서가 문제되기도 함. 또한 번갈아 가면서 사용하기 때문에 **공유 데이터**가 존재한다.<br>
여러 pthread를 생성해 전역 변수에 접근하게 한다면 해당 전역 변수를 공유 데이터라고 하며, 생성된 순으로 접근한다는 보장은 없다.<br>
이를 해결하기 위해  락, 세마 포어, 모니터 등의 생겼다.<br>

## Persistence

휘발성인 DRAM에 데이터를 저장할 수 없기 때문에 전원이 꺼지고 시스템이 손상되더라도 데이터를 저장하기 위한 HW, SW가 필요하다.<br>

- HW : 하드 드라이브, SSD(Solid-State Drive) 같은 I/O 장치
- SW : 파일 시스템은 사용자가 생성한 모든 파일을 안전하게 저장
 
OS는 CPU, RAM에 대한 가상화를 제공하지만 Disk에 대한 가상화를 제공하지 않는데 안전한 저장이 우선이기 때문이다.<br>
Disk를 추상화한 File system을 통해 데이터를 오랫동안 저장하고 다른 사용자와 공유할 수 있게 된다.<br>
**tmp** directly 내에 있는 모든 파일은 모든 user가 공유 가능한데 delete는 file 소유자만 가능하다.<br>

- File system(files_struct -> file struct -> inode struct)

















