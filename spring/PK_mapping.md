## Primary Key Mapping

Entity 의 특정 필드를 DB 의 PK로 사용하기 위해 기본키(primary key) mapping annotation 을 설정해야 한다. **PK 는 null 이 될 수 없고 변하면 안된다.** 

PK를 개발자가 직접 할당하는 방식과 DB 가 자동으로 할당하는 방식이 있다.
<br>

## @Id 직접 할당

```java
@Entity
public class User {

	@Id
    private Long id;

    private String email;
    private String name;
    private String username;
    private String password;
}
```
```@Id``` 만 추가하면 해당 필드가 DB 의 PK가 된다. ```User Entity``` 에서 **id** 필드가 DB 에서 PK 가 된다.

```json
{
  	"id" : '1',
    "email" : "apple123@gmail.com",
    "name" : "lee",
    "username" : "id123",
    "password" : "password123"
}
```
새로운 User 를 생성하기 위해 위와 같은 JSON 형식으로 요청하면 다음과 같은 결과를 얻는다.

```text
insert into users (id, email, name, password, username) 
values (?, ?, ?, ?, ?)

insert into users (id, email, name, password, username)
values ('1', 'apple123@gmail.com', 'lee', 'password123', 'id123');
```
![](/_img/Spring/pk_mapping_result.png)

요청한대로 잘 설정됨을 알 수 있다. 하지만 PK 를 지정하지 않을 경우 다음과 같은 예외가 발생한다.
<br>

### @Id 를 지정하지 않을 경우

```json
{
  	"email" : "apple123@gmail.com",
    "name" : "lee",
    "username" : "id123",
    "password" : "password123"
}
```

@Id로 설정한 필드를 채우지 않고 요청하면 springframework 에서는 ```JpaSystemException``` 을, hibernate 에서는 ```IdentifierGenerationException``` 가 발생한다.

```
nested exception is org.springframework
.orm.jpa.JpaSystemException: ids for this class 
must be manually assigned before calling save():
com.domain.user.User; 

nested exception is org
.hibernate.id.IdentifierGenerationException:
ids for this class must be manually assigned before calling save():
com.domain.user.User] with root cause
```
그러므로 **@Id 로 설정한 필드는 DB 에서 PK** 가 되므로 무조건 작성해야 한다. 하지만 PK 는 unique 해야 하는데 데이터가 많아질수록 unique 한 PK 를 만들기 어려울 것이라고 생각한다. 그럴경우 ```@GeneratedValue``` 를 사용해 DB 가 알아서 설정하도록 권한을 위임하면 된다.

<br>

# @GeneratedValue 자동 생성

```java
@Entity
public class User {
	@Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    
    private String email;
    private String name;
    private String username;
    private String password;
}
```
```@GeneratedValue``` 을 사용하면 PK 지정을 DB 에게 위임하며 **개발자가 직접 지정할 수 없다.** DB 가 **auto-increment** 로 PK 를 할당한다.

```json
{
    "email" : "apple123@gmail.com",
    "name" : "lee",
    "username" : "id123",
    "password" : "password123"
}
```
PK 인 id 필드를 제외한 나머지 필드에 대한 값만 작성해 요청하면 된다. 만약 ```@GeneratedValue``` 을 설정했는데 직접 PK 값을 설정한다면 다음과 같은 결과가 된다.

### @Id 직접 지정할 경우

```json
{
    "id" : "35",  // @Id
    "email" : "apple123@gmail.com",
    "name" : "lee",
    "username" : "id123",
    "password" : "password123"
}
```
```text
insert into users (id, email, name, password, username) 
values (null, ?, ?, ?, ?)
insert into users (id, email, name, password, username)
values (null, 'apple123@gmail.com', 'lee', 'password123', 'id123');
```
![](/_img/Spring/pk_mapping_result.png)

@Id 필드를 **35** 로 요청했지만 **query 에 반영되지 않고 실제 DB 에도 반영되지 않는다.** 그러므로 ```@GeneratedValue``` 설정 시 @Id 필드를 직접 지정하지 않아도 된다.

```@GeneratedValue``` 에는 IDENTITY, SEQUENCE, TABLE 전략이 존재한다. TABLE 전략은 잘 사용되지 않는다.

<br>

## IDENTITY

**PK 설정을 DB 에게 위임**하는 전략으로 MYSQL, PostgreSQL, SQL Server, DB2 에서 주로 사용한다.

```java
@Entity
public class User {

	@Id 
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    
    private String username;
}
```

User Entity 의 **id 필드가 PK** 이며 ```@GeneratedValue``` 을 설정했기 때문에 개발자가 직접 입력하지 않는다. 그러므로 나머지 필드만 채워 요청하면 된다.

![](/_img/Spring/pk_mapping_identity_result.png)

위와 같이 PK 인 ID 필드가 중복되지 않고 **auto-increment** 으로 채워진다.
<br>

### DB 에 반영되는 과정

우리는 각 필드의 값을 JSON 형식으로 요청해 UserRepository 에 저장할 수 있는 API 를 호출하면 끝난다. 이 과정에서 User Entity 를 만들어 실제 DB 에 저장된다. 실제 DB 에 저장하기 위해 persistence context 를 거친다. 

- 저장하려고 하는 User Entity 에 대해 ```persist()``` 를 호출해 영속 상태로 만들고
- transaction commit 실행 시 entity manager 가 내부에서 ```flush()``` 를 호출해 실제 DB 에 반영한다.

위 과정을 거쳐야 실제 DB 에 반영된다.

```persist()``` 를 호출했다는 것은 persistence context 에서 영속 상태가 되기 위함이다. 영속 상태가 되려면 **persistence context 내에서도 Entity 를 식별하기 위한 PK 가 필요**하다. 하지만 IDENTITY 전략은 DB 에게 PK 지정을 위임했으므로 **DB 에 접근하기 전까지 PK 를 설정**할 수 없다.

이를 해결하기 위해 IDENTITY 전략만 예외적으로 ```persist()``` 호출 시 **INSERT query 가 발생**한다. ```persist()``` 호출 시 INSERT query 가 발생해 DB 에 저장되면서 PK 도 설정된다.

DB connection 비용을 줄이기 위해 발생한 여러 query 를 어느정도 모았다가 **한번에 보내 최적화**한다. 하지만 IDENTITY 전략을 사용하는 Entity 를 처음 저장할 때는 PK 설정때문에 바로 DB 에 접근해야 하므로 이런 상황에서는 여러 query 를 모아 **한번에 insert 할 수 없다는 단점**이 있다.

```java
EntityManagerFactory emf = Persistence.createEntityManagerFactory();
EntityManager em = emf.createEntityManager();
EntityTransaction tx = em.getTransaction();

User user = new User();
user.setUsername("nayoung");

em.persist(user);
System.out.println(user.getId());
```

JPARepository 를 사용하지 않고 EntityManager 만 사용한 코드이다.

위 코드에서 ```persist()``` 후 PK 값을 출력하기 위해 getId() 를 호출했다. Id 필드의 값을 가져오기 위해 **SELECT query 가 발생될 것 같지만 그렇지 않다.** JDBC 는 INSERT query 를 발생하고 해당 Entity 의 값을 받아와 1차 캐시에 저장하므로 SELECT query 를 발생하지 않고도 해당 필드에 접근할 수 있다.

<br>

## SEQUENCE 

SEQUENCE 전략이란 유일한 값을 순서대로 생성하는 DB object 로 Oracle, PostgreSQL, DB2, H2 datebase 에서 주로 사용한다.

```java
@Entity
@SequenceGenerator(
        name = "instagram",
        sequenceName = "insta_entity",
        initialValue = 3, allocationSize = 1
)
public class User {
	@Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE,
                generator = "instagram")
    private Long id;
    
    private String username;
}   
```

```@SequenceGenerator``` 으로 sequence 를 작성하고 id 필드의 PK 할당을 SEQUENCE 전략으로 선택한다. SEQUENCE 전략의 속성은 다음과 같다.

- name : 식별자 생성기 이름
- sequenceName : DB 에 등록되어 있는 sequence 이름
- initialValue : DDL 생성 시에만 사용되며, sequence DDL 을 생성할 때 처음 시작하는 번호
- allocationSize : sequence 1 번 호출 시 증가하는 수 (아래에서 자세히 설명)

initialValue 속성을 3 으로 설정했기 때문에 ```insta_entity``` 라는 이름을 갖는 시퀀스를 사용할 경우 3 부터 auto-increment 된다.

```User Entity``` 와 ```Post Entity``` 가 동일한 sequence 를 사용하면 어떤 PK 를 할당받는지 알기 위해 다음과 같이 작성한다.

```java
@Entity
@SequenceGenerator(
        name = "instagram",
        sequenceName = "insta_entity",
        initialValue = 3, allocationSize = 1
)
public class Post {
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE,
                generator = "instagram")
    private Integer id;
}
```

위와 같이 ```Post Entity``` 도 ```insta_entity``` 라는 이름을 갖는 시퀀스를 사용하도록 작성했다.

- username 이 ```id456``` 인 User 를 저장하고
- 2개의 Post 를 저장하고
- username 이 ```id456``` 인 User 를 저장했을 때 할당받는 PK 는 다음과 같다.

![](/_img/Spring/pk_mapping_sequence1.png)

```insta_eneity``` 라는 이름을 갖는 시퀀스의 initialValue 속성을 3 으로 설정했기 때문에 처음 저장된 ```User(id456)``` 는 3 을 할당받는다. 그다음 저장된 Post 는 4, 5 를 할당받는다. 마지막으로 저장된 ```User(id123)``` 은 6 을 할당받는다. 서로 다른 객체가 같은 시퀀스를 사용해도 중복되지 않고 할당되는 것을 알 수 있다.

![](/_img/Spring/pk_mapping_sequence2.png)

```Current value``` 는 마지막으로 할당한 수이다.
<br>

### initialValue 를 다르게 설정

만약 ```User Entity``` 는 **3** 부터 시작되고, ```Post Entity``` 는 **8** 부터 시작되도록 설정하기 위해 아래와 같이 작성했다고 하자.

```java
@Entity
@SequenceGenerator(
        name = "instagram",
        sequenceName = "insta_entity",
        initialValue = 3, allocationSize = 1
)
public class User {
	@Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE,
                generator = "instagram")
    private Long id;
    
    private String username;
}   
```
```java
@Entity
@SequenceGenerator(
        name = "instagram",
        sequenceName = "insta_entity",
        initialValue = 8, allocationSize = 1
)
public class Post {
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE,
                generator = "instagram")
    private Integer id;
}
```
위와 같이 작성하면 아래와 같이 ```HibernateException``` 예외가 발생한다.

```text
nested exception is org.hibernate.HibernateException:
Multiple references to database sequence [insta_entity] were encountered 
attempting to set conflicting values for 'initial value'.  Found [8] and [3]
```
```insta_entity``` 라는 시퀀스가 초기 값을 설정해야 하는데 3 과 8 을 **다중 참조**하기 때문에 예외가 발생해 실행할 수 없다. 그러므로 아래와 같이```sequenceName``` 속성을 수정해 서로 다른 시퀀스를 사용한다.

```java
@Entity
@SequenceGenerator(
        name = "instagram",
        sequenceName = "user_entity",
        initialValue = 3, allocationSize = 1
)
public class User {
	@Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE,
                generator = "instagram")
    private Long id;
    
    private String username;
}   
```
```java
@Entity
@SequenceGenerator(
        name = "instagram",
        sequenceName = "post_entity",
        initialValue = 8, allocationSize = 1
)
public class Post {
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE,
                generator = "instagram")
    private Integer id;
}
```

```sequenceName``` 속성을 서로 다르게 설정하면 서로 다른 시퀀스가 생성된다. 

![](/_img/Spring/pk_mapping_sequence3.png)

```User Entity``` 는 **3** 부터, ```Post Entity``` 는 **8** 부터 할당되는 것을 알 수 있다.
<br>

### DB 에 반영되는 과정

```java
EntityManagerFactory emf = Persistence.createEntityManagerFactory();
EntityManager em = emf.createEntityManager();
EntityTransaction tx = em.getTransaction();

User user = new User();
user.setUsername("nayoung");

em.persist(user); // PK 만 설정하고 DB 에는 저장되지 않은 상태
System.out.println(user.getId());  // 1차 캐시에 존재하므로 SELECT query 발생하지 않음

tx.commit();  // 이 시점에 DB 에 접근
```

JPARepository 를 사용하지 않고 EntityManager 만 사용한 코드이다.

IDENTITY 전략은 DB 에 접근해야만 PK 값을 할당받을 수 있다. 그러므로 PK 를 얻어 영속 상태를 만들기 위해 ```persist()``` 호출 시 자동으로 INSERT query 가 발생한다. INSERT query 가 발생해 DB 접근하면 PK 를 할당받고 영속 상태가 된다. 

SEQUENCE 전략은 매핑한 **sequence 에서 PK 를 얻어온다**. ```persist()``` 호출 시 매핑한 sequence 에서 PK 값을 받아와 PK 값을 지정하며 영속 상태가 된다. PK 만 설정하고 **DB 에는 저장되지 않은 상태**이다. commit 시점에 INSERT query 가 발생해 DB 에 저장된다. 

하지만 sequence object 도 DB 가 관리하기 때문에 DB 에서 PK 값을 가져온다. 즉, **sequence object 가 DB 에 접근해 PK 값을 가져오고**, 가져온 PK 값을 entity 에게 준다. 따라서 IDENTITY 전략과 마찬가지로 SEQUENCE 전략도 DB 에 접근해야만 PK 값을 얻을 수 있다.

또한 sequence 에서 PK 를 받아오기 위한 쿼리가 발생하고, commit 시점에 DB 에 INSERT 하기 위한 쿼리가 발생한다. 즉, 1개의 Entity 를 DB 에 INSERT 하기 위해 2번의 쿼리가 발생해 네트워크 사용량이 증가한다. 이를 해결하기 위해 allocationSize 를 설정한다.
<br>

### allocationSize

네트워크 사용량을 줄이기 위해 allocationSize 를 설정해야 하며 default 값은 50 이다.

PK 값을 받아올 때마다 sequence 에 접근하지 말고, allocationSize 에 설정한 크기만큼 **sequence 에서 미리 받아와 메모리에 저장**한다. default 값이 50 이므로 미리 50 개를 가져오기 때문에 **50 개를 모두 사용하기 전까지 쿼리를 발생하지 않고도 메모리에만 접근해 PK 값을 얻을 수 있다**.

![](/_img/Spring/pk_mapping_allocation_size.png)

sequence 에서 50 을 미리 가져왔다면 현재 값은 51 로 설정된다. 이는 다음 증가 시작점을 의미한다.

allocationSize 를 큰 수로 설정하면 네트워크 접근 비용을 더 줄일 수 있지만 웹서버가 다운되면 사용하다가 남은 값을 사용할 수 없고 sequence 의 현재 값부터 다시 시작해야 한다. 즉, 중간에 빈 구간이 생길 수 있어 allocationSize 를 적절한 값으로 설정해야 한다.