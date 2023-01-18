# Clustered Index

책의 목차처럼 대용량 데이터에서 원하는 데이터는 빠르게 조회하기 위해 Index를 사용한다. **Cardinality가 높거나 Selectivity(선택도, 분포도)가 낮거나 활용도가 높은** 요소들이 인덱스로 설정되어야 검색 성능을 높일 수 있다.

클러스터링이란 여러 개를 하나로 묶는다는 의미로 사용한다. 클러스터드 인덱스는 **테이블의 PK에 대해서만 적용**된다. 즉, **PK 값이 비슷한 레코드끼리 묶어서 저장**하는 것을 클러스터드 인덱스(클러스터딩 인덱스)라고 표현한다. 

PK 값에 의해 레코드의 위치가 결정되며 이는 PK 변경 시 레코드의 물리적인 저장 위치를 변경해야한다는 의미이다. **InnoDB은 클러스터링 인덱스로 저장**되므로 PK 기반의 검색이 매우 빠르지만, 레코드 저장이나 PK 변경은 상대적으로 느리다.
<br>

> B-Tree 인덱스도 인덱스 값으로 정렬되어있으므로 인덱스로 클러스터링되었다고 생각할 수 있지만, 이는 클러스터링 인덱스라고 하지 않는다. 테이블의 레코드가 PK 값으로 정렬된 경우만 클러스터링 인덱스 또는 클러스터링 테이블이라고 한다. - Real MySQL 8.0

![png](/Database/_img/clustered_index.png)

위 구조는 클러스터링 테이블의 구조이며 B-Tree와 비슷하다. Secondary Index를 위한 B-Tree의 리프 노드에는 RowID가 저장되어있지만, **클러스터링 인덱스의 리프 노드에는 레코드의 모든 칼럼이 저장**되어있다. 즉, 클러스터링 테이블은 그 자체가 하나의 인덱스 구조로 관리된다.

<br>

## 페이지 분할

클러스터링 인덱스는 PK 값으로 테이블의 레코드가 정렬되므로 **PK에 의존**하는 것을 알 수 있다. 만약 PK를 변경하거나 새로운 PK를 추가한다면 페이지 이동 또는 분할이 발생한다.

![png](/Database/_img/page_split.png)

PK 값을 변경하면 PK를 기준으로 다시 재정렬이 일어나 위 그림과 같이 다른 페이지(리프 노드)로 이동해야한다. 물론 실제 서비스에서 PK 값이 변경되는 경우가 거의 없다고 한다. 

![png](/Database/_img/page_split(2).png)

 **새로운 데이터가 중간에 들어갈때도** 물리적 저장 위치가 바뀌어 위와 같이 **페이지 분할**이 일어난다. 클러스터드 인덱스의 **INSERT**와 **UPDATE** 작업은 부하가 크다. 
<br>

만약 PK를 설정하지 않았다면 InnoDB 스토리지 엔진은 다음 우선순위대로 PK를 대체할 칼럼을 선택한다.

1. PK가 있으면 PK를 클러스터링 키로 선택
2. NOT NULL 옵션의 Unique Index 중에서 첫 번째 인덱스를 클러스터링 키로 선택
3. 자동으로 유니크한 값을 가지도록 증가되는 칼럼을 **내부적으로 추가**한 후 클러스터링 키로 선택

인덱스를 사용하는 이유는 결국 **빠른 검색 속도**를 위해서이다. 하지만 PK를 설정하지 않아 InnoDB 스토리지 엔진이 내부적으로 칼럼을 만들어 정렬하는 것은 아무 의미 없는 숫자 값으로 클러스터링되는 것이므로 인덱스를 사용하고자하는 목적에 벗어나게 된다. 그러므로 적절한 PK를 명시하는 것이 좋다.

<br>

## 공간 지역성의 효율이 높아짐

![png](/Database/_img/space_efficiency.png)

위 테이블은 ```from_user``` 와 ```to_user``` 칼럼으로 복합키를 생성한 결과이다. 두 칼럼을 알파벳 순서로 나열하면 ```from_user``` 칼럼이 빠르기 때문에 ```from_user``` 칼럼을 기준으로 정렬된다. 

```from_user``` 가 2인 데이터를 찾는다면 ```from_user``` 칼럼으로 정렬되었기 때문에 위와 같이 **연속**으로 찾을 수 있다. 만약 해당 데이터들이 같은 페이지에만 있다면 그 페이지만 탐색하면 된다. 

**공간 지역성은 특정 데이터와 가까운 주소가 순서대로 접근되는 경우**를 의미하는데 한 메모리 주소에 접근할 때 해당 블록을 전부 캐시에 가져와 공간 지역성의 효율을 높힌다. 이렇게 Range Scan이 가능해 **연관된 여러 데이터를 찾을 때 사용**하면 유리하다.

<br>

# Non-Clustered Index

Account 테이블에서 주민번호 칼럼으로 Clustered Index를 만들었다면 모든 레코드는 주민번호 값을 기준으로 정렬되어있다. 그러므로 주민번호를 기준으로 탐색할 때는 O(logN) 시간복잡도로 찾을 수 있다. 하지만 username 칼럼으로 레코드를 찾는다면 어떻게 될까?

주민번호 값으로 정렬되어있으므로 **Full Index Scan**을 해야 username을 찾을 수 있다. **Full Scan을 한다는 것은 결국 Index를 쓰는 목적이 없어지므로** 해결책이 필요하며 이 해결책이 Non-Clustered Index이다.

Clustered Index는 리프 노드에 실제 데이터가 존재하며 1개의 특정 칼럼(주로 PK)를 기준으로 정렬하기 때문에 테이블 당 하나만 생성할 수 있다. Non-Clustered Index는 실제 데이터를 건드리지 않고 특정 칼람을 기준으로 Index page를 독립적으로 생성한다. 원하는 칼럼으로 Non-Clustered Index를 생성할 수 있으므로 1개의 테이블은 여러 Non-Clustered Index를 가질 수 있다.

username 컬럼 값으로 Non-Clustered Index를 만들면 username 값으로도 O(logN) 시간복잡도만으로 레코드를 찾을 수 있다.

![png](/Database/_img/nonclustered_index.png)

Clustered Index는 **리프 노드에 레코드의 모든 칼럼을 저장**하므로 리프 노드까지 내려가면 바로 데이터를 읽을 수 있다. 이와 다르게 Non-Clustered Index는 **리프노드에 레코드의 물리적 위치인 RowID를 저장**한다. RowID를 통해 데이터 파일에서 레코드를 다시 한번 찾아야 하므로 조회 시 Non-Clustered Index가 상대적으로 느리다.

즉, Non-Clustered Index와 실제 데이터는 독립적이므로 Clustered Index와 다르게 인덱스 페이지를 따로 만들어야 한다. 또한 리프 노드에는 레코드의 물리적 위치인 RowID가 저장된다고 했는데 정확히 말하면 스토리지 엔진마다 차이가 있다.

![png](/Database/_img/MyISAM_data_record.png)

MyISAM이나 MEMORY 테이블은 클러스터링되지 않기 때문에 레코드가 INSERT될 때 처음 저장된 공간에서 절대 이동하지 않는다. 그러므로 MyISAM 테이블은 리프 노드에 데이터 파일 내의 **위치(Offset)** 을 저장한다.

![png](/Database/_img/InnoDB_data_record.png)

InnoDB 테이블은 리프 노드에 레코드의 **PK 값**을 저장한다.

MyISAM이나 MEMORY 테이블은 클러스터링되지 않기 때문에 레코드가 INSERT될 때 처음 저장된 공간에서 절대 이동하지 않는다. 하지만 InnoDB 테이블은 클러스터링 인덱스를 사용하기 때문에 레코드가 INSERT될 때마다 데이터 파일 내에서 PK 값을 기준으로 순서가 계속 바뀐다. 

만약 InnoDB 테이블의 Non-Clustered Index의 리프 노드에 데이터의 위치를 저장했다면 새로운 레코드가 Insert 될 때마다 변경되는 위치에 대해 Index page에도 수정 작업을 해야한다. 이런 오버헤드를 제거하기 위해 InnoDB 테이블의 모든 Non-Clustered Index는 레코드의 위치가 아닌 **PK 값을 저장**하도록 구현되어있다.

<br>


## Reference

- https://sqlhints.com/tag/clustered-index-b-tree-structure/
- https://docs.microsoft.com/ko-kr/sql/relational-databases/indexes/clustered-and-nonclustered-indexes-described?view=sql-server-ver16
- https://itholic.github.io/database-index/
- https://github.com/JaeYeopHan/Interview_Question_for_Beginner/tree/master/Database
- https://www.youtube.com/watch?v=P5SZaTQnVCA&t=1s
- https://yurimkoo.github.io/db/2020/03/14/db-index.html