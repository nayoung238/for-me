## Next Key Lock

Record Lock과 Gap Lock을 합쳐 놓은 잠금으로 특정 레코드를 기준으로 앞에 존재하는 레코드의 사이 공간과 기준이었던 레코드를 잠근다. ‘정확히 어떤 범위를 잠그냐’에 대한 답변은 아래의 TEST 1~4를 통해 파악할 수 있다.
Repeatable Read 격리 수준에서 Phantom Read 현상이 발생한다. 그러나 InnoDB는 Next Key Lock을 사용해 근처 많은 부분을 잠그므로 Phantom Read 현상을 막을 수 있다. 하지만 많은 부분이 잠그므로 적절한 인덱스 설정이 중요하다.

추가적으로 MySQL 8.0 부터는 Read Committed isolation에서 Next Key Lock을 사용하지 않는다고 한다. 그러므로 Read-Committed 격리 수준을 사용하면 Phantom Read 현상이 발생할 수 있다.

Next Key Lock의 목적은 바이너리 로그에 기록되는 쿼리가 슬레이브에서 실행될 때마다 마스터에서 만들어 낸 결과와 동일한 결과를 만들어내도록 보장하는 것이다. 실제 업무 상에서는 Gap Lock보다 Next Key Lock이 일반적으로 많이 사용된다.
<br>

## LOCK_MODE 종류

- X, REC_NOT_GAP: Exclusive record lock
- S. REC_NOT_GAP: Shared record lock
- X, GAP: Exclusive Gap lock
- S; Next Key lock
- X: Next Key lock
- IS: Intention Shared lock
- IX: Intention Exclusive lock

<br>

## TEST1 : 이미 존재하는 레코드 수정

```shell
mysql> begin;
mysql> update account set name = 'nayoung2' where id = 3;  // PK
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0
mysql> commit;
```
```
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:1063:5056289624
ENGINE_TRANSACTION_ID: 1807
            THREAD_ID: 49
             EVENT_ID: 28
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:2:4:11:5059203096
ENGINE_TRANSACTION_ID: 1807
            THREAD_ID: 49
             EVENT_ID: 28
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 3
2 rows in set (0.00 sec)
```

- account table에 IX 락이 걸린다.
- 존재하는 조건이며, 해당 조건은 PK이므로 해당 레코드에 X, REC_NOT_GAP(record lock)이 걸린다.
- Read-committed, Repeatable-read 격리 수준에서 모두 같은 결과가 나왔다.

<br>

## TEST2: 중복 ID로 INSERT 시도

```shell
mysql> select * from account;
+----+---------+
| id | name    |
+----+---------+
|  1 | kaya    |
|  3 | nayoung |
|  6 | apple   |
| 12 | banana  |
| 16 | kiwi    |
+----+---------+
5 rows in set (0.00 sec)
```
```
mysql> begin;
mysql> insert account value (12, 'ba');
ERROR 1062 (23000): Duplicate entry '12' for key 'account.PRIMARY'
```
```
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:1063:5056289624
ENGINE_TRANSACTION_ID: 1820
            THREAD_ID: 49
             EVENT_ID: 64
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:2:4:10:5059203096
ENGINE_TRANSACTION_ID: 1820
            THREAD_ID: 49
             EVENT_ID: 64
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: S,REC_NOT_GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 12
2 rows in set (0.00 sec)
```

- INSERT 연산이므로 해당 테이블에 IX 락이 걸린다.
- 하지만 PK 12는 이미 존재하는 값이므로 새로운 쿼리는 반영은 되지않는다.
- 잘보면 PK 12 레코드에 S, REC_NOT_GAP(Record Lock)이 걸린다.
- InnoDB는 제약 조건을 지켜지지 않으면 해당 레코드에 S, REC_NOT_GAP이 걸린다.
- Read-Committed, Repeatable-Read 격리 수준의 결과는 같다.

<br>

## TEST3: 존재하지 않는 ID에 UPDATE 시도

```shell
mysql> begin;
mysql> update account set name = 'kaya2' where id = 0;
Query OK, 0 rows affected (0.00 sec)
Rows matched: 0  Changed: 0  Warnings: 0
mysql> commit;
```

### Repeatable Read 격리 수준

```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:1063:5056289624
ENGINE_TRANSACTION_ID: 1811
            THREAD_ID: 49
             EVENT_ID: 42
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:2:4:2:5059203096
ENGINE_TRANSACTION_ID: 1811
            THREAD_ID: 49
             EVENT_ID: 42
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 1
2 rows in set (0.00 sec)
```

- UPDATE 작업이므로 일단 해당 테이블에 IX락을 걸린다.
- 0보다 크면서 존재하는 레코드인 id = 1을 기준으로 앞쪽으로 X, GAP(gap lock)이 걸린다.
- 여기서 LOCK_TYPE: RECORD 로 설정되어있는데 이는 레코드 수준의 잠금을 의미할뿐, 테이블에 존재하는 레코드 자체의 잠금으로 해석하면 안된다.
- 같은 예제로 Read Committed 격리 수준에서 테스트 한 결과는 다음과 같다.

### Read Committed 격리 수준

```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:1067:5056289624
ENGINE_TRANSACTION_ID: 1901
            THREAD_ID: 51
             EVENT_ID: 83
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
1 row in set (0.00 sec)
```

- Repeatable read 격리 수준에서는 존재하지 않는 값보다 큰 값을 갖는 레코드를 기준으로 앞부분에 X, GAP(gap lock)이 걸렸지만
- Read-Committed 격리 수준에서는 Gap lock이 걸리지 않는다. 
<br>

정말 앞쪽으로 걸리는지 확인하기 위해 한번 더 테스트해보자. 이번에는 id 3과 6사이에 존재하지 않는 4를 업데이트 시켜보자.

```shell
mysql> select * from account;
+----+---------+
| id | name    |
+----+---------+
|  1 | kaya    |
|  3 | nayoung |
|  6 | apple   |
| 16 | kiwi    |
+----+---------+

mysql> update account set name = 'kaya2' where id = 4;
Query OK, 0 rows affected (0.00 sec)
Rows matched: 0  Changed: 0  Warnings: 0
```

### Repeatable read 격리 수준

```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:1063:5056289624
ENGINE_TRANSACTION_ID: 1813
            THREAD_ID: 49
             EVENT_ID: 48
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:2:4:6:5059203096
ENGINE_TRANSACTION_ID: 1813
            THREAD_ID: 49
             EVENT_ID: 48
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 6
2 rows in set (0.00 sec)
```

- 존재하는 레코드 중 4보다 큰 6을 기준으로 X, GAP(gap lock) 이 걸린다.
- 같은 예제로 Read-Committed 격리 수준에서 테스트한 결과는 다음과 같다.

### Read Committed 격리 수준

```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:1067:5056289624
ENGINE_TRANSACTION_ID: 1905
            THREAD_ID: 51
             EVENT_ID: 99
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
1 row in set (0.00 sec)
```

- Read-Committed 격리 수준에서는 역시나 Gap-lock이 걸리지 않는다.
- 마지막 레코드보다 큰 값을 업데이트 하는 경우도 테스트해보자.

```shell
mysql> select * from account;
+----+---------+
| id | name    |
+----+---------+
|  1 | kaya    |
|  3 | nayoung |
|  6 | apple   |
| 16 | kiwi    |
+----+---------+

mysql> begin;
mysql> update account set name = 'kaya2' where id = 24;
Query OK, 0 rows affected (0.00 sec)
Rows matched: 0  Changed: 0  Warnings: 0
```

### Repeatable read 격리 수준

```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:1063:5056289624
ENGINE_TRANSACTION_ID: 1814
            THREAD_ID: 49
             EVENT_ID: 52
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056289624
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:2:4:1:5059203096
ENGINE_TRANSACTION_ID: 1814
            THREAD_ID: 49
             EVENT_ID: 52
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: supremum pseudo-record
2 rows in set (0.00 sec)
```

- 테이블 끝에 있는 범위를 잠그는 경우 사용가능한 키가 없다면 supremum pseudo-record 로 설정되고 X(Next key lock)이 걸린다.
- supremum이란 진짜 인덱스 레코드가 아니고, 가장 큰 인덱스 값 다음에 오는 갭을 잠그는 것을 의미
같은 예제를 Read-Committed 격리 수준에서 테스트하면 역시나 Gap-lock은 걸리지 않는다.