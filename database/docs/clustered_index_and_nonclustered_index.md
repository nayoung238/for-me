## Clustered Index

책의 목차처럼 대용량 데이터에서 원하는 데이터는 빠르게 조회하기 위해 Index 사용
- **Cardinality(유니크한 원소 개수)가 높거나 
- Selectivity(선택도, 분포도)가 1에 가까운** 요소가 인덱스로 설정되어야 검색 성능을 높일 수 있음

클러스터링이란 여러 개를 하나로 묶는다는 의미
- 클러스터드 인덱스는 **테이블의 PK에 대해서만 적용**
- 즉, **PK 값이 비슷한 레코드끼리 묶어서 저장**하는 것을 클러스터드 인덱스(클러스터링 인덱스)라고 표현
- **InnoDB은 클러스터링 인덱스로 저장**되므로 PK 기반의 검색이 매우 빠르지만,
- 레코드 저장이나 PK 변경은 정렬된 상태를 유지하기 위해 읽기 쿼리보다 상대적으로 느림
<br>

![png](/database/img/clustered_index.png)
> 출처: Real MySQL 8.0

- Secondary Index B-Tree의 리프 노드에는 Row ID가 저장되어 있지만,
- **클러스터링 인덱스의 리프 노드에는 레코드의 모든 데이터 저장**

<br>

## 페이지 분할

![png](/database/img/page_split.png)
> 출처: Real MySQL 8.0

클러스터링 인덱스는 PK 값으로 테이블의 레코드가 정렬된다. 
- 만약 PK를 변경하거나 새로운 PK를 추가한다면 페이지 이동 또는 분할이 발생할 수 있음
- 물론 실제 서비스에서 PK 값이 변경되는 경우가 거의 없다고 함 

![png](/database/img/page_split2.png)

- **새로운 데이터가 중간에 들어갈 때도** 물리적 저장 위치가 바뀌어 위와 같이 **페이지 분할**이 발생할 수 있음
- 즉, 클러스터드 인덱스의 **INSERT**와 **UPDATE** 작업은 부하 큼

<br>

## PK 설정하지 않았다면...

만약 PK를 설정하지 않았다면 InnoDB 스토리지 엔진은 다음 우선순위대로 PK를 대체할 칼럼을 선택한다.

1. PK가 있으면 PK를 클러스터링 키로 선택
2. NOT NULL 옵션의 Unique Index 중에서 첫 번째 인덱스를 클러스터링 키로 선택
3. 자동으로 유니크한 값을 가지도록 증가되는 칼럼을 **내부적으로 추가**한 후 클러스터링 키로 선택

인덱스를 사용하는 이유는 결국 **빠른 검색 속도**를 위함
- 하지만 PK를 설정하지 않아 InnoDB 스토리지 엔진이 내부적으로 칼럼을 만들어 정렬하는 것은 
- 아무 의미 없는 숫자 값으로 인덱스를 사용하기에 인덱스를 사용하는 목적에 벗어남
- 그러므로 적절한 PK를 명시하는 것이 좋음

<br>

## 공간 지역성의 효율이 높아짐

![png](/database/img/space_efficiency.png)

위 테이블은 ```from_user``` 와 ```to_user``` 칼럼으로 복합키를 생성한 결과이다. 
- 두 칼럼을 알파벳 순서로 나열하면 ```from_user``` 칼럼이 빠르기 때문에 ```from_user``` 칼럼을 기준으로 정렬

```from_user``` 가 2인 데이터를 찾는다면
- ```from_user``` 칼럼으로 정렬되었기 때문에 위와 같이 **range scan** 가능
- 만약 해당 데이터들이 같은 페이지에만 있다면 그 페이지만 탐색하면 됨

**공간 지역성은 특정 데이터와 가까운 주소가 순서대로 접근되는 경우**를 의미하는데 
- 한 메모리 주소에 접근할 때 해당 블록을 전부 캐시에 가져와 공간 지역성의 효율을 높임 
- 이렇게 Range Scan이 가능해 **연관된 여러 데이터를 찾을 때 사용**하면 유리

<br>

***

## Non-Clustered Index

