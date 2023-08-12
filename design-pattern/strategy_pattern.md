![png](/_img/Design-pattern/template_method_vs_strategy_pattern.png)

template method pattern 은 ```변하지 않는 부분``` 을 부모 클래스에 정의하고 ```변하는 부분``` 을 자식 클래스에 정의한다. 하지만 상속을 사용하기 때문에 부모의 기능을 사용하지 않더라도 부모와 자식이 강하게 의존한다는 문제가 있다. 이를 해결하기 위한 Strategy pattern 은 ```변하지 않는 부분``` 과 ```변하는 부분``` 에 상속이 아닌 **위임**이라는 개념을 사용한다. 

전략 패턴 (Strategy pattern) 은 ```변하지 않은 템플릿 코드``` 를 Context class 에 정의하고 ```변하는 부분``` 을 Strategy **Interface** 에 정의한다. **문맥 (Context) 는 크게 변하지 않지만, 문맥속에서 일부 전략 (Strategy) 이 변한다**고 생각하면 된다.

GOF 디자인 패턴에서 정의한 전략 패턴은 다음과 같다.

> 알고리즘 제품군을 정의하고 각각을 캡슐화하여 상호 교환 가능하게 만들자. 전략을 사용하면 알고리즘을 사용하는 클라이언트와 독립적으로 알고리즘을 변경할 수 있다.

전략 패턴은 스프링에서 의존관계 주입 방식같은 **DI (Dependency Injection)** 방식과 **parameter 로 Strategy 를 넘겨주는** 방식이 있다.
<br>

# Dependency Injection 방식

>* DI 방식 코드 커밋: https://github.com/evelyn82ny/Design-pattern/commit/c3b376760783892f6c5484749177eec73cad7fd6

```text
[88e439e9] OrderController.order()
[88e439e9] |--> OrderService.order()
[88e439e9] |   |--> ItemRepository.order()
[88e439e9] |   |<-- ItemRepository.order() time = 1005ms
[88e439e9] |<-- OrderService.order() time = 1010ms
[88e439e9] OrderController.order() time = 1013ms
```
위와 같이 각 클래스가 어떤 로직인지 나타내는 log 를 남기고, log 를 남기기까지 걸리는 시간을 출력하는 코드를 작성하려고 한다.

- 어떤 로직인지 나타내는 log 는 각 객체마다 다르게 남겨야 하기 때문에 변하는 부분이므로 Strategy interface 에 정의한다.
- log 를 남기기까지 걸리는 시간을 출력하는 로직은 모든 객체가 사용하는 변하지 않는 템플릿 코드이므로 Context class 에 작성한다.
<br>

## Strategy

변하는 알고리즘은 Strategy Interface 에 구현한다.

```java
public interface Strategy {
    void call();
}
```
```java
@Slf4j
public class StrategyOrder implements Strategy {

    @Override
    public void call() {
        log.info("order logic");
    }
}

@Slf4j
public class StrategyRefund implements Strategy {

    @Override
    public void call() {
        log.info("refund logic");
    }
}
```
Strategy Interface 에서 변하는 부분인 call() 을 abstract method 로 작성했다. Strategy Interface 를 구현하는 StrategyOrder, StrategyRefund 는 abstract method 인 call() 를 override 하여 구현한다.
<br>

## Context

변하지 않는 로직인 템플릿 코드를 Context class 에 작성한다.

```java
@Slf4j
public class Context {

    private Strategy strategy;

    public Context(Strategy strategy) {
        this.strategy = strategy;
    }

    public void execute() {
        long startTime = System.currentTimeMillis();
        
        strategy.call();  // 해당 interface 에게 위임
        
        long endTime = System.currentTimeMillis();
        long resultTime = endTime - startTime;
        log.info("resultTime = {}", resultTime);
    }
}
```
Context 는 Strategy 필드를 가지고 있으며 원하는 Strategy 구현체를 주입하면 된다. ```strategy.call();``` 에서 해당 interface 구현체가 오버라이딩하여 구현한 메서드가 실행된다.

![png](/_img/Design-pattern/strategy_pattern_dependency_injection.png)

``` java
Context context1 = new Context(new StrategyOrder());
context1.execute();

Context context2 = new Context(new StrategyRefund());
context2.execute();
```
<br>

## anonymous inner class

```java
Strategy strategyOrder = new Strategy() {
	@Override
    public void call() {
    	log.info("order logic");
    }
};
Context context = new Context(strategyOrder);
context.execute();
```
Strategy pattern 에도 아래와 같이 익명 내부 클래스 (anonymous inner class) 를 적용할 수 있다.

```java
Context context = new Context(new Strategy() {
	@Override
    public void call() {
    	log.info("order logic");
    }
);
context.execute();
```
<br>

## lambda

>* lambda 코드 커밋: https://github.com/evelyn82ny/Design-pattern/commit/4dff702a727b4a9065529f7c94738692abe6f516

JAVA8 부터 anonymous inner class 를 lambda 로 변경할 수 있다.

```java
Context context1 = new Context(() -> log.info("order logic"));
context1.execute();

Context context2 = new Context(() -> log.info("refund logic"));
context2.execute();
```
**lambda 로 변경하려면 interface 에 method 가 1 개만 존재해야 한다.** Strategy interface 는 call method 1 개만 존재하므로 lambda 로 변경 가능하다.  
<br>

## template method 패턴과의 차이

![png](/_img/Design-pattern/template_method_vs_strategy_pattern.png)

template method pattern 은 abstract class 인 부모 클래스에 변하지 않는 템플릿 코드 (context) 를 메서드에 정의하고 **변하는 부분 (strategy) 을 추상 메서드로 설정**한다. 부모 클래스를 상속하는 자식 클래스가 추상 메서드를 override 하여 구현한다. 즉, Context 와 Strategy 가 분리되지 않고 의존적이다.

Strategy pattern 은 Context 와 Strategy 를 완전히 분리했다. Context 는 Strategy Interface 에만 의존하기 때문에 Strategy Interface 의 구현체를 변경하거나 새로 생성되어도 Context 는 아무런 영향을 받지 않는다.

### DI 방식의 단점

Context 에 원하는 Strategy 을 주입한 후 실행한다. 즉, 주입이 끝난 후 실행되기 때문에 strategy 를 변경하기 어렵다. 또한, Context 를 singleton 으로 사용한다면 동시성 (concurrency) 이슈 등 처리해야할 문제가 많다. 만약 strategy 를 실시간으로 변경해야 한다면 Context 를 여러개 생성해 처리해야 한다.
<br>

# parameter 전달 방식

>- parameter 전달 방식 커밋: https://github.com/evelyn82ny/Design-pattern/commit/b895165a39aaf50ea51aa091cc4a23b5cae2e154

```java
@Slf4j
public class Context {

	//private Strategy strategy;

    public void execute(Strategy strategy) {
        long startTime = System.currentTimeMillis();
        strategy.call();
        long endTime = System.currentTimeMillis();
        ...
   }
}
```
DI 방식에서 사용한 strategy field 를 갖지말고 Strategy 를 parameter 로 전달받는다.

![png](/_img/Design-pattern/strategy_pattern_dependency_injection_vs_parameter.png)

```java
Context context = new Context();
context.execute(new StrategyOrder());
context.execute(new StrategyRefund());
```

Context 를 실행하는 시점에 원하는 Strategy 을 전달한다. 실행 시점에 Strategy 를 설정할 수 있으니 1개의 Context 로 여러 전략을 실행할 수 있어 DI 방식 보다 유연하게 실행할 수 있다.
<br>

## anonymous inner class

```java
Context context = new Context();
context.execute(new Strategy() {
	@Override
    public void call() {
    	log.info("order logic");
    }
});
```
Strategy pattern 에도 익명 내부 클래스 (anonymous inner class) 를 사용할 수 있다.
<br>

## lambda

>- lambda 적용 커밋: https://github.com/evelyn82ny/Design-pattern/commit/236c285828a55ce86626788828898370202e8040

```java
Context context = new Context();
context.execute(() -> log.info("order logic"));
context.execute(() -> log.info("refund logic"));
```

Strategy interface 는 1개의 메서드만 있기 때문에 lambda 를 사용할 수 있다.
<br>

# 정리

![png](/_img/Design-pattern/strategy_pattern_dependency_injection_vs_parameter.png)

Strategy pattern 에서는 변하지 않는 템플릿 코드를 Context 에 작성하고, 변하는 코드를 Strategy Interface 에 구현한다.

Strategy 를 주입하는 방식은 실행 전에 주입하기 때문에 실행 후에는 strategy 를 신경쓰지 않아도 된다. 하지만 실행 후 strategy 를 변경할 수 없다는 단점이 있다.

Strategy 를 parameter 로 전달하는 방식은 실행할 때마다 strategy 를 유연하게 변경할 수 있다. 하지만 실행할 때마다 strategy 를 지정해야한다.

Spring 에서 사용하는 Template callback pattern 이 **strategy 를 Parameter 로 전달하는 Strategy pattern 의 변형**이다.
