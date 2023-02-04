strategy pattern 에서 strategy 를 anonymous inner class 로 전달받아 사용하는 방식을 spring 에서 Template callback pattern 이라고 한다.

여기서 callback 이란 template 코드를 **call** 하고 template 의 **back** 에서 strategy 를 실행한다는 의미이다.
<br>

# log 추적 기능 구현

>- log 추적기 구현 커밋: https://github.com/evelyn82ny/Design-pattern/commit/935d6fb498de743f1fc48fc47e24aa22dad6dd84

상품을 주문하기 위해 HTTP GET 요청을 하면 처리되는 로직을 파악하기 위해 다음과 같이 log 를 출력하는 추적기를 적용시키려 한다. 동기화를 위해 ThreadLocal 로 구현한다.

InitDB 클래스로 ```apple: 34개```, ```candy: 56개``` 를 ItemRepository 에 저장했다. 재고가 있는 apple 을 주문하기 위해 ```http://localhost:8080/order?itemId=apple``` 을 작성했다면 위와 같이 정상적으로 log 가 남는다.

```text
[88e439e9] OrderController.order()
[88e439e9] |--> OrderService.order()
[88e439e9] |   |--> ItemRepository.order()
[88e439e9] |   |<-- ItemRepository.order() time = 1005ms
[88e439e9] |<-- OrderService.order() time = 1010ms
[88e439e9] OrderController.order() time = 1013ms
```

하지만 재고가 없는 kiwi 를 주문하기 위해 ```http://localhost:8080/order?itemId=kiwi``` 을 작성했다면 아래와 같이 log 에 예외 메시지가 출력된다.

```text
[bdbe3b56] OrderController.order()
[bdbe3b56] |--> OrderService.order()
[bdbe3b56] |   |--> ItemRepository.order()
[bdbe3b56] |   |<X- ItemRepository.order() time = 0ms, exception = java.lang.IllegalStateException: 상품 없음
[bdbe3b56] |<X- OrderService.order() time = 4ms, exception = java.lang.IllegalStateException: 상품 없음
[bdbe3b56] OrderController.order() time = 4ms, exception = java.lang.IllegalStateException: 상품 없음
```
<br>

# Template callback pattern

>- 템플릿 콜백 패턴 커밋: https://github.com/evelyn82ny/Design-pattern/commit/ea1a9b2277844b601c404dc0164e3bbd4974043c

위에서 본 HTTP 요청 처리 과정을 log 로 출력하는 기능을 Template callback pattern 으로 Controller, Service, Repository 에 적용한다.

## callback

```java
public interface TraceCallback <T>{
    T order();
}
```
변하는 부분인 callback 을 전달하는 interface 이다. order() 호출시 Repository 와 service 는 반환하지 않지만 Controller 는 "order success" 라는 메시지를 반환하기 때문에 제네릭을 사용한다.

## Template

```java
@Component
public class TraceTemplate {

    private final LogTrace trace;

    public TraceTemplate(LogTrace trace) {
        this.trace = trace;
    }

    public <T> T execute(String message, TraceCallback<T> callback) {
        TraceStatus status = null;
        try {
            status = trace.begin(message);
            T result = callback.order(); // 로직 호출
            trace.end(status);
            return result;
        } catch (Exception e) {
            trace.exception(status, e);
            throw e;
        }
    }
}
```
![png](/_img/Design-pattern/template_callback_pattern.png)

템플릿 역할인 TraceTemplate 클래스이다. execute 메서드에서 변경되는 코드인 interface 구현체를 parameter 로 전달받는다. 로직을 실행하다가 ```T result = callback.order();``` 지점에 오면 interface 구현체의 메서드를 실행시킨다.

callback 역할인 TraceCallback interface 에서 order 추상 메서드는 제네릭을 사용하기 때문에 이를 처리하기 위해 execute 메서드도 제네릭을 사용한다.
<br>

## application 에 적용

주로 anonymous inner class (익명 내부 클래스) 나 lambda 로 interface 구현체를 구현한다. 특정 구현체를 여러 곳에서 재사용하는 경우 class 로 만든다.

```java
public class OrderController {

    private final OrderService orderService;
    private final TraceTemplate template;

    @GetMapping("/order")
    public String order(String itemId) {
        return template.execute("OrderController.order()", () -> {
            orderService.order(itemId);
            return "order success";
        });
    }
}
```

```java
public class OrderService {

    private final ItemRepository itemRepository;
    private final TraceTemplate template;

    public void order(String itemId) {
        template.execute("OrderService.order()", () -> {
            itemRepository.order(itemId);
            return null;
        });
    }
}
```
<br>

# Template method 패턴과의 차이

![png](/_img/Design-pattern/template_method_vs_callback_structure.png)

Template **method** pattern 은 **변경되지 않는 템플릿 코드**는 abstract class 인 부모 클래스에 정의하고, **변경되는 부분**은 상속을 통해 오버라이딩으로 자식 클래스에서 구현한다. 상속을 사용하기 때문에 부모 클래스가 변경되면 자식 클래스도 영향을 받는다는 단점이 있다.

Template **callback** pattern 은 **변경되지 않는 템플릿 코드**는 template class 에 작성하고, **변경되는 부분**은 interface 로 정의해 원하는 interface 구현체를 template 에 전달하는 방식이다. **변경되지 않는 템플릿 코드**와 **변경되는 코드**를 완벽히 분리했기 때문에 서로에게 영향을 주지 않는다는 장점이 있다.

![png](/_img/Design-pattern/template_method_vs_callback_code.png)

template callback pattern 을 적용해 조금 더 간결하게 작성할 수 있었다. 하지만 log 추적 기능을 application 에 적용하기 위해 controller, service, repository 코드를 수정해야 한다는 문제가 있다.
