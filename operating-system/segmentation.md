## Segmentation

- 프로그램을 구성하는 서브루틴, 프로시저, 함수나 모듈 등으로 세그먼트를 구성
- 프로세스는 여러 Segment로 구성되며 각 세그먼트의 크기는 다르고 변할 수 있음
- 즉, Segmentation이란 컴파일러가 원시 프로그램을 실행 프로그램으로 자동 변환하면서 서브루틴과 프로시저, 함수, 모듈 등 각기 크기가 다른 세그먼트로 구성
<br>

- Segment는 **연관된 기능**을 수행하는 하나의 모듈 프로그램을 다루며 Code, Data, Stack 그리고 Heap 을 Segment라고 함
  - 정적 세그먼트: Code, Data(+BSS)는 컴파일 시 사이즈가 정해지며 변경이 불가능
  - 동적 세그먼트: Stack, Heap은 Runtime 과정에서 메모리 할당이 이루어짐
- 각 세그먼트는 메모리의 연속된 위치에서 구성하되 서로 인접할 필요는 없음
- 현재 사용하는 방식이 **Paged Segmentation**으로 **먼저 Segmentation을 수행하고 각 Segment별로 Paging 수행**
- 즉, Code, Data, Stack 그리고 Heap을 나누는 방식이 Segmentation 이므로 모두 크기가 다르고 모든 Segment가 메모리 내에서 Paging 방식으로 처리

<br>

## 보안 및 공유

- Segment는 의미 단위로 나눠지기 때문에 공유와 보안에 있어서 Paging 기법보다 훨씬 효과적
- Protection: 각 Segment 별로 Protection bit가 존재해 의미 단위별로 Read/Write 권한 부여
- Sharing: 의미 단위로 공유하는 것이 훨씬 효과적

<br>

## 외부 단편화 발생

- 가변 분할이므로 Internal fragmentation 발생하지 않음
- 가변 분할이므로 Swapping이 힘듦
- 프로세스마다 크기가 다르므로 Segment 크기도 달라 External fragmentation 발생
- 평균 Segment 크기가 작을수록 External fragmentation 적음
- Segmentation은 동적 배치 알고리즘이므로 메모리 압축이 가능

<br>

## 하드웨어 구조

![jpeg](/operating-system/_img/segmentation_address_translation.jpeg)

> 출처: 운영체제 - 구현회

- 왼쪽 n비트를 세그먼트 번호로, 나머지 오른쪽 비트를 Offset으로 사용
- 왼쪽 비트로 세그먼트 번호를 추출해 Segment table에서 Segment의 물리 주소를 찾음
  - 세그먼트 번호로 Code, Data, Stack 그리고 Heap 을 구분
- 나머지 오른쪽 비트인 Offset으로 세그먼트의 길이와 비교
  - 만약 offset이 세그먼트의 길이보다 크거나 같다면 해당 주소는 유효하지 않음 -> Trap (addressing error, Segmentation fault)
- 접근하고자 하는 물리주소는 세그먼트의 시작물리주소와 offset의 합
- STBR(Segment Table Base Register) 와 STLR(Segment Table Length Register) 2개의 Register는 프로세스를 디스패치할 때 적재되어 논리적 주소를 검사