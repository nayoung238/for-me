## Singleton pattern

- 1개의 객체만 생성 어디서든 참조할 수 있는 상태로 만드는 패턴
- 같은 instacne을 만들어내지 않고 동일한 instacne를 사용하게 함
- getInstance method로 모든 client에게 같은 instance을 반환
- new로 1번만 생성하기 때문에 메모리 낭비를 방지
- 메모리 낭비를 막기 위해 사용하거나 데이터 공유를 위해 사용
- Spring은 기본적으로 Bean을 Singleton으로 생성 및 관리
<br>

- 많은 데이터를 공유할 경우 다른 클래스의 instance 간의 결합도가 높아지는 문제
  - SOLID 원칙 중 DIP(Dependency inversion principle)과 OCP(open/closed principle) 을 위반
- 데이터 공유이므로 Synchronization 문제를 해결해야 함

<br>

## Race condition 발생

```java
public class Apple {
    private static Apple apple = null;
    private Apple(){}
    
    public static Apple getApple() {
        if(apple == null) {
            return new Apple();
        }
        return apple;
    }
}
```
- ```Thread_A```가 Apple instance가 아직 **null**임을 확인
- 그러므로 Instance을 만들어야 함
- 하지만 이 과정에서 ```Thread_B```도 instance가 아직 **null**임을 확인하고 Instance 생성을 시도
- 결과적으로 Apple instance가 2개 만들어짐

**Multi-Thread 환경에서** Race condition으로 인해 Singleton이 충족되지 않은 경우가 발생할 수 있음<br>

```java
public class Apple {
    private static Apple apple = null;
    private Apple(){}
    
    public static Apple getApple() {
        if(apple == null) {
            try {
                Thread.sleep(1);
            } catch(InterruptedException e) {}
            apple = new Apple();
        }
        return apple;
    }
}
```

```sleep()``` 을 이용해 Singleton 을 막아볼 수 있고 해결 방법은 다음과 같다.

<br>

## Solution1: static variable 바로 초기화

```java
public class Apple {
    private static Apple apple = new Apple();
    private Apple(){}
    
    public static Apple getApple() {
        return apple;
    }
}
```
- 클래스가 메모리에 로딩될 때 초기화
- 프로그램이 종료할 때까지 메모리에 존재하며 모든 객체에서 참조 가능

<br>

## Solution2: synchronized 사용

```java
public class Apple {
    private static Apple apple = null;
    private Apple(){}
    
    public static synchronized Apple getApple() {
        if(apple == null) {
            apple = new Apple();
        }
        return apple;
    }
}
```
- ```synchronzied```를 사용해 getInstance method를 임계 영역으로 변경
- Lock을 사용하기 때문에 속도가 느려져 권장하지 않는 방법

<br>

## Solution3:  Holder에 의한 초기화

```java
public class Apple {
    private Apple(){}
    
    private static class InnerInstanceClazz {
        private static final Apple instance = new Apple();
    }
    
    public static Apple getInstance() {
        return InnerInstanceClazz.instance;
    }
}
```
- JVM의 Class loader 메커니즘과 Class가 로드되는 시점을 이용
- JVM의 클래스 초기화 과정에서 보장되는 원자적 특성을 이용 (JVM이 알아서 잘 처리하도록)
- ```InnerInstanceClazz``` 내의 instance가 **static** 이므로 클래스 로딩 시점에 1번만 호출되고 **final**을 사용해 다른 값으로 할당되는 것을 막음
- 가장 많이 사용되는 방법

<br>

### Reference
 
- https://gmlwjd9405.github.io/2018/07/06/singleton-pattern.html
- https://jeong-pro.tistory.com/86