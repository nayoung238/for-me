GOF 디자인 패턴에서 template method pattern 을 다음과 같이 정의했다.

> 작업에서 알고리즘의 골격을 정의하고 일부 단계를 하위 클래스로 연기합니다.
템플릿 메서드를 사용하면 하위 클래스가 알고리즘의 구조를 변경하지 않고도 알고리즘의 특정 단계를 재정의할 수 있습니다.

쉽게 말해 **변경되지 않는 부분**은 부모 클래스에 구현하고 **변경되는 부분**은 상속을 통해 오버라이딩으로 자식 클래스에서 구현하는 방식을 템플릿 메서드 패턴이라고 한다.

Controller, Service, Repository 에서 HTTP GET 요청 처리 과정을 log 로 출력하는 기능을 구현하려고 한다. 템플릿 메서드 패턴을 적용하기 전과 적용했을 때의 차이를 비교해보며 좋은 설계 방식을 학습한다.

# log 추적 기능 구현

>- log 추적기 구현 커밋: https://github.com/evelyn82ny/Design-pattern/commit/935d6fb498de743f1fc48fc47e24aa22dad6dd84
- 주문 시스템 구현 커밋: https://github.com/evelyn82ny/Design-pattern/commit/b5ad5fcd57d73f5b8fcac88bbb9b661022c20ae0

```text
[88e439e9] OrderController.order()
[88e439e9] |--> OrderService.order()
[88e439e9] |   |--> ItemRepository.order()
[88e439e9] |   |<-- ItemRepository.order() time = 1005ms
[88e439e9] |<-- OrderService.order() time = 1010ms
[88e439e9] OrderController.order() time = 1013ms
```
상품을 주문하기 위해 HTTP GET 요청을 하면 처리되는 로직을 파악하기 위해 위와 같이 log 를 출력하는 추적기를 적용시키려 한다. **동기화를 위해 ThreadLocal 로 구현**했다. 만약 ThreadLocal 로 구현하지 않으면 동기화가 해결되지 않으므로 서로 다른 요청 Log 가 섞인다.

InitDB 클래스로 ```apple: 34개```, ```candy: 56개``` 를 ItemRepository 에 저장했다.

```java
public class OrderController {

	private final OrderService orderService;
    private final LogTrace trace;

    @GetMapping("/order")
    public String order(String itemId) {
        TraceStatus status = null;
        try {
            status = trace.begin("OrderController.order()");
            orderService.order(itemId);
            trace.end(status);
            return "order success";
        } catch(Exception e) {
            trace.exception(status, e);
            throw e;
        }
    }
}
```

재고가 있는 apple 을 주문하기 위해 ```http://localhost:8080/order?itemId=apple``` 을 요청했다면 아래와 같이 정상적으로 log 가 출력된다.

```text
[88e439e9] OrderController.order()
[88e439e9] |--> OrderService.order()
[88e439e9] |   |--> ItemRepository.order()
[88e439e9] |   |<-- ItemRepository.order() time = 1005ms
[88e439e9] |<-- OrderService.order() time = 1010ms
[88e439e9] OrderController.order() time = 1013ms
```

재고가 없는 kiwi 를 주문하기 위해 ```http://localhost:8080/order?itemId=kiwi``` 을 요청했다면 아래와 같이 log 에 예외 메시지가 출력된다.

```text
[bdbe3b56] OrderController.order()
[bdbe3b56] |--> OrderService.order()
[bdbe3b56] |   |--> ItemRepository.order()
[bdbe3b56] |   |<X- ItemRepository.order() time = 0ms, exception = java.lang.IllegalStateException: 상품 없음
[bdbe3b56] |<X- OrderService.order() time = 4ms, exception = java.lang.IllegalStateException: 상품 없음
[bdbe3b56] OrderController.order() time = 4ms, exception = java.lang.IllegalStateException: 상품 없음
```
<br>

## template-method 패턴을 적용하기전의 문제점

해당 객체가 제공하는 고유 기능을 **핵심 기능**이라 하고, log 추적 기능같은 핵심 기능을 보조하기 위해 제공되는 기능을 **부가 기능** 이라고 한다.

log 추적 기능을 적용한 OrderController 와 OrderService 의 주문을 처리하는 order method 는 다음과 같다.

![png](/Design-pattern/_img/before_applying_template_method_pattern.png)

OrderController 의 order method 에서 핵심 기능은 **OrderService 의 order(itemId) 를 호출하는 것** 이고, 나머지 부분은 log 를 추적하기 위한 코드다. OrderSerive 도 마찬가지로 **ItemRepository 의 order(itemId) 를 호출하는 것**만 핵심 기능이다. 즉, 핵심 기능보다 부가 기능을 처리하기 위한 코드가 더 길어졌다.

또한 OrderController 와 OrderService 를 보면 핵심 기능을 처리하는 부분만 다르고 **부가 기능을 위한 코드는 중복**된다. 중복된 부분을 별도의 메서드로 처리하면 되지만 위 코드는 ```try - catch 문``` 이 존재하고 핵심 기능 코드가 중간에 존재하기 때문에 별도의 메서드로 처리하는 것이 쉽지 않다.
<br>

위 코드에서 ```변하는 것``` 과 ```변하지 않는 것``` 을 정의할 수 있다.

- 변하지 않는 것 : 부가 기능을 위한 코드 -> 공유 가능
- 변하는 것 : 핵심 기능을 위한 코드 -> 처리하는 로직이 다름

```변하는 것``` 과 ```변하지 않는 것``` 으로 분리해 모듈화하는 Template Method Pattern 을 적용해 위 문제를 해결할 수 있다.
<br>

# template-method 패턴 적용

>- template method-pattern 적용 커밋: https://github.com/evelyn82ny/Design-pattern/commit/84e8d1099bc4af7a69c4231508c48ec7e52ced1f

템플릿은 어느정도 정해진 형식을 말한다. 템플릿 메소드 패턴에서 템플릿도 **기준이 되는 틀**을 의미한다. **템플릿에 변하지 않는 부분**을 두고, **변하는 부분을 별도로 호출**하는 방식이다.

![png](/Design-pattern/_img/template_method_pattern.png)

즉, **부모 클래스에 변하지 않는 템플릿 코드인 log 추적 기능을 정의**하고, 객체에 따라 **변하는 코드인 핵심 기능 코드는 부모 클래스를 상속하는 자식 클래스**에서 오버라이딩하여 정의한다.

```java
public abstract class AbstractTemplate<T> {

    public T execute(String message) {
        TraceStatus status = null;
        try {
            status = trace.begin(message);
            T result = order(); // 변하는 부분
            trace.end(status);
            return result;
        } catch (Exception e) {
            trace.exception(status, e);
            throw e;
        }
    }

    protected abstract T order();
}
```

변하지 않는 부분인 log 추적 기능을 abstract 클래스에 작성한다. 그리고 변하는 부분인 핵심 기능은 order() 를 호출해서 처리한다.


## using Anonymous inner class

부모 클래스를 상속해 자식 클래스를 만들어도 되지만 객체마다 자식 클래스를 생성해야 되므로 익명 내부 클래스 (Anonymous inner class) 를 사용한다.

> 익명 내부 클래스 (Anonymous inner class) 란 상속한 자식 클래스에 대한 정의와 객체의 인스턴스 생성을 동시에 하는 이름없는 클래스로 일회용이다. 단 1개의 추상 클래스나 인터페이스를 바로 생성한다.

```java
@RestController
@RequiredArgsConstructor
public class OrderController {

	@GetMapping("/order")
	public String order(String itemId) {
		AbstractTemplate<String> template = new AbstractTemplate<>(trace) {
    		@Override
        	protected String order() {
        		orderService.order(itemId); // 핵심 기능
            	return "order success";
        	}
    	};
    	return template.execute("OrderController.order()");
	}
}
```

AbstractTemplate 의 order 메서드를 오버라이딩하여 OrderController 의 핵심 기능인 **OrderService 의 order(itemId) 를 호출**을 작성한다. 주문에 성공하면 ```order success``` 를 반환하므로 제네릭 반환 타입을 ```String``` 으로 작성한다.

```java
@Service
@RequiredArgsConstructor
public class OrderService {

	public void order(String itemId) {
		AbstractTemplate<Void> template = new AbstractTemplate<>(trace) {
    		@Override
        	protected Void order() {
        		itemRepository.order(itemId); // 핵심 기능
            	return null;
        	}
    	};
    	template.execute("OrderService.order()");
	}
}
```
마찬가지로 OrderSerive 도 오버라이딩한 order 메서드에 핵심 기능을 작성한다. OrderService 의 order 메서드는 반환할 내용이 없기 때문에 ```Void``` 타입으로 작성한다. 제네릭에 기본 타입인 ```void``` 는 설정할 수 없음을 주의해야 한다.
<br>

## 나아진점

![png](/Design-pattern/_img/apply_template_method_pattern.png)

이전에 작성했던 왼쪽 코드는 핵심 기능과 부가 기능이 함께 있다. template method pattern 을 적용한 오른쪽 코드는 부가 기능을 처리하는 템플릿을 호출하는 코드와 핵심 기능 코드만 있다.

코드를 **수정하는 시점**에 두 코드 중 어떤 코드가 좋은 설계인지 파악할 수 있다. 만약 log 추적 기능을 수정한다면 template-method pattern 을 적용한 오른쪽 설계는 **log 추척 기능을 구현한 AbstractTemplate 만 수정**하면 된다. 하지만 왼쪽 설계에서는 log 추적 기능을 작성한 모든 객체를 수정해야 한다.

즉, template method pattern 을 적용해 객체지향 설계 5원칙 중 **단일 책임 원칙 (SPR: Single Responsibility Principle)** 을 지켰다. 변경 지점을 하나로 모아 쉽게 처리할 수 있는 설계이기 때문이다.
<br>

## 문제점

![png](/Design-pattern/_img/template_method_pattern.png)

부모 클래스에 변경되지 않는 템플릿 코드를 정의하고, 변경되는 로직은 자식 클래스에 정의했다. 즉, template method pattern 은 상속을 사용한다. 

상속한다는 것은 자식 클래스가 부모 클래스를 의존하고 있다는 의미이다. 부모 클래스의 기능을 사용하지 않아도 자식 클래스는 의존해야 하며 부모 클래스가 변경되면 모든 자식 클래스는 변경 사항을 처리해야 한다.

template method pattern 을 적용한 Controller, Service, Repository 에 작성한 자식 클래스인 Anonymous inner class 를 보면 부모 클래스인 AbstractTemplate 의 기능을 전혀 사용하지 않기 때문에 좋은 설계가 아니다. 

이는 상속이 아닌 **위임 개념을 사용하는 Strategy pattern 으로 해결**할 수 있다.