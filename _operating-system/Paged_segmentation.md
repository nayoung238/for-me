## Paged segmentation

- Paging은 내부 단편화가 발생하지만, 작업 크기가 동일해 메모리를 효율적으로 사용가능
- Segmentation은 외부 단편화가 발생하지만, 가변적인 데이터 구조와 모듈 처리, 공유와 보호의 지원이 편리
- 2가지 기법의 장단점을 취합한 방법이 Paged Segmentation

<br>

## 하드웨어 구조

- 멀틱스 시스템을 기준으로 설명
- 논리적 주소는 18비트의 세그먼트 번호와 16비트의 Segment offset으로 구성
- 16비트 Segment offset은 6비트의 페이지 번호와 10비트의 페이지 offset으로 구성
- 페이지 번호는 해당 세그먼트의 페이지 테이블에서 인덱스로 사용
<br>

![jpeg](/_operating-system/_img/paged_segmentation.jpeg)

- 18비트 세그먼트 번호와 세그먼트 테이블 기준 레지스터(STBR)로 **세그먼트 테이블**을 확인
- 각 세그먼트는 자신의 페이지 테이블이 존재
- Segment offset에서 6비트를 사용하는 페이지 번호로 PTE를 찾고
- PTE에서 물리 메모리의 Page frame 번호와 Segment offset에서 10비트를 차지하는 페이지 오프셋을 결합해 물리 메모리로 접근
