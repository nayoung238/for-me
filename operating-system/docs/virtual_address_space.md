## 가상 주소 공간

OS (Operating System) 란 컴퓨터 하드웨어를 관리하는 소프트웨어이다. **CPU**를 **프로세스**로, **RAM**을 **Memory management**로, Disk를 file system으로 추상화해 관리한다.
이 중 프로세스와 memory management 는 가상화 기법으로 사용자에게 환상을 주는데 해당 포스트는 **memory management 가 주는 환상인 메모리 가상화**에 대해 작성했다.

<br>

프로세스는 자신만의 가상 주소 공간을 갖기 때문에 **각 프로세스가 자신만의 physical memory를 갖는 것처럼** 보이게 하는 환상을 주는데 이것을 메모리 가상화라고 한다.

64비트 CPU 란 **레지스터의 크기가 64 비트**로 **2^64 크기의 가상 주소 공간을 갖는다.** 하지만 현대 OS 에선 2^48 크기만 사용하고, 아래 설명은 레지스터가 32비트인 2^32 = 4GB 의 가상 주소 공간을 기준으로 설명한다. 32비트 시스템에서 2^32 = 약 4GB 의 가상 주소 공간을 갖는다는 것은 **4GB 만큼의 메모리 주소를 표현할 수 있다**는 의미이다.

프로세스가 표현하는 주소는 물리 메모리를 가리키는 물리적 주소가 아니라 **가상의 논리 주소**이다. 프로세스의 논리적 주소에 대응하는 물리적 주소를 알아야 프로세서가 프로세스를 실행할 수 있다. 즉, 실제 물리 메모리를 참조하려고 할 때 하드웨어 장치인 **MMU(Memory Management Unit)** 가 가상 메모리 주소를 실제 메모리 주소로 변경해 실제 메모리에 접근하고, 이렇게 프로그램에 사용된 구성 요소의 실제 값 또는 프로퍼티를 결정짓는 행위를 **Binding**이라고 한다.

<br>

## Fragmentation

![png](/operating-system/_img/all_allocation.png)

프로그램 전체를 메모리에 올린다면 당장 사용하지 않는 부분도 메모리를 차지하고 있어 정작 실행해야 할 다른 프로그램이 메모리를 차지하지 못하는 상황이 발생할 수도 있다. 위와 같이 동작한다면 **외부 단편화(External Fragmentation), 내부 단편화(Internal Fragmentation)** 문제가 발생한다.
이 문제를 가상 주소 공간을 통해 어느정도 해결 수 있다.

### 고정 분할: 내부 단편화 발생

![png](/operating-system/_img/internal_fragmentation1.png)

메모리를 고정된 크기로 분할 후 크기에 맞는 프로세스를 선택한다. 프로세스는 크기에 맞는 대기 큐에 들어가 기다린다. 이때 **메모리의 고정 크기보다 프로세스의 크기가 작으면 내부 단편화가 발생**한다. 또한 특정 큐에만 작업이 들어온다면 다른 메모리 부분은 사용되지 않는 문제도 발생한다.

![png](/operating-system/_img/internal_fragmentation2.png)

이를 해결하기 위해 모든 작업을 하나의 대기 큐에 넣고 작업에 맞는 공간이 생기면 메모리에 올린다. 하지만 작업의 크기보다 같거나 큰 **연속된 공간**이 나올 때까지 기다려하고, 여전히 내부 단편화가 발생한다.


### 가변 분할: 외부 단편화 발생

![png](/operating-system/_img/external_fragmentation.png)

메모리를 고정 분할하면 메모리의 고정된 크기와 작업의 크기의 차이에서 낭비가 생기기 때문에 고정된 경계를 없애고 **프로세스가 필요한 만큼 메모리를 할당**하는 것을 가변 분할이라고 한다. task2가 끝나면 task4가 들어간다. 이전 작업과 현재 작업의 크기 차이에서 사용하지 않는 메모리가 발생하며 이를 외부 단편화라고 한다.

50KB 크기의 프로세스가 메모리를 요구한다. 만약 30KB와 26KB 공간이 비어있지만, 두 공간은 연속적이지 않다면 해당 프로세스에게 제공할 수 없다. 이를 해결하는 방법엔 **인접한 빈 공간을 합치는 메모리 통합(coalescing) 방법**이 있다. 인접한 빈 공간만 합칠 수 있기 때문에 이를 합쳐도 요구하는 공간보다 작다면 제공할 수 없다.

![png](/operating-system/_img/memory_compaction.png)


