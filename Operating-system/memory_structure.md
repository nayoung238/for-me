## 메모리 영역

![png](/Operating-system/_img/memory_structure.png)

<br>

### Code

- 프로그램을 시작할 때 프로세서가 디스크에서 읽어 **컴파일한 프로그램**을 저장
- **Compile 시 메모리 할당되어 크기가 고정**
- 중간에 코드를 바꿀 수 없게 READ-Only

<br>

### Data

- 전역변수, static 변수 같은 고정된 데이터가 할당되는 공간
- 전역변수, static 값을 참조한 코드는 컴파일 후 Data 영역의 주소 값을 가르키도록 변경
- **Compile 시 메모리 할당되어 크기가 고정**
- 실행도중 전역변수의 값이 변경될 수 있기 때문에 Read-Write
- 초기화되지 않는 전역 변수는 **BSS(Block Started by Symbol)** 에 할당
    - C, CPP는 0으로 초기화하고 JAVA는 error

<br>

### Stack

- 함수내의 지역변수, 매개변수, 리턴값 등이 저장되고 함수가 종료되면 해당 데이터도 제거
- LIFO 방식
- **Runtime 시 메모리가 할당되어 가변적**
- Heap 영역이 있는 방향으로 추가 할당
- stack이 Heap 영역 침범시 Stack overflow

<br>

### Heap

- 런타임에 결정되는 영역으로 malloc(cpp), new 객체(java) 와 같이 동적할당으로 객체를 생성
- **Runtime 시 메모리가 할당되어 가변적**
- Stack 영역이 있는 방향으로 추가 할당
- Heap이 Stack 영역 침범시 Heap overflow
- **포인터로 메모리 영역을 접근**하기 때문에 다른 자료구조에 비해 데이터 읽고 쓰는게 느림