# Virtual Address Space

## Single programming

초기 운영체제는 물리 메모리에 1개의 실행중인 프로그램이 존재하는 single programming system이었다.<br>
1개의 프로세스만 물리 메모리에 올라가니 배치는 신경 쓸 필요없고, 크기에만 신경썼다.<br>
물리 메모리보다 프로세스 크기가 더 클경우 추가 메모리를 구입하거나, overlay 기법을 사용하여 공통으로 같이쓰는 Data부분을 겹치게 하는 방식으로 해결했다.<br>

## Multi programming

물리 메모리에 여러 프로세스가 올려 cpu의 utilization을 개선했다.<br>
처음으로 **time sharing**이 적용되었지만 기존 프로세스 상태르 disk에 저장하였기에 느리다는 단점이 있었다.<br>
이를 해결하기 위해 지금은 PCB에 프로세스의 모든 상태를 저장하고 memory에 올리는 가상주소공간을 적용한다.<br>

## Virtual Address Space

![png](/Operating_system/_img/virtual_address_space.png)

>출처: https://slenderankle.tistory.com/189

주소공간이란 운영체제가 실행 중인 프로세스에게 자신만의 메모리를 갖고있다고 착각하게 만드는 개념이다.<br>
하나의 응용프로그램은 여러 프로세스가 될 수 있고, 각각의 프로세스는 자신만의 가상주소공간을 부여받는다.<br>
register 32bit 기준으로 0x0000_0000 부터 0xffff_ffff까지 약 4GB의 가상주소공간을 갖는다.<br>
즉, 가상주소공간은 register 크기에 비례한다.<br>

> 현대 os는 64bit이지만 48bit만을 사용하고 있으며(48bit만 써도 충분하기 때문에)<br>
> 설명은 register 32bit 기준으로 한다. <br>

가상주소공간 4GB는 리눅스에선 Kernel Space(1GB) + User Space(3GB)로 나눠진다.<br>
윈도우는 2GB kernel space + 2GB user space로 나눠진다.<br>

## User Space

text, stack, data(heap)로 구성되며, 실행 프로그램의 모든 메모리 상태를 포함하고 있다.<br>
프로세스간의 공유가 불가능한 공간이다.<br>

- text : 작성된 코드이며 읽기 전용이다. 메모리로 load될 때 가장 먼저되는 부분이다.
- data : 전역 변수와 static 변수가 저장된다. 프로그램 시작과 함꼐 할당되며, 프로그램 종료시 소멸한다.
- stack : 함수가 시작될떄 그 안의 지역변수와 매개변수가 저장된다. 함수 종료시 소멸된다.

## Kernel Space
각각의 프로세스의 코드, 데이터(user space)가 바뀌는 위험을 막기위해, kernel를 통해 프로세스간의 공유를 한다.<br>
프로세스는 4GB 공간을 한번에 메모리로 load하는게 아니라 일부만 load한다.<br>


## page, page frame, disk block

- page : 4GB 가상주소공간을 4KB로 나눈 하나의 블록
- page frame : 물리 메모리를 4KB로 나눈 하나의 블록
- disk block : disk를 4KB로 나눈 하나의 블록

## test.c 파일이 memory에 load 되는 과정

![png](/Operating_system/_img/memory_load.png)

test.c 파일은 disk에 존재하며 compile하면 test.exe가 생성되어 메모리로 load된다.<br>
파일은 자신만의 inode를 갖고 있으며 inode는 여러개의 disk block를 포인터로 가르킨다.<br>
inode가 2개의 disk block을 갖고 있다면 해당파일의 크기는 8KB이다.<br>
disk block은 text 또는 data영역을 가리키는데 보통 text가 메모리로 먼저 load된다.<br>

disk block의 실제 주소를 갖고 있는 page table도 memory에 존재하며 보통 1개의 page frame을 차지한다..<br>
> 위 그림에선 page table을 page + page frame으로 표현했는데, 실제로 page 부분은 존재하지 않는다.<br>

1개의 page가 실행하고자 할때 MMU(Memory Management Unit)가 해당 process의 page table를 통해 물리주소로 mapping 된다.<br>
> MMU(Memory Management Unit)란<br>CPU가 메모리에 접근하는 것을 관리하는 컴퓨터 하드웨어 부품으로, 가상 메모리주소를 실제 메모리주소로 변환한다. 라고 wikipedia에 정의되어있다.<br>

이때 control bit를 확인해야한다.<br>
- D : write bit로 쓴 적이 있는지를 의미한다.
- R : read bit로 읽은 것이 있는지를 의미한다.
- P : present bit로 1일 경우 실제 물리 메모리에서, 0일 경우 swap영역에서 block를 찾아 메모리에 올린다.

### Page Fault
swap영역에서 메모리로 가져오는 것을 말하며 memory fault라고 하기도 한다.<br>
swap영역에 있는 것은 disk block이며, 메모리로 load될때 page frame이 된다.<br>
page fault도 I/O 작업이다.<br>\

## 서로 다른 프로세스의 주소가 같은 경우

아래와 같이 2개의 프로세스를 실행시키고 주소를 출력했을 때 같은 경우가 있다.

![png](/Operating_system/_img/same_address(1).png)<br>
![png](/Operating_system/_img/same_address(2).png)<br>

서로 다른 프로세스가 t2처럼 같은 가상 주소를 가질수 있지만, 실제 주소는 pf5, pf4처럼 달라 다른 공간을 사용한다.<br>
이렇게 같은 가상 주소를 갖는 것처럼 보이게 하여 프로세스는 자신만의 메모리를 사용한다고 착각하게 만들지만<br>
실제로는 다른 공간에서 실행되는 것이 메모리 가상화 기법이다.<br>

