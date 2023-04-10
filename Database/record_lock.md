##Record Lock

레코드 자체가 아닌 인덱스의 레코드에 대한 잠금이다. SELECT…FOR UPDATE 구문으로 다른 트랜잭션의 삽입, 업데이트 그리고 삭제 작업을 막기 때문에 Exclusive Lock을 획득한다. 

PK를 설정하지 않았다고 해도 InnoDB 스토리지 엔진은 내부적으로 클러스터 인덱스를 자동 생성하므로 무조건 인덱스 레코드를 잠근다. 레코드 자체가 아닌 인덱스를 잠그는 것은 큰 성능 차이를 보인다고 한다. 실제 데이터 파일이 아닌 인덱스를 잠근다는 것은 실제 데이터가 존재하는 디스크까지 접근하지 않아도 되니 작업 부하가 덜할 것이라 예상된다. 물론 나의 추측이라 이 부분은 수정 가능성이 높다.

InnoDB에서 Clustered(PK)나 유니크 인덱스에 의한 변경 작업은 Gap에 대해 잠그지 않고, 레코드 자체만 락을 걸어 처리할 수 있다. 그러나 유니크하지 않은 보조 인덱스나 인덱스가 아닌 값을 기준으로 수행되는 작업은 Next key lock 또는 Gap lock을 사용한다. 

FOREIGN KEY 제약 조건이 테이블에 정의된 경우 제약 조건을 확인해야 하는 모든 삽입, 업데이트 또는 삭제는 제약 조건을 확인하기 위해 살펴보는 레코드에 공유 레코드 수준 잠금(S, REC_NOT_GAP)을 설정한다. 또한 InnoDB는 제약 조건이 실패하는 경우 이러한 잠금을 설정한다.


## Repeatable-Read에서 테스트

```mysql
create table account (
	id INT NOT NULL,
	name VARCHAR(50) NOT NULL,
	age INT NOT NULL,
	PRIMARY KEY(id),
	KEY idx_name(name)
);
```
```shell
mysql> select * from account;
+----+-------+------+
| id | name  | age  |
+----+-------+------+
|  1 | one   |   10 |
|  2 | two   |   20 |
|  3 | three |   30 |
|  4 | two   |  234 |
|  5 | two   |   12 |
|  7 | three |  234 |
|  8 | two   | 3425 |
| 11 | xyz   | 2543 |
+----+-------+------+
8 rows in set (0.00 sec)

mysql> select count(*) from account where name = 'two';
+----------+
| count(*) |
+----------+
|        4 |
+----------+
1 row in set (0.00 sec)

mysql> select count(*) from account where name = 'two' and age = 12;
+----------+
| count(*) |
+----------+
|        1 |
+----------+
1 row in set (0.00 sec)

mysql> start transaction;
mysql> update account set name = 'twoo' where name = 'two' and age = 12;
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0
```

만약 name과 age 둘 다 인덱스로 설정하지 않았다면 update account set name = 'twoo' where name = 'two' and age = 12; 실행 시 모든 레코드에 대해서 X(Next-key lock)이 걸린다.

만약 name만 인덱스로 설정했다면 결과는 다를까?

```shell
mysql> select * from performance_schema.data_locks \G\
*************************** 1. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 'two', 2
*************************** 3. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 'two', 5
*************************** 4. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 'two', 4
*************************** 5. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 'two', 8
*************************** 6. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203440
            LOCK_TYPE: RECORD
            LOCK_MODE: X,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 2
*************************** 7. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203440
            LOCK_TYPE: RECORD
            LOCK_MODE: X,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 4
*************************** 8. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203440
            LOCK_TYPE: RECORD
            LOCK_MODE: X,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 8
*************************** 9. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203440
            LOCK_TYPE: RECORD
            LOCK_MODE: X,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 5
*************************** 10. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059203784
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 'xyz', 11
*************************** 11. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059203784
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 'twoo', 5
11 rows in set (0.00 sec)
```

index로 설정한 name과 그렇지 않은 age의 조합으로 검색한 결과 역시나 많은 부분이 잠기는 것을 확인할 수 있다. 
잘보면 조건에 일부분 일치하는 PK를 기준으로 모두 X.REC_NOT_GAP이, 인덱스로 설정한 name을 기준으로 모두 X(Next Key Lock)이 걸린다. 
또한 커밋 예정인 Index(‘twoo’, 5(PK))에도 X, GAP(Gap lock)이 걸리는 것을 확인할 수 있다.

이처럼 적절한 인덱스를 사용하지 않는다면 많은 레코드를 잠금으로서 클라이언트 간의 동시성이 떨어져 한 세션에서 쓰기 작업을 하는 중에는 다른 클라이언트는 해당 테이블에 대한 쓰기 작업을 하지 못하고 기다려야 되는 상황이 발생하기 때문이다.

where 절에 작성한 여러 조건이 모두 인덱스가 아니라면 테이블에 존재하는 모든 레코드에 X(Next key lock)이 걸린다는 사실에 꼭 주의하자.


## Read-Committed에서 테스트

위 결과는 InnoDB의 기본 격리 수준인 Repeatable-Read 격리 수준에서 실행한 결과이다. 격리 수준을 낮춰 Read-Committed로 변경해 같은 상황을 테스트하면 결과는 다음과 같다.

```shell
mysql> select * from performance_schema.data_locks \G\
*************************** 1. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056291672
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: idx_name
OBJECT_INSTANCE_BEGIN: 5059212312
            LOCK_TYPE: RECORD
            LOCK_MODE: X,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 'two', 5
*************************** 3. row ***************************
               ENGINE: INNODB
          OBJECT_NAME: account
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059212656
            LOCK_TYPE: RECORD
            LOCK_MODE: X,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 5
3 rows in set (0.00 sec)
```

Read-Committed 격리 수준에서는 Gap Lock을 사용하지 않으므로 Clustered(PK)와 index 모두 조건과 완벽히 일치한 레코드에만 X, REC_NOT_GAP (record lock) 이 걸린다. 
Read-Committed 격리 수준은 Gap Lock을 사용하지 않아 다른 세션에서 근처 범위내에 새로운 레코드를 삽입할 수 있어 Phantom read 현상이 발생한다.
