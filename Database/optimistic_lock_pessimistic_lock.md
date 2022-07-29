## Optimistic Lock (냑관적 락)

- 데이터 수정 시 경합이 발생하지 않을 것이라고 예상하는 연산에서 주로 사용
    - 회원 정보 수정은 해당 회원에 의해서만 이루어지므로 동시 요청이 발생할 가능성이 낮음
    - 이와 반대의 상황인 아이템 재고 수정은 동시 요청 발생 가능성이 높음
- 즉, 동시에 수정이 이루어진 시점에 예외를 발생시켜도 되며 이런 예외 발생 가능성이 낮다고 예상함
- 잠금보다는 충돌 감지(Conflict detection)에 가까움
- Transaction 단위가 아니므로 충돌 발생 시 Application 단에서 롤백을 수동으로 처리

<br>

## Pessimistic Lock (비관적 락)

- 아이템 재고 수정같이 특정 데이터를 동시에 수정할 가능성이 높다고 예상하는 연산에서 주로 사용
- 높은 충돌 감지를 예상하기 때문에 애초에 락을 걸어놓고 시작
- Shared Lock(Read) 또는 Exclusive Lock(Write)을 사용
- 다른 트랜잭션이 끝날 때까지 수정을 시도할 수 없는 Repeatable Read(level2)와 Serializable(level3) 레벨 수준에서 사용 가능
<br>

- 데이터 수정 즉시 트랜잭션의 충돌 감지 가능
- 한 작업에 여러 연산이 있는 Transaction에서 Pessmistic Lock이 적합
- 처리 중간에 오류 발생은 Transaction이 실패한 것이므로 Database 단에서 전체 Rollback이 일어남
<br>

수정 비율이 높다면 Pessimistic Lock을 사용하고 읽기 비율이 높다면 Optimistic Lock을 사용한다.

<br>

## Implicit Lock

- JPA에서 Entity에 ```@Version``` 또는 ```@OptimisticLocking``` 이 설정되어있는 경우 자동적으로 충돌감지를 위한 잠금이 실행
    - JPA에서 제공하는 ```@Version``` 사용 시 특정 데이터가 다른 트랜잭션에 의해 이미 버전 정보가 바뀐 상태라고 하면 업데이트 로우 수가 0이 반환되고 이를 충돌감지로 처리해 예외(OptimisticLockException)가 발생
    - Hibernate에서 제공하는 ```@OptimisticLocking``` 은 @OptimisticLocking(type = OptimisticLockType.원하는 종류) 형식으로 설정
- 데이터베이스가 업데이트, 삭제 쿼리 발생시 해당 Row에 대한 Row Exclusive Lock(행 배타잠금)이 실행

<br>

## Explicit Lock

- 프로그램을 통해 의도적으로 잠금을 실행
- JPA에서 Entity를 조회할 때 LockMode를 지정
- ```SELECT ~ FOR UPDATE``` 구문을 사용해 특정 데이터에 대한 Exclusive Lock을 건다. (다른 쿼리가 같은 데이터에 접근 시 읽는 것만 가능)


> Reference
- https://reiphiel.tistory.com/entry/understanding-jpa-lock