다른 방법으로는 **메모리의 내용을 적절히 움직여 빈 공간을 생성하는 메모리 압축(compaction) 방법**이 있다. 작업 3과 작업 4를 이동해 빈 공간을 합쳤다. 메모리 통합 방법보다는 큰 공간을 만들 수 있다. 하지만 다른 주소로 이동한 작업 3과 작업 4에 대해서 모든 내부 주소를 대체해야만 새로운 위치에서 계속 수행 가능하다. 또한 압축하는 동안 시스템은 다른 작업을 할 수 없다. 

<br>

## Page와 Page frame

메모리와 프로세스 사이에서 발생하는 외부 단편화와 내부 단편화를 해결하기 위해 Page와 Page frame 단위를 적용한다.

![png](/operating-system/_img/memory_mapping1.png)

4KB 단위로 가상 주소 공간을 물리 메모리에 할당해 필요없는 부분까지 할당되는 낭비를 막는데 이렇게 **가상 주소 공간을 4KB 단위로 나눈 블록을 Page**라고 한다. 물리 메모리도 동일한 이유로 메모리가 낭비되는 것을 막기 위해 4KB 단위로 나눠 관리하는데 이를 **Page Frame**이라고 한다.

가상 주소 공간의 각 Page가 실행되기 위해 물리 메모리의 Page frame 단위로 할당하는 것을 **Paging**이라고 한다. 이렇게 프로세스의 일부분만 메모리에 올리기 때문에 메모리를 효율적으로 사용하는 점도 개선되었고 CPU의 utilization도 증가한다.

I/O 작업은 I/O controller가 수행하기 때문에 **I/O 작업을 하는 프로세스는 CPU를 차지하지 않는다**. 많은 프로세스의 일부분만 메모리에 올리게 되면 I/O 작업이 아닌 연산을 필요로 하는 프로세스를 선택할 수 있는 경우의 수가 증가하기 때문에 CPU 사용률을 높일 수 있다. 이는 현재 메모리 가상화 정책 중 하나이다.

<br>

## page table

![png](/operating-system/_img/memory_mapping2.png)

같은 프로그램에서 생성된 여러 page가 물리 메모리에 할당될 때 **연속적으로 배치된다는 보장이 없기**때문에 각 page가 어디에 있는지 파악해야 한다. 또한 물리 메모리는 여러 프로세스가 함께 사용하기 때문에 각 page가 어느 page frame에 있는지 파악하기 위해 page table가 필요하다.

프로세스마다 자신만의 page table을 갖고 있으며 메인 메모리의 커널 영역에 page frame단위로 생성되므로 **분리되지 않고 연속적인 형태**이다. 1개의 page frame으로 부족할 경우 2개를 사용한다.

![png](/operating-system/_img/page_table2.png)

Page table은 많은 PTE(Page Table Entry)로 구성되는데 PTE의 Valid bit 인 P bit로 찾고자 하는 Page가 물리 메모리에 있는지 디스크에 있는지 확인한다. P bit가 1 인 경우 물리 메모리에 존재하고 0 인 경우 디스크에 있음을 의미한다. page가 디스크에 있는 경우 실행될 수 없는 상태이므로 물리 메모리로 올려야 한다.

가상 주소 공간의 개념없이 물리 메모리만 사용한다면 물리 메모리에 올라간 process 간의 침범을 막기위해 경계를 계속 확인해야 한다. 하지만 **가상 주소 공간의 Page단위**와 **물리 메모리의 Page frame단위**를 **서로 매칭**하기 때문에 경계지점을 따로 확인하지 않아도 되는 편리함이 있다.
<br>

## PTE

![png](/operating-system/_img/page_table_entry.png)

- PFN : Page Frame Number
- present (Absent) : valid bit로 해당 page가 디스크에 있는지, 물리 메모리에 있는지 파악하는 비트
- protection : 페이지 권한을 파악하는 비트로 0 은 read-only, 1 은 read-write
- reference : 해당 page가 참조된 적 있는지 파악하는 비트
- caching : 해당 page를 캐싱할지 선택
- dirty : modify bit 로 해당 page의 내용이 변경되었는지 판단하는 비트로 문맥 교환전 페이지 내용이 변경되었음을 알려 기록하게 함
<br>

4GB 가상 주소 공간을 표현하기 위해선 2^20 = 1,048,576개의 PTE가 필요하다.
> 4GB / 4KB (page 1개 크기) = 2^32 / 2^12 = 2^ 20<br>

2^20 개를 모두 표현하려고 하면 page table의 크기가 너무 커져 많은 page frame을 사용해야 한다. 이를 해결하기 위해 Multi-Level Page Tables 을 적용한다.