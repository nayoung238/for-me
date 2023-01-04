특정 서비스를 사용하는 사용자를 클라이언트라고 한다. 웹 브라우저가 서버에게 무언가를 요청한다면 이 관계에서 웹 브라우저는 클라이언트가 된다. 이처럼 클라이언트와 서버의 개념은 넓게 사용된다. 즉, **서버에 필요한 것을 요청**하는 입장이 클라이언트가 되고, 서버는 **클라이언트의 요청을 처리**한다.

클라이언트는 서버에게 원하는 것을 **직접 호출**할 수 있다. 이말은 **간접 호출**도 가능하다는 것이다.

![png](/design-pattern/_img/proxy.png)

클라이언트는 대리자를 통해 서버에게 간접적으로 요청할 수 있는데 이 대리자를 **proxy** 라고 한다. 프록시 객체로 다음과 같은 기능을 구현할 수 있다.

- 접근 제어 : 클라이언트가 서버 접근 권한이 없다면 미리 차단해 서버 접근을 막는다.
- 캐싱 : 캐시에 있는 데이터를 요청하면 서버에 접근하지 않고 반환하기 때문에 보다 빠르게 얻을 수 있다.
- 부가 기능 추가 : 기존 기능 로직을 수정하지 않고 부가 기능을 추가할 수 있다.

클라이언트는 해당 요청의 처리 과정을 알 필요가 없다. 그저 요청한 결과를 제대로 얻기만 하면된다. 즉, 클라이언트는 원하는 요청을 서버가 처리했는지, 프록시가 처리했는지 구분가지 않게 해야 한다. 
<br>

proxy pattern 과 decorator patter 은 둘다 proxy 사용하지만 두 패턴은 의도 (intent) 가 다르다. 프록시 패턴은 **접근 제어**가 목적이고, 데코레이터 패턴은 **새로운 기능 추가**가 목적이다.

```interface 와 구현 클래스```, ```interface 가 없는 concrete 클래스``` 에 프록시를 적용한다.

# interface 기반

![png](/design-pattern/_img/based_on_interface(1).png)

클라이언트가 서버에게 요청했는지, 프록시에게 요청했는지 구분가지 않게 하려면 **서버와 프록시는 같은 interface 를 사용**해야 한다. 그렇기 때문에 DI (dependency injection) 로 서로가 대체 가능하며, 이는 서버 객체를 프록시 객체로 변경해도 서비스 동작 방식에 아무런 문제가 발생하지 않음을 의미한다. 

Runtime 시점에 DI (dependency injection) 를 통해 객체를 주입하므로 어떤 의존관계로 변경해도 클라이언트 코드는 전혀 변경되고 클라이언트 입장에선 변경 사실을 인지할 수 없다.

## controller, service, repository 구현

>- interface 기반 구현 커밋 : https://github.com/evelyn82ny/design-pattern/commit/429b35433a43fe36a8f787459692904f16869c2b

![png](/design-pattern/_img/based_on_interface(2).png)

```java
@RequestMapping
@ResponseBody
public interface OrderControllerV1 {

    @GetMapping("/v1/order")
    String order(@RequestParam("itemId") String itemId);
}
```

```java
@Slf4j
public class OrderControllerV1Impl implements OrderControllerV1 {

    private final OrderServiceV1 orderService;

    public OrderControllerV1Impl(OrderServiceV1 orderService) {
        this.orderService = orderService;
    }

    @Override
    public String order(String itemId) {
        orderService.order(itemId);
        return "order success";
    }
}
```
위와 같이 Interface 기반으로 controller, service, repository 를 구현한다.
<br>

## proxy 적용

>- interface 기반에 proxy 적용 커밋: https://github.com/evelyn82ny/design-pattern/commit/6258cb3ea5b41d2492475ef356d94e237710d367

![png](/design-pattern/_img/based_on_interface(3).png)

Interface 기반으로 만든 구현 클래스에 프록시를 적용하려면 Controller, Service, Repository 객체에 대해 각각의 Proxy 를 만든다.

![png](/design-pattern/_img/based_on_interface(4).png)

Runtime 시점에 클라이언트가 프록시를 사용하도록 DI 를 설정한다. OrderController 에 대한 Proxy 인 ```OrderControllerInterfaceProxy``` 는 다음과 같다.

```java
@RequiredArgsConstructor
public class OrderControllerInterfaceProxy implements OrderControllerV1 {

    private final OrderControllerV1 target; // 비즈니스 로직을 처리하는 객체
    private final LogTrace logTrace;

    @Override
    public String order(String itemId) {
        TraceStatus status = null;
        try {
            status = logTrace.begin("OrderController.order()");
            String result = target.order(itemId);  // 비즈니스 로직
            logTrace.end(status);
            return result;
        } catch (Exception e) {
            logTrace.exception(status, e);
            throw e;
        }
    }
}
```
```OrderControllerInterfaceProxy``` 로 부가 기능인 **로그 추적** 기능을 수행하고, 실제 주문하는 비즈니스 로직은 Proxy 객체가 아닌 ```OrderControllerV1``` 구현 객체에서 수행된다.

![png](/design-pattern/_img/based_on_interface(5).png)

그러므로 ```OrderControllerInterfaceProxy``` 는 ```OrderControllerV1``` 객체를 **target** 으로 참조해 비즈니스 로직을 수행하는 시점에 **target 을 호출**한다.

위 코드를 보면 ```주문을 처리하는 비즈니스 기능``` 과 ```로그를 남기는 부가 기능``` 이 완벽히 분리된다는 장점이 있다.
<br>

## Bean 등록

```java
@Configuration
public class AppV1InterfaceConfig {

    @Bean
    public OrderControllerV1 orderControllerV1(LogTrace logTrace) {
        OrderControllerV1Impl controllerV1Impl = new OrderControllerV1Impl(orderServiceV1(logTrace));
        return new OrderControllerInterfaceProxy(controllerV1Impl, logTrace);
    }

    @Bean
    public OrderServiceV1 orderServiceV1(LogTrace logTrace) {
        OrderServiceV1Impl orderServiceV1Impl = new OrderServiceV1Impl(itemRepositoryV1(logTrace));
        return new OrderServiceInterfaceProxy(orderServiceV1Impl, logTrace);
    }

    @Bean
    public ItemRepositoryV1 itemRepositoryV1(LogTrace logTrace) {
        ItemRepositoryV1Impl itemRepositoryV1Impl = new ItemRepositoryV1Impl();
        return new ItemRepositoryInterfaceProxy(itemRepositoryV1Impl, logTrace);
    }
}
```
![png](/design-pattern/_img/proxy_pattern_bean.p)

```OrderControllerV1Impl```, ```OrderServiceV1Impl``` 같은 비지니스 로직을 처리하는 객체를 Spring bean 으로 등록하지 않고, 프록시 객체로 생성한 ```OrderControllerInterfaceProxy```, ```OrderServiceInterfaceProxy``` 등 을 실제 **Spring bean 으로 등록**하는 구조이다. 즉, 구현 객체와 프록시 객체 모두 자바 heap 메모리에 올라가지만 프록시 객체만 스프링 컨테이너가 관리한다.

interface 를 구현한 ```OrderControllerV1Impl``` 구현 객체를 Spring Bean 으로 등록하지 않아도 ```OrderControllerInterfaceProxy``` 프록시 객체가 **target** 으로 참조하고 있기 때문에 비즈니스 로직을 호출할 수 있다.


## 결과

>- InitDB2 코드: https://github.com/evelyn82ny/design-pattern/blob/master/src/main/java/nayoung/designpattern/app/proxy/InitDB2.java

```java
public class InitDB2 {

    private final ItemRepositoryV1 itemRepositoryV1;
    @PostConstruct
    public void init() {
        itemRepositoryV1.save("apple", 34L);
        itemRepositoryV1.save("candy", 56L);
    }
}
```

```apple 34개```, ```candy 56개``` 를 repository 에 저장하고 ```http://localhost:8080/v1/order?itemId=apple``` 을 요청하면 정상적으로 log 가 찍힌다.

```text
[88e439e9] OrderController.order()
[88e439e9] |--> OrderService.order()
[88e439e9] |   |--> ItemRepository.order()
[88e439e9] |   |<-- ItemRepository.order() time = 1005ms
[88e439e9] |<-- OrderService.order() time = 1010ms
[88e439e9] OrderController.order() time = 1013ms
```

## 문제점

![png](/design-pattern/_img/proxy_pattern_based_on_interface_structure.png)

Proxy 객체를 사용해 ```주문을 처리하는 비즈니스 기능``` 과 ```로그를 남기는 부가 기능``` 을 완벽히 분리했다. 하지만 모든 객체에 대한 Proxy 객체를 만들어야 한다는 단점이 있다. 이는 **JDK Dynamic proxy** 로 해결가능하다.
<br>

# concrete class 기반

![png](/design-pattern/_img/based_on_concrete_class(1).png)

Concrete class (구체 클래스) 를 상속해 Proxy 객체를 만든다. Interface 기반과 다른점은 interface 구현이 아닌 상속을 해야하며 다른 것은 모두 동일하다.

구체 클래스를 상속해 만든 Proxy 객체가 ```로그를 남기는 부가 기능``` 을 구현하고 구체 클래스에 ```주문을 처리하는 비즈니스 기능``` 을 구현한다. Interface 기반과 마찬가지로 Proxy 객체는 구체 클래스를 참조해 비즈니스 기능을 수행해야 되는 시점에 구체 클래스를 호출한다.

## controller, service, repository 구현

>- 해당 커밋: https://github.com/evelyn82ny/design-pattern/commit/1d7434b66141cd5c109b19f3b6500471401c20f2

```java
@RequestMapping
@ResponseBody
public class OrderControllerV2 {

    private final OrderServiceV2 orderService;

    public OrderControllerV2(OrderServiceV2 orderService) {
        this.orderService = orderService;
    }

    @GetMapping("/v2/order")
    public String order(String itemId) {
        orderService.order(itemId);
        return "order success";
    }
}
```
Interface 가 없는 controller, service, repository 를 구현한다.


## proxy 적용

>- 해당 커밋: https://github.com/evelyn82ny/design-pattern/commit/429bf1ed01ee72f485c04e0e2475991ce31774f8

위에서 생성한 ```OrderControllerV2```, ```OrderServiceV2```, ```ItemRepositoryV2``` 를 **상속**하여 Proxy 객체를 만든다.

```java
public class OrderControllerV2ConcreteProxy extends OrderControllerV2 {

    private final OrderControllerV2 target;
    private final LogTrace logTrace;

    public OrderControllerV2ConcreteProxy(OrderControllerV2 target, LogTrace logTrace) {
        super(null);
        this.target = target;
        this.logTrace = logTrace;
    }

    @Override
    public String order(String itemId) {
        TraceStatus status = null;
        try {
            status = logTrace.begin("OrderController.order()");
            String result = target.order(itemId);  // 비즈니스 로직
            logTrace.end(status);
            return result;
        } catch (Exception e) {
            logTrace.exception(status, e);
            throw e;
        }
    }
}
```

interface 기반과 마찬가지로 Proxy 객체는 **target** 으로 실제 객체를 참조한다. 부가 기능을 실행하다가 비즈니스 기능을 실행해야할 시점에 **실제 객체인 target** 을 호출한다.

클래스를 상속하여 만들기 때문에 **부모 클래스의 생성자가 호출**된다. 프록시 객체는 비즈니스 로직을 수행할 때 **target** 을 호출하고, **부모 객체의 기능을 사용하지 않기**때문에 ```super(null)``` 을 작성한다.

이처럼 부모 객체의 기능을 사용하지 않아도 상속해야된다는 문제점이 있다. 

## Bean 등록

>- 해당 커밋: https://github.com/evelyn82ny/design-pattern/commit/254c5c7773e95a683d603b666960a72d049ed4da

```java
@Configuration
public class AppV2ConcreteProxyConfig {

    @Bean
    public OrderControllerV2 orderControllerV2(LogTrace logTrace) {
        OrderControllerV2 orderController = new OrderControllerV2(orderServiceV2(logTrace));
        return new OrderControllerV2ConcreteProxy(orderController, logTrace);
    }

    @Bean
    public OrderServiceV2 orderServiceV2(LogTrace logTrace) {
        OrderServiceV2 orderService = new OrderServiceV2(itemRepositoryV2(logTrace));
        return new OrderServiceV2ConcreteProxy(orderService, logTrace);
    }

    @Bean
    public ItemRepositoryV2 itemRepositoryV2(LogTrace logTrace) {
        ItemRepositoryV2 itemRepository = new ItemRepositoryV2();
        return new ItemRepositoryV2ConCreteProxy(itemRepository, logTrace);
    }
}
```

![png](/design-pattern/_img/based_on_concrete_class(1).png)
<br>

## 결과

![png](/design-pattern/_img/based_on_concrete_class(2).png)

Concrete class 를 상속해 만든 Proxy 객체를 통해 ```주문을 처리하는 비즈니스 기능``` 과 ```로그를 남기는 부가 기능``` 이 완벽히 분리했다. Interface 기반과 다른점은 interface 구현이 아닌 상속을 해야하며 다른 것은 모두 동일하다. 즉, 클래스를 상속하기 때문에 부모 객체의 기능을 사용하지 않아도 가지고 있어야 한다는 단점이 존재한다.