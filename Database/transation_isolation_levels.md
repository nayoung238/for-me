## Transaction Anomaly detection

- Dirty Read: 커밋이 끝나지 않은 트랜잭션의 변경 사항을 읽는 경우
    - Read Uncommitted 레벨에서 발생
- Non-Repeatable Read: 한 트랜잭션에서 두번의 조회를 했을 때 그 사이에 다른 트랜잭션에 의해 데이터가 변경되어 두번의 조회 결과가 서로 달라 데이터의 일관성이 깨진 경우
    - Read Uncommitted, Read Committed 레벨에서 발생
- Phantom Read: 한 트랜잭션에서 같은 쿼리를 2번 실행하는데 그 사이에 없던 레코드가 추가되는 경우
    - Serializable 레벨을 제외한 모든 레벨에서 발생
<br>

## Isolation Level

데이터 무결성과 동시성을 조절한다. 데이터 무결성을 보장할수록 동시성이 낮아지고, 동시성을 높일수록 데이터의 무결성을 보장하기 어렵다.

## Level 0: Read Uncommitted

- 커밋, 롤백 여부에 상관없이 다른 트랜잭션에서 변경하는 값을 읽을 수 있음
- Dirty Read 현상 발생
- Select 문장을 수행하는 경우 해당 데이터에 Shared lock이 걸리지 않음

## Level 1: Read Committed

- 트랜잭션이 커밋되어 확정된 데이터만 읽는 것을 허용
- 한 트랜잭션에서 두번의 조회를 하려는 상황이고 다른 트랜잭션에서 수정중인 상황으로 예시
- 수정중인 트랜잭션이 커밋하기 전에는 Undo 영역의 데이터(수정되기 전의 기존 데이터이며 이전에 커밋이 완료)를 읽고 (커밋하기 전에 테이블은 수정한 값으로 기록되어있으며 아직 커밋되지 않은 데이터이므로 이전 커밋인 Undo 영역의 데이터를 읽는다.)
- 커밋 후에는 실제 데이터를 읽으므로 Non-Repeatable Read 현상 발생
- Select 문장이 수행되는 동안 해당 데이터에 Shared Lock이 걸림
- RDB에서 대부분 사용하는 레벨

## Level 2: Repeatable Read

- InnoDB에서 사용하는 격리 수준
- Select 문장이 사용하는 모든 데이터에 Shared Lock이 걸리므로 다른 트랜잭션은 관련 데이터를 수정할 수 없음
- 다른 트랜잭션의 커밋 여부와 관계없이 동일 트랜잭션 내에서는 항상 Undo 영역의 데이터를 반환하므로 Non-Repeatable Read 현상이 발생하지 않음
- 즉, 한 트랜잭션에서 조회한 데이터의 결과가 항상 동일함을 보장
- 보통 다른 트랜잭션에 커밋하면 해당 Undo log를 삭제하는데 Repeatable Read 레벨에서는 요청한 트랜잭션 이전의 트랜잭션에 의해 생성된 Undo 영역은 삭제 대상에 포함시키지 않도록 설계

## Level 3: Serializable

- 읽기 작업도 Lock을 획득해야만 가능하므로 동시 처리 성능이 가장 낮음
- DB에서 거의 사용하지 않음

<br>

> Reference
- http://egloos.zum.com/ljlave/v/1530887
- https://nesoy.github.io/articles/2019-05/Database-Transaction-isolation
- https://velog.io/@hyj2508/MySQL-%ED%8A%B8%EB%9E%9C%EC%9E%AD%EC%85%98-%EA%B2%A9%EB%A6%AC%EC%88%98%EC%A4%80