# Join

테이블별로 분리되어 있는 데이터를 연결하여 조회하기 위해 JOIN을 사용한다.

- Inner join : 교집합으로 두 테이블의 join column에 해당하는 값이 모두 존재하는 경우 조회
- Outer join : join 테이블에 데이터가 없어도 NULL로 처리되어 조회
- Left join : 기준 테이블을 왼쪽으로 지정
- Right join : 기준 테이블을 오른쪽으로 지정

<br>

## INNER JOIN

![](/Database/_img/sql_innerJoin.png)

inner join은 두 테이블에서 join하는 column 값이 일치하는 경우, 결합하여 새로운 데이블을 생성한다.
<br>

![](/Database/_img/joinTableA.png)

```sql
SELECT ID, NAME, PHONE_NUMBER
FROM A INNER JOIN B
ON A.ID = B.ID
```

![](/Database/_img/innerJoin_result.png)

<br>

### leetcode 181

> 문제: https://leetcode.com/problems/employees-earning-more-than-their-managers/

![](/Database/_img/leetcode_db181.png)

직원 정보가 들어있는 Employee 테이블에서 직원 중 매니저가 있는 경우 ManagerId column이 직원 Id로 채워져 있다. **매니저가 있는 직원 중 매니저보다 더 많은 Salary 를 받는 직원**을 오른쪽과 같이 출력한다.

```sql
SELECT e1.Name AS Employee
FROM Employee e1 INNER JOIN Employee e2
ON e1.ManagerId = e2.Id
WHERE e1.Salary > e2.Salary
```

<br>

## OUTER JOIN

![](/Database/_img/sql_outerJoin.png)

join하는 여러 table 중 한쪽에만 데이터가 있고 다른 쪽에는 없을 때, 데이터가 있는 table의 데이터를 출력한다. ORACLE에서는 FULL OUTER JOIN를 제공하지만 MySQL은 지원하지 않으니 직접 구현해야 한다.

![](/Database/_img/outerJoin_joinTableA.png)

```sql
SELECT *
FROM A LEFT JOIN B ON A.ID = B.ID
UNION
SELECT *
FROM B RIGHT JOIN A ON A.ID = B.ID
```

![](/Database/_img/outerJoin_result.png)

<br>

## OUTER JOIN + ( left is NULL OR right is NULL )

![png](/Database/_img/sql_outerJoin_null.png)

공통된 부분을 제외한 나머지 데이터를 모두 조회한다.

<br>

![png](/Database/_img/outerJoinTable.png)

```sql
SELECT *
FROM A LEFT JOIN B WHERE B.ID IS NULL
UNION
SELECT *
FROM B RIGHT JOIN A WHERE A.ID IS NULL
```

![png](/Database/_img/outerJoin_null_result.png)

<br>

## LEFT JOIN

![png](/Database/_img/sql_leftJoin.png)

join 기준으로 왼쪽 table의 모든 데이터를 조회한다.

<br>

![png](/Database/_img/joinTableA.png)

```sql
SELECT A.ID, A.NAME, B.PHONE_NAME
FROM A LEFT OUTER JOIN B
ON A.ID = B.ID
```
![png](/Database/_img/leftOuterJoin_result.png){

<br>

### leetcode 183

> 문제: https://leetcode.com/problems/customers-who-never-order

![png](/Database/_img/leetcode_db183.png)

고객 이름이 있는 Customers 테이블과 주문자의 아이디가 있는 Orders 테이블이 있다.
오른쪽과 같이 주문하지 않는 고객만 조회하려고 한다.

```sql
select Name as Customers
from Customers c left join Orders o on c.id = o.customerid
where o.id is null
```

<br>

## LEFT JOIN + right is NULL

![png](/Database/_img/sql_leftJoin_null.png)

공통된 부분을 제외하고 join 기준 왼쪽 테이블에만 존재하는 데이터를 조회한다.
<br>

![png](/Database/_img/joinTableA.png)

```sql
SELECT A.ID, A.NAME, B.PHONE_NAME
FROM A LEFT OUTER JOIN B
ON A.ID = B.ID
WHERE B.ID IS NULL
```
![png](/Database/_img/leftOuterJoin_Bnull_result_result.png)

<br>

## RIGHT JOIN

![png](/Database/_img/sql_rightJoin.png)

join 기준으로 오른쪽 table의 모든 데이터를 조회한다.
<br>

![png](/Database/_img/joinTableB.png)

```sql
SELECT A.ID, A.NAME, B.PHONE_NUMBER
FROM A RIGHT OUTER JOIN B
ON A.ID = B.ID
```
![png](/Database/_img/rightOuterJoin_result.png)

<br>

## RIGHT JOIN + left is NULL

![png](/Database/_img/sql_leftJoin_null.png)

공통된 부분을 제외하고 join 기준 오른쪽 테이블에만 존재하는 데이터를 조회한다.
<br>

![png](/Database/_img/joinTableB.png)

```sql
SELECT A.ID, A.NAME, B.PHONE_NUMBER
FROM A RIGHT OUTER JOIN B
ON A.ID = B.ID
WHERE A.ID IS NULL
```

![png](/Database/_img/rightOuterJoin_Anull_result.png)
