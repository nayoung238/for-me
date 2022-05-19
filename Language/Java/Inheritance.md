## Inheritance

```java
package abc;

public class User {

    private Integer id;
    public Integer managerId;

    User(Integer id, Integer managerId) {
        this.id = id;
        this.managerId = managerId;
    }
    
    public Integer getManagerId() {
        return managerId;
    }
}
```

## 같은 Package

```java
package abc;

public class Person extends User {
    
    private String name;
    private Integer age;
    
    public Person(String name, Integer age, Integer id, Integer managerId) {
        super(id, managerId);
        this.name = name;
        this.age = age;
    }
    
    public Integer getManagerId() {
        return managerId + 100;
    }
}
```
- 부모의 ```getManagerId()```를 재정의함
- ```Person.getManagerId()``` 를 호출하면 Person의 ```getManagerId()```가 실행
<br>

```java
package abc;

public class Cat extends User {
    
    private String name;

    public Cat(String name, Integer id, Integer managerId) {
        super(id, managerId);
        this.name = name;
    }
}
```
- ```Pet.getManagerId()```를 호출하면 부모인 User의 ```getManagerId()```가 실행

<br>

## 다른 Package

```java
package xyz;

public class Dog extends User {
    
    private String name;
    
    Dog(String name, Integer id, Integer managerId) {
        super(id, managerId);
        this.name = name;
    }
}
```
```java
package abc;

public class User {

    private Integer id;
    public Integer managerId;
    
    protected User(int id, int managerId) {
        this.id = id;
        this.managerId = managerId;
    }
    
    public Integer getManagerId() {
        return managerId;
    }
}
```
- 부모 클래스가 다른 Package에 존재
- 부모 생성자의 접근 제한자가 ```public``` 또는 ```protected``` 여야 생성 가능

<br>

## 접근 제한자

- ```public```: 외부 클래스도 자유롭게 사용 가능
- ```protected```: 같은 패키지나 자식 클래스만 사용 가능
- ```default```: 같은 패키지에서만 사용 가능
- ```private```: 자신을 제외한 모든 클래스가 사용 불가

아무런 설정을 하지 않을 경우 ```default```로 설정됨