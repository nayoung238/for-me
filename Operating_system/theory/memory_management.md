## 메모리 관리

- 여러 프로세스를 수용하기 위해 주기억장치를 동적 분할하는 메모리 관리 작업이 필요하며 **Paging**과 **Segmentation** 기법 존재
- 현재 Paging과 Segmentation 기법을 합쳐서 사용
- External fragmentation: 프로세스의 크기와 **고정된 사이즈 차이**에서 생기는 낭비
- Internal fragmentation: 프로세스가 끝나고 생긴 **빈 공간과 새로 들어갈 프로세스 사이즈 차이**에서 생기는 낭비

<br>

## 메모리 고정 분할

- Internal fragmentation 발생
- 절대 번역: 고정된 크기에 따른 **여러 큐 존재** 
  - 특정 큐에는 계속 프로세스가 들어와 대기하고 다른 큐에는 들어오지 않는 경우 효율성 떨어짐
- 재배치: 큐 1개 존재
  - 비어있는 메모리를 계속 체크하며 대기 중인 프로세스 중 1개를 선택
  - 절대 번역의 문제를 해결할 수 있지만 여전히 Internal fragmentation 발생

<br>

## 메모리 가변 분할

- External fragmentation 발생
- 특정 프로세스가 끝나면 그만큼의 공간이 생김
- 생긴 공간보다 작거나 같은 크기를 가진 프로세스가 들어가면 됨
- 생긴 공간보다 작은 크기의 프로세스가 들어갈 경우 공간이 남는 External fragmentation 발생
- 이를 Compaction(메모리를 쭉 올림) 또는 Coalescing(생긴 공간의 인접한 것만 붙임) 으로 해결

<br>

## 재배치

- 비어있는 공간 중 어디로 들어갈 것인가?
- First Fit: 비어있는 위쪽부터
- Best Fit: Size가 가장 잘 맞는 곳부터
- Worst Fit: 비어있는 공간이 가장 큰 곳부터
- Next Fit: 현재 Pointer의 밑에 비어있는 곳부
