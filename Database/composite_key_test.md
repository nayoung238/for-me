## TEST5: Composite Key

```shell
mysql> desc follow;
+--------------+--------+------+-----+---------+-------+
| Field        | Type   | Null | Key | Default | Extra |
+--------------+--------+------+-----+---------+-------+
| from_account | bigint | NO   | PRI | NULL    |       |
| to_account   | bigint | NO   | PRI | NULL    |       |
+--------------+--------+------+-----+---------+-------+
2 rows in set (0.00 sec)
```
```
mysql> select * from follow;
+--------------+------------+
| from_account | to_account |
+--------------+------------+
|            1 |          7 |
|            2 |          1 |
|            2 |          6 |
|            2 |          7 |
|            2 |          8 |
|            3 |          1 |
|            3 |          5 |
|            3 |          7 |
|            4 |          2 |
|            4 |          3 |
|            5 |          1 |
|            6 |          2 |
|            7 |          3 |
|            8 |          7 |
|            8 |         13 |
+--------------+------------+
15 rows in set (0.00 sec)
```

## CASE1: from_account 기준 for update

```shell
mysql> select * from follow where from_account = 4 for update;
+--------------+------------+
| from_account | to_account |
+--------------+------------+
|            4 |          2 |
|            4 |          3 |
+--------------+------------+
```
위 쿼리에 해당되는 레코드는 2개이며, Range Scan이 가능하다. 실제 락이 걸린 상태는 다음과 같다.

```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056290648
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059207704
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 4, 2
*************************** 3. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059207704
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 4, 3
*************************** 4. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059208048
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 5, 1
4 rows in set (0.00 sec)
```

- 조건에 일부 해당되는 모든 레코드를 기준으로 X(Next Key Lock)이 걸린다.
- 또한, 해당되는 마지막 레코드를 기준으로 그 다음 레코드 사이에 X, GAP(gap lock)이 걸린다.
- 그러므로 다른 트랜잭션에서 위 범위에 INSERT 시도 시 대기해야한다. 
- ```insert into follow value (4, 1);``` 실행 시 락 상태는 다음과 같다.

```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056291672
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059212312
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP,INSERT_INTENTION
          LOCK_STATUS: WAITING
            LOCK_DATA: 4, 2
```

- 다른 트랜잭션에서 획득한 Next Key Lock 범위에 INSERT 하기 위해 X, GAP, INSERT_INTENTION 락을 획득하기 위해 대기한다.

<br>

## CASE2: from_account 기준 share mode

```shell
mysql> select * from follow where from_account = 4 lock in share mode;
+--------------+------------+
| from_account | to_account |
+--------------+------------+
|            4 |          2 |
|            4 |          3 |
+--------------+------------+
```
```shell
mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056290648
            LOCK_TYPE: TABLE
            LOCK_MODE: IS
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059207704
            LOCK_TYPE: RECORD
            LOCK_MODE: S
          LOCK_STATUS: GRANTED
            LOCK_DATA: 4, 2
*************************** 3. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059207704
            LOCK_TYPE: RECORD
            LOCK_MODE: S
          LOCK_STATUS: GRANTED
            LOCK_DATA: 4, 3
*************************** 4. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059208048
            LOCK_TYPE: RECORD
            LOCK_MODE: S,GAP
          LOCK_STATUS: GRANTED
            LOCK_DATA: 5, 1
4 rows in set (0.00 sec)
```

- for update와 상황은 같지만, S, GAP 락이 걸린다.
- 위 상태에서 다른 세션이 ```insert into follow value (4, 1);``` 를 실행할 경우 당연히 대기하게 되며, 락 상태는 다음과 같다.

```shell
mysql> mysql> select * from performance_schema.data_locks \G
*************************** 1. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 5056291672
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
        OBJECT_SCHEMA: instagram
          OBJECT_NAME: follow
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 5059212312
            LOCK_TYPE: RECORD
            LOCK_MODE: X,GAP,INSERT_INTENTION
          LOCK_STATUS: WAITING
            LOCK_DATA: 4, 2

```

<br>

## CASE3: to_account 기준 for update

몇달동안 해결하지 못한 to_account의 많은 문제… 일단 락 상태부터 알아보자.

```shell
mysql> select * from follow where to_account = 7 for update;
+--------------+------------+
| from_account | to_account |
+--------------+------------+
|            1 |          7 |
|            2 |          7 |
|            3 |          7 |
|            8 |          7 |
+--------------+------------+
4 rows in set (0.00 sec)

mysql> select count(*) from follow;
+----------+
| count(*) |
+----------+
|       15 |
+----------+
```
 
- 현재 Follow Table에 존재하는 모든 레코드는 총 15개이다. 
- ```select * from follow where to_account = 7 for update;``` 실행하면 총 17개의 Lock을 획득한다.
<br>

- 테이블에 IX락 획득 : 1개
- 존재하는 모든 레코드에 대해 X(Next Key Lock)락 획득 : 15개
- 마지막 부분인 supremum pseudo-record에 X(Next Key Lock)락 획득 : 1개
<br>

- from_account, to_account 둘다 PK로 설정했지만, from_account, to_account 순서로 정렬된다. 
- 그러므로 from_account를 사용하지 않고 검색할 경우 결국 Full Scan을 해야하므로 모든 레코드에 락이 걸린다.

<br>

## CASE4: to_account 기준 share mode

```shell
mysql> select * from follow where to_account = 7 lock in share mode;
+--------------+------------+
| from_account | to_account |
+--------------+------------+
|            1 |          7 |
|            2 |          7 |
|            3 |          7 |
|            8 |          7 |
+--------------+------------+
4 rows in set (0.00 sec)
```

- share mode도 for update와 상황은 같다.
- 즉, follow 테이블에 총 15개가 존재한다면 17개의 락이 걸린다.
- 1개의 IS + 15개의 S락 + supremum pseudo-record에 S락 1개
<br>

- 복합키의 경우 where절에 모든 복합키를 넣야 Next-Key Lock이 안걸리고
- 첫번째 값만 가지고 탐색해도 해당되는 모든 경우에 Next-Key Lock이 걸린다.
