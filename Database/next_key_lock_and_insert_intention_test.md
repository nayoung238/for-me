## TEST: Next-key lock VS Insert_intention

index를 사용할 경우 PK이 걸리는 락과 index에 걸리는 락이 상황에 따라 다르다. 만약 Index에 X(Next Key Lock)이 걸려도 Clustered에는 X, REC_NOT_GAP이 걸린다. 

name 컬럼을 index로 설정했을 때 Next-Key lock과 insert_intention이 충돌하는데 다음 예제에서 살펴보자.

```sql
create table account (
	id INT NOT NULL,
	name VARCHAR(50) NOT NULL,
	age INT NOT NULL,
	PRIMARY KEY(id),
	KEY idx_name(name) // NON-UNIQUE
);

insert into account values (1, 'one', 10), (2, 'two', 20), (3, 'three', 30); 
```

트랜잭션 A가 select * from account where name = 'two' for update; 를 실행할 경우 다음과 같이 락이 걸린다.

```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: supremum pseudo-record
*************************** 3. row ***************************
               ENGINE: INNODB
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 'two', 2
*************************** 4. row ***************************
               ENGINE: INNODB
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203440
            LOCK_TYPE: RECORD
            LOCK_MODE: X,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 2
4 rows in set (0.00 sec)
```

PK는 Gap Lock이 아닌 Record Lock이 걸리고,
검색 조건인 idx_name에는 Next Key Lock이 걸린다. 


NON-UNIQUE Secondary Index이므로 Next Key Lock이 걸린다. 
만약 UNIQUE Secondary Index라면 Secondary Index에도 X, REC_NOT_GAP이 걸린다.
만약 Secondary Index가 아니라면 Clustered에 존재하는 모든 레코드에 X(Next Key Lock)이 걸린다.

이 상태에서 트랜잭션 B가 insert into account value (5, ‘twoa’, 34); 를 실행하면 일단 대기해야한다. 그 이유는 트랜잭션 A가 idx_name에서 two를 기준으로 뒤(supremum pseudo-record)에 X(Next Key Lock)을 획득한 상태이므로 트랜잭션 B는 X, INSERT_INTENTION 을 획득하기 위해 대기한다.

```shell
mysql> select * from performance_schema.data_locks \G\
*************************** 1. row ***************************
               ENGINE: INNODB
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056290648
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059207704
            LOCK_TYPE: RECORD
            LOCK_MODE: X,INSERT_INTENTION
          LOCK_STATUS: WAITING
            LOCK_DATA: supremum pseudo-record
```
 
이를 통해 PK와 Index에 대한 수정이 동기화로 이루어진다는 것을 확인할 수 있다. 또한, 기존에 Next-Key Lock을 획득한 곳에 새로운 Intention lock의 획득은 불가능하다는 것도 확인할 수 있다.

만약 Secondary-index가 없는 상태에서 위와 똑같은 예를 테스트하면 결과는 다음과 같다. 일단 트랜잭션 B는 역시나 INSERT_INTENTION 을 얻기 위해 기다린다. 하지만, 조건절에 작성한 where name = 'two' 는 더 이상 index가 아니다. 이처럼 Index가 아닌 컬럼으로 검색할 경우 Clustered(PK)에 존재하는 모든 레코드에 X(Next-Key lock)이 걸린다. 즉, 해당 테이블에 어떠한 수정도 불가능하므로 적절한 Index 설정 및 쿼리 작성이 중요하다.