Account 테이블에서 전화번호 컬럼으로 Clustered Index를 만들었다면 모든 레코드는 전화번호 값을 기준으로 정렬되어 있다. 
- 그러므로 전화번호 기준으로 탐색할 때는 O(logN) 시간복잡도 발생

Full scan
- 만약 인덱스가 아닌 유저네임 컬럼으로 레코드를 찾는다면 어떻게 될까?
- 전화번호 값으로 정렬되어 있고 유저네임이 인덱스가 아니라서 **Full Index Scan**을 해야 유저네임을 찾을 수 있음
- **Full Scan을 한다는 것은 결국 인덱스를 활용하지 못하는 것**
- username 컬럼 값으로 Non-Clustered Index를 만들면 username 값으로도 O(logN) 시간복잡도로 레코드를 찾을 수 있음

Clustered Index는 리프 노드에 실제 데이터가 존재하며 1개의 특정 컬럼(주로 PK)를 기준으로 정렬하기 때문에 테이블 당 하나만 생성할 수 있다.
- Non-Clustered Index는 특정 컬람을 기준으로 Index page를 독립적으로 생성 (메모리 문제로 이어질 수 있음)
- 원하는 컬럼으로 Non-Clustered Index를 생성할 수 있고 1개의 테이블은 여러 Non-Clustered Index를 가질 수 있음

![png](/database/img/nonclustered_index.png)
> 출처: https://junghu.tistory.com

clustered Index는
- **리프 노드에 레코드의 모든 컬럼을 저장**
- 리프 노드까지 내려가면 바로 데이터를 읽을 수 있음

이와 다르게 Non-Clustered Index는
- **리프노드에 레코드의 물리적 위치인 Row ID를 저장**
- Row ID를 통해 Clustered Index에서 레코드를 다시 찾아야 하므로 조회 시 Non-Clustered Index가 상대적으로 느림
- 하지만 인덱스만으로 쿼리 응답이 가능하면 Clustered Index에 접근하지 않아도 되며 이를 커버링 인덱스라고 함
- 실제 데이터는 독립적이므로 Clustered Index와 다르게 인덱스 페이지를 따로 생성 (메모리 사용률 증가)

<br>

### Row ID

Non-Clustered 인덱스의 리프 노드에는 레코드의 물리적 위치인 Row ID가 저장된다고 했는데 정확히 말하면 스토리지 엔진마다 차이가 있다.

![png](/database/img/MyISAM_data_record.png)

MyISAM이나 MEMORY 테이블은 클러스터링되지 않기 때문에 레코드가 insert될 때 처음 저장된 공간에서 절대 이동하지 않는다. 그러므로 MyISAM 테이블은 리프 노드에 데이터 파일 내의 **위치(Offset)** 을 저장한다.

![png](/database/img/InnoDB_data_record.png)

InnoDB 테이블은 리프 노드에 레코드의 **PK 값**을 저장한다.
- MyISAM이나 MEMORY 테이블은 클러스터링되지 않기 때문에 레코드가 insert될 때 처음 저장된 공간에서 절대 이동하지 않음
- 하지만 InnoDB 테이블은 클러스터링 인덱스를 사용하기 때문에 레코드가 insert될 때마다 데이터 파일 내에서 PK 값을 기준으로 순서 재정렬

만약 InnoDB 테이블의 Non-Clustered Index의 리프 노드에 데이터의 위치를 저장했다면 
- 새로운 레코드가 Insert 될 때마다 변경되는 위치에 대해 Index page에도 수정 작업을 해야 함
- 이런 오버헤드를 제거하기 위해 InnoDB 테이블의 모든 Non-Clustered Index는 레코드의 위치가 아닌 **PK 값을 저장**

<br>

## Reference

- https://sqlhints.com/tag/clustered-index-b-tree-structure/
- https://docs.microsoft.com/ko-kr/sql/relational-databases/indexes/clustered-and-nonclustered-indexes-described?view=sql-server-ver16
- https://itholic.github.io/database-index/
- https://github.com/JaeYeopHan/Interview_Question_for_Beginner/tree/master/Database
- https://www.youtube.com/watch?v=P5SZaTQnVCA&t=1s
- https://yurimkoo.github.io/db/2020/03/14/db-index.html