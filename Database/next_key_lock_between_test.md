## TEST4: BETWEEN

```shell
mysql> desc account;
+-------+-------------+------+-----+---------+-------+
| Field | Type        | Null | Key | Default | Extra |
+-------+-------------+------+-----+---------+-------+
| id    | int         | NO   | PRI | NULL    |       |
| name  | varchar(50) | NO   |     | NULL    |       |
+-------+-------------+------+-----+---------+-------+

mysql> select * from account;
+----+---------+
| id | name    |
+----+---------+
|  2 | nayoung |
|  5 | kaya    |
| 11 | apple   |
| 15 | kiwi    |
+----+---------+
4 rows in set (0.00 sec)
```
```shell
mysql> select * from account where id between 4 and 13 for update;

mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:1067:5056289624
ENGINE_TRANSACTION_ID: 1913
            THREAD_ID: 51
             EVENT_ID: 131
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
       ENGINE_LOCK_ID: 4851436672:6:4:3:5059203096
ENGINE_TRANSACTION_ID: 1913
            THREAD_ID: 51
             EVENT_ID: 131
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 5
*************************** 3. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:6:4:4:5059203096
ENGINE_TRANSACTION_ID: 1913
            THREAD_ID: 51
             EVENT_ID: 131
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203096
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 11
*************************** 4. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851436672:6:4:5:5059203440
ENGINE_TRANSACTION_ID: 1913
            THREAD_ID: 51
             EVENT_ID: 131
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059203440
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP // Exclusive Gap Lock
          LOCK_STATUS: GRANTED
            LOCK_DATA: 15
4 rows in set (0.00 sec)
```

![](/_img/database/next_key_lock_between_repeatable_read_isolation_level.png)

- Repeatable-read 격리 수준에서 테스트한 결과이다.
- 지정한 범위보다 많은 범위가 잠긴다.
- 테스트 결과 Next Key Lock이 잠금을 거는 정확한 범위에는 이전 레코드가 포함되지 않는다. 
- 그러므로 5를 기준으로 Next Key Lock을 건다면 존재하는 레코드 2는 잠금에 포함되지 않는다. 
- 그래서 레코드 2에 대한 UPDATE 및 DELETE 작업이 가능하다.
<br>

- 위 결과를 바탕으로 테이블의 레코드 건수가 적을수록 Gap Lock의 간격이 넓어지는 역효과가 나타는 것을 알 수 있다. 
- 그러므로 테이블의 레코드 건수가 적은 상태에서 동시 실행되는 트랜잭션이 많을수록, 트랜잭션간 상호 간섭과 대기를 고려해야 한다. 
- 트랜잭션간 상호 간섭과 대기란 위와 같은 상황에서 다른 세션은 회색 부분에 어떠한 레코드도 삽입 및 수정이 불가능하므로 응답 시간이 느려지는 것을 의미한다.
<br>

- 만약 다른 세션에서 ```insert into account (11, 'nayoung');``` 을 작성한다면 해당 세션은 기존 Next Key Lock이 풀릴 때까지 기다려야 하고, 
- 만약 대기가 길어진다면 ```ERROR 1205 (HY000): Lock wait timeout exceeded; try restarting transaction```이 뜨며 자동으로 중지된다. 
- 또한 락 상태 결과를 보면 기존 락에 다음 락이 추가된다.

```shell
mysql> _intetnmysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851437464:1070:5056290648
ENGINE_TRANSACTION_ID: 2034
            THREAD_ID: 78
             EVENT_ID: 22
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056290648
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 4851437464:9:4:4:5059207704
ENGINE_TRANSACTION_ID: 2034
            THREAD_ID: 78
             EVENT_ID: 22
        OBJECT_SCHEMA: test
          OBJECT_NAME: account
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059207704
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP,INSERT_INTENTION
          LOCK_STATUS: WAITING
            LOCK_DATA: 11

```
- X, GAP, INSERT_INTENTION 락을 획득하기위해 기다리는 상태이다.

![](/_img/database/next_key_lock_between_read_committed_isolation_level.png)

- Read-Committed 격리 수준에서 테스트한 결과이다.
- 개발자가 설정한 범위에 존재하는 레코드만 X, REC_NOT_GAP 으로 잠긴다.
- 그러므로 아무리 범위를 지정해도 존재하지 않는 공간은 잠기지 않으니 Non-repeatable read 현상이 발생한다.
