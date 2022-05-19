## Abstract class (추상 클래스)

- 추상 클래스를 상속하는 실체 클래스는 추상 클래스의 abstract method 를 구현해야한다.
- 추상 클래스는 abstract method를 구현하지 않는다.
- 추상 클래스는 어느정도의 틀만 잡고 실체 클래스에서 구체적으로 작성한다. (다형성: 같은 기능이지만 다른 결과를 도출할 수 있음)
- 추상 클래스는 추상적이기 때문에 **객체를 생성할 수 없고 상속**만 가능하다.
- 추상 클래스를 상속하는 실체 클래스는 추상 클래스의 abstract method 를 구현해야하기 때문에 접근 제한자를 ```public``` 이나 ```protected``` 만 사용 가능

```java
package abc;

public abstract class User {
    
    private Integer id;
    private Integer managerId;
    
    protected User(Integer id, Integer managerId) {
        this.id = id;
        this.managerId = managerId;
    }

    public abstract void publicMethod();
    protected abstract void protectedMethod();
}
```
- 접근 제한자가 ```public```, ```protected```인 abstract method가 있다.
- 해당 클래스를 상속 중인 자식 클래스는 2개의 abstract method를 Override 한다.
- abstract method에 접근 제한자 ```default```, ```private``` 을 적용할 수 없다.(구현할 수 없기 때문에)
- abstract 를 변수에 적용하지 못한다.
<br>

### 같은 package

```java
package abc;

public class Person extends User {
    
    private String name;
    private Integer age;
    
    Person(String name, Integer age, Integer id, Integer managerId) {
        super(id, managerId);
        this.name = name;
        this.age = age;
    }

    @Override
    public void publicMethod() {
        System.out.println("Person에서 Abstract publicMethod 실행");
    }

    @Override
    protected void protectedMethod() {
        System.out.println("Person에서 Abstract protectedMethod 실행");
    }
}
```
- 추상 클래스의 모든 Abstract method를 구현

### 다른 package

```java
package xyz;

public class Pet extends User {

    private String type;
    private Integer age;

    Pet(String type, Integer age, Integer id, Integer managerId) {
        super(id, managerId);
        this.type = type;
        this.age = age;
    }

    @Override
    public void publicMethod() {
        System.out.println("Pet에서 Abstract publicMethod 실행");
    }

    @Override
    protected void protectedMethod() {
        System.out.println("Pet에서 Abstract protectedMethod 실행");
    }
}
```
- 다른 Package에 있는 실체 클래스도 abstract method를 구현

<br>

## 추상 클래스 장점

### 유지 보수가 좋다.

- 공통 기능을 구현한 여러 실체 클래스를 골라 사용 가능
- 예를 들어 자동차라는 추상 클래스와 자동차A, 자동차B라는 구체 클래스가 있을 경우
- 자동차A를 쓰다가 자동차B로 바꾸고 싶다면 field나 method 변경없이 그냥 사용하면 된다.
- 이는 공통된 field 와 method를 구현했기 때문에 유지보수가 높고 통일성이 유지되기 때문

### 어느 정도 정해진 틀

- 추상 클래스를 상속한 경우 abstract method만 구현하면 된다.
- 추상 클래스의 abstract method를 제외한 method를 그대로 사용하면 된다.

### 다형성

- 같은 기능이지만 다른 결과를 도출할 수 있음

