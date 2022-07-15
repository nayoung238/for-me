## Paging

- External fragmentation: 프로세스의 크기와 고정된 사이즈 차이에서 생기는 낭비
- Internal fragmentation: 프로세스가 끝나고 생긴 빈 공간과 새로 들어갈 프로세스 사이즈 차이에서 생기는 낭비

고정 분할은 내부 단편화가, 가변 분할은 외부 단편화가 생겨 메모리를 효율적으로 이용하지 못한다.<br>

Paging은 작업 크기가 동일한 페이지로 나눠 처리하는 방법으로 프로세스는 Page 단위, 메인 메모리는 Page Frame 단위인 동일한 크기(4KB)로 나눠 페이지를 프레임에 적재하는 방식이다. 이 방식은 외부 단편화로 인한 메모리 낭비를 없애고 4KB 미만의 내부 단편화로 인한 낭비만 존재한다.<br>

프로세스를 실행시키기 위해 Page를 Page Frame에 올리는데 4KB로 크기가 같기 때문에 외부 단편화가 생기지 않는다. 하지만 프로세스를 4KB로 분할하면 마지막 부분이 4KB이하일 경우 내부 단편화가 생기지만 4KB미만의 낭비이다.<br>

Page, Page Frame 단위의 크기를 더 줄이면 내부 단편화를 줄일 수 있겠지만, Page가 어느 Frame에 할당되었는지 관리하는 Page Table이 부담이 커진다.

### 인위적 영속성

같은 프로세스의 여러 Page가 메모리로 올라갈 경우 해당되는 Page Frame은 연속적이라고 보장할 수 없다. 연속적인 것보다 메모리의 빈공간을 채워나가는 것이 더 효율적이며 흩어진 Page Frame은 Page table을 통해 찾을 수 있다.

<br>

## Page table

- 프로세스마다 Page table을 가지고 있으며 메모리에 존재
- Page table로 각 Page가 어느 Page frame에 할당되었는지 체크 (Frame 번호를 가지고 있음)
- 그렇기 떄문에 한 프로세스의 여러 Page는 연속적인 Page frame에 할당 될 필요없음
- 한 프로세스의 Page table의 크기가 커서 여러 Page frame이 필요하다면 이는 연속적으로 할당됨

<br>

## Address translation

![png](/Operating_system/_img/address_translation.png)

- Logical address(논리 주소): 프로그램의 시작위치로부터 상대적인 값으로 MMU가 논리주소를 물리주소로 변환함 
  - MMU의 도움을 받아 주소를 변경하는 것을 **Dynamic relocation**이라고 함
- 프로그램 안에서 각 논리주소는 Page 번호와 offset으로 구성
- Page 번호로 Page table의 해당 PTE 를 찾고 (Page 번호가 Page table의 Index로 사용)
- PTE의 Page frame 번호와 offset으로 Main memory에 접근
- 가상주소를 물리주소로 바꿀때마다 위 과정이 일어나는데 OS가 처리하면 느리므로 MMU가 처리

<br>

## Offset

![png](/Operating_system/_img/offset.png)

- Page의 크기는 offset에 비례
- offset 12비트이면 Page 크기 2^12 = 4096 = 4KB
- 즉, Page 크기가 4KB이면 Offset은 12bit