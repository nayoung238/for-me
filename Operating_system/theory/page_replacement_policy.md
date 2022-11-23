## Page & Page Frame

- Page: 가상 주소 공간을 일정한 크기(4KB)로 분할한 단위
- Page Frame: 메모리를 일정한 크기(4KB)로 분할한 단위
- Page와 Page Frame의 크기가 4KB로 같고, Page의 데이터를 Page frame에 올려 실행

<br>

## Paging

- 가상 주소 공간에 있는 특정 Page를 실행하려면 해당 Page에 있는 내용을 Main memory의 비어있는 Page frame(free frame)에 올린다. 이 방식을 Paging 기법이라고 한다.
- Main memory에 존재하는 Page table에서 특정 Page가 어디있는지 파악
  - P(present bit)가 1이면 해당 Page가 Memory에 존재
  - P(present bit)가 0이면 Swap 영역에 존재있으니 Swap 영역에서 가져옴 (page fault)
- Page table에서 해당 Page의 가상주소를 찾고 **MMU(memory management unit)**의 도움을 받아 가상주소를 물리주소로 변경
- 물리주소로 Main memory에서 해당 Page의 내용이 있는 Page frame을 찾아 실행

<br>

## Page Replacement Policy

- Page fault: Data가 Main memory의 Page frame에 적재되지 않고 Swap 영역에 존재해 실행할 수 없는 상태
- 해당 Data를 Main memory에 올리기 위해서 **Free page frame**이 필요
- 만약 Free page frame이 없다면 Page frame 중 1개를 Swap 영역으로 보내야함
- 이때 out 되는 Page를 **Victim page**라고 하며 **수정되지 않은 Page를 내보내는 것이 유리**
- 수정된 것을 내보낼 경우 하드디스크에서 수정 과정을 거쳐야하기 때문
- 이러한 경우를 모두 고려하며 교체를 진행하며 주로 **LRU(Least Recently Used)** algorithm 사용
<br>

- 교체 정책은 교체되는 페이지가 가까운 미래에 참조될 가능성이 가장 적어야 함
- 지역성의 원리로 최근 참조와 가까운 미래의 참조 패턴이 높은 상관관계
- 지역성: 기억 장치 내의 정보를 균일하게 접근하는 것이 아니라 **한 순간에 특정부분을 집중적으로 참조**하는 특성
- 시간 지역성: 최근에 참조된 주소의 내용이 곧 다시 참조되는 특성
- 공간 지역성: 실제 프로그램이 참조된 주소와 인접한 주소의 내용이 다시 참조되는 특성
<br>

- 대부분 교체 정책은 과거를 근거로 미래를 예측
- LRU(Least Recently Used): 최근에 참조되지 않은 Page를 out
- LFU(Least Frequently Used): 최근에 자주 참조되지 않은 Page를 out
- Clock(NUR-not used recently): 최근에 참조 패턴, read, write가 없던 Page를 out
<br>

- Global 교체: 메모리에 적재된 모든 프로세스 page를 교체
- Local 교체: 각 프로세스가 자신에게 할당된 Frame 중 교체될 것을 선택
- 다중 프로그래밍의 경우 Main memory에 다양한 프로세스가 존재하므로 Global 교체가 더 효율적이라고 하는데 ~~확실하지 않음~~

<br>

## LRU

- Least Recently Used
- **가장 오랫동안 참조되지 않은** 주기억장치 상의 Page 교체
- 지역성에 따라 오랫동안 참조되지 않은 페이지는 가까운 미래에 참조될 가능성이 적다고 판단
- 즉, 참조에 대한 기록을 해야하며 Overhead가 아주 큼

<br>

## FIFO

- Round-robin 처럼 메모리에 할당된 Frame을 순환버퍼처럼 다룸
- 메모리에 가장 오래 있었던 Page 교체
- **가장 오래있었지만 계속 이용되는 코드라면 문제**가 되며, 이처럼 Frame을 늘린다해서 Fault가 줄어들지 않는 것을 FIFO abnormally 라고 함

<br>

## Clock

![png](/Operating_system/_img/clock.png)

- Use bit 사용하며 해당 Page가 참조될 때마다 Use bit 1로 설정
- 1개의 pointer가 첫 교체 후보를 가르킴
- 한 페이지 교체 시점에 포인터가 가리키는 버퍼부터 시작해 **Use bit 가 0인 frame을 찾음**
- 찾는 과정에서 Use bit가 1이라면 0으로 바꿔주고 다음 frame으로 넘어감