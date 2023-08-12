접근 제어를 위한 Proxy pattern, 새로운 기능 추가를 위한 Decorator pattern은 둘다 Proxy를 사용해 핵심 기능과 부가 기능을 분리할 수 있다. 하지만 다음과 같은 문제가 발생한다.

- 프록시를 적용하기 위해 적용할 대상만큼의 프록시 객체 생성
- 같은 기능을 처리하는 프록시를 적용하려면 적용할 대상만큼의 프록시 클래스 생성

즉, 같은 기능을 처리하는 프록시가 적용하는 객체가 다르다는 이유로 중복되는 코드가 생성되고, 프록시를 적용하는 만큼 프록시 객체를 만들어야 되는 문제가 발생한다.

이를 해결한 것이 JDK Dynamic proxy와 CGLIB 라이브러이이다. Interface 기반에는 JDK Dynamic proxy를 적용하고 Concrete class에는 CGLIB를 사용한다.

# JDK Dynamic Proxy

![png](/_img/Design-pattern/JDK_dynamic_proxy(2).png)

Proxy 객체를 Runtime에 동적으로 만들어주기 때문에 개발자가 직접 작성하지 않아도 된다. JDK Dynamic proxy는 **Interface 기반**으로 proxy를 만들기 때문에 Interface를 구현한 객체에 적용된다.

> * Interface 기반으로 객체를 생성한 커밋: https://github.com/evelyn82ny/Design-pattern/commit/429b35433a43fe36a8f787459692904f16869c2b
* JDK Dynamic proxy test 커밋: https://github.com/evelyn82ny/Design-pattern/commit/22c29fda2f0d2119d383aa53f046cc79c8348401


```java
package nayoung.designpattern.app.proxy.basedOnInterface;

public interface OrderServiceV1 {
    void order(String itemId);
}
```
```java
package nayoung.designpattern.app.proxy.basedOnInterface;

public class OrderServiceV1Impl implements OrderServiceV1 {

    private final ItemRepositoryV1 itemRepository;

    public OrderServiceV1Impl(ItemRepositoryV1 itemRepository) {
        this.itemRepository = itemRepository;
    }

    @Override
    public void order(String itemId) {
        itemRepository.order(itemId);
    }
}
```

## InvocationHandler

>- InvocationHandler 커밋: https://github.com/evelyn82ny/design-pattern/commit/258518775e7ca0b9149500a51c7d43d5a7b30f60

JDK Dynamic Proxy를 적용할 로직은 **공통 로직**이다. 이를 위해 JDK Dynamic Proxy가 제공하는 ```InvocationHandler Interface```를 구현한다. 

```java
package java.lang.reflect;

public interface InvocationHandler {
	public Object invoke(Object proxy, Method method, Object[] args)
        throws Throwable;
}
```
- ```Object proxy``` : Proxy 자신
- ```Method method``` : 호출할 Method
- ```Object[] args``` : Method 호출할 때 전달할 인수
<br>

LogTrace로직에 JDK Dynamic Proxy를 적용하기 위해 ```InvocationHandler Interface``` 를 구현해 작성한다.

```java
public class LogTraceHandler implements InvocationHandler {

    private final Object target;
    private final LogTrace logTrace;

    public LogTraceHandler(Object target, LogTrace logTrace) {
        this.target = target;
        this.logTrace = logTrace;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        TraceStatus status = null;
        try {
            String message = method.getDeclaringClass().getSimpleName() + "." 
            				+ method.getName() + "()";
            status = logTrace.begin(message);

            Object result = method.invoke(target, args);  // 비지니스 로직

            logTrace.end(status);
            return result;
        } catch (Exception e) {
            logTrace.exception(status, e);
            throw e;
        }
    }
}
```

```method.invoke(target, args);``` 를 호출하면 Java Reflection을 사용해서 target instance의 method를 실행한다.
<br>

## Dynamic Proxy 생성

>- newProxyInstance 커밋: https://github.com/evelyn82ny/Design-pattern/commit/dbae9c6adb8f28a25fcd423a6e8d00a75fe2fd0f

```java
@Configuration
public class JdkDynamicProxyConfig {

    @Bean
    public OrderControllerV1 orderControllerV1(LogTrace logTrace) {
        OrderControllerV1 orderController = new OrderControllerV1Impl(orderServiceV1(logTrace));
        OrderControllerV1 proxy = (OrderControllerV1) Proxy.newProxyInstance(
        							OrderControllerV1.class.getClassLoader(),
                					new Class[]{OrderControllerV1.class}, 
                                    new LogTraceHandler(orderController, logTrace));
        return proxy;
    }

    @Bean
    public OrderServiceV1 orderServiceV1(LogTrace logTrace) {
        OrderServiceV1 orderService = new OrderServiceV1Impl(itemRepositoryV1(logTrace));
        OrderServiceV1 proxy = (OrderServiceV1) Proxy.newProxyInstance(
        						OrderServiceV1.class.getClassLoader(),
                				new Class[]{OrderServiceV1.class}, 
                                new LogTraceHandler(orderService, logTrace));
        return proxy;
    }
    
    // repository 생략
}    
```
```newProxyInstance```로 Dynamic proxy를 생성한다. ClassLoader 정보, Interface, handler 로직을 작성하면 해당 Interface를 기반으로 Dynamic proxy를 생성해 반환한다. 해당 Proxy 정보를 출력하면```class com.sun.proxy.$Proxy11```으로 출력된다.

JDK Dynamic proxy가 적용되어 처리되는 과정은 다음과 같다.

![png](/_img/Design-pattern/JDK_dynamic_proxy(1).png)

1. client 는 주문을 처리하고 Log 를 출력하기 위해 ```orderAndPrintLog()``` 를 호출한다.
2. ServiceV1Impl에 JDK dynamic proxy를 적용했기 때문에 Proxy가 존재한다. 이 JDK dynamic proxy는 ```InvocationHandler.invoke()``` 를 호출하고 LogTraceHandler 구현체의 ```LogTraceHandler.invoke()``` 가 호출된다.
3. LogTraceHandler가 내부 로직을 수행하고 ```method.invoke(target, args);``` 를 통해 실제 객체인 OrderServiceV1(Impl)을 호출한다.
4. OrderServiceV1(Impl)의 ```orderAndPrintLog()``` 을 실행하고 LogTraceHandler로 되돌아온다.


### 실행 결과

로그를 출력하기 위해 ```http://localhost:8080/v1/order-printLog?itemId=apple``` 요청하면 아래와 같이 정상적으로 로그가 출력된다.

```text
[853605e1] OrderControllerV1.orderAndPrintLog()
[853605e1] |--> OrderServiceV1.orderAndPrintLog()
[853605e1] |   |--> ItemRepositoryV1.orderAndPrintLog()
[853605e1] |   |<-- ItemRepositoryV1.orderAndPrintLog() time = 1005ms
[853605e1] |<-- OrderServiceV1.orderAndPrintLog() time = 1006ms
[853605e1] OrderControllerV1.orderAndPrintLog() time = 1006ms
```
<br>

## log 출력하지 않는 API 생성

>- 해당 커밋: https://github.com/evelyn82ny/Design-pattern/commit/c30e104512896baa5be0b2e2a8eb6086f3f26800

```java
public interface OrderControllerV1 {

    @GetMapping("/v1/order-printLog")
    String orderAndPrintLog(@RequestParam("itemId") String itemId);

    @GetMapping("/v1/order-noLog")
    String orderAndNoLog(@RequestParam("itemId") String itemId);
}
```

주문 처리를 상황에 맞게 사용하기 위해 **처리되는 과정을 log 로 출력**하고 싶을 땐 ```orderAndPrintLog()``` 를, **log 출력 없이** 주문만 처리할 때는 ```orderAndNoLog()``` 를 호출하도록 분리한다.

즉, log 를 출력할 경우 ```http://localhost:8080/v1/order-printLog?itemId=아이템명``` 을 요청하고, log 를 출력하지 않고 주문을 처리할 경우 ```http://localhost:8080/v1/order-noLog?itemId=아이템명``` 을 요청하면 된다.


### 문제점 

JDK dynamic proxy 를 적용한 코드에서 로그를 출력하지 않기 위해 ```http://localhost:8080/v1/order-noLog?itemId=apple``` 요청해도 아래와 같이 로그가 찍힌다.

```text
[99a1423e] OrderControllerV1.orderAndNoLog()
[99a1423e] |--> OrderServiceV1.orderAndNoLog()
[99a1423e] |   |--> ItemRepositoryV1.orderAndNoLog()
[99a1423e] |   |<-- ItemRepositoryV1.orderAndNoLog() time = 1000ms
[99a1423e] |<-- OrderServiceV1.orderAndNoLog() time = 1001ms
[99a1423e] OrderControllerV1.orderAndNoLog() time = 1002ms
```
JDK dynamic proxy가 모든 Method에 적용되기 때문에 noLog 전용으로 만든 API도 log가 출력된다는 문제가 발생한다. 이를 해결하기 위해 필터를 설정한다.

### 필터 설정

>- 해당 커밋: https://github.com/evelyn82ny/Design-pattern/commit/3f94842723ead39662c9e17640da0f2b71a3eee3


```java
@Configuration
public class JdkDynamicProxyConfig {

    public static final String[] PATTERNS = {"*NoLog*"};
    
    @Bean
    public OrderControllerV1 orderControllerV1(LogTrace logTrace) {
        OrderControllerV1 orderController = new OrderControllerV1Impl(orderServiceV1(logTrace));
        OrderControllerV1 proxy = (OrderControllerV1) Proxy.newProxyInstance(
        							OrderControllerV1.class.getClassLoader(),
                					new Class[]{OrderControllerV1.class},
                					new LogTraceHandler(orderController, logTrace, PATTERNS));
        return proxy;
    }
    // 아래 생략
}    
```

```java
public class LogTraceHandler implements InvocationHandler {
	private final Object target;
    private final LogTrace logTrace;
    private final String[] patterns;
    
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {

        String methodName = method.getName();
        if(PatternMatchUtils.simpleMatch(patterns, methodName)) {
            return method.invoke(target, args);
        }
        
        // 로그 출력하는 로직 생략
}
```

Spring이 제공하는 ```PatternMatchUtils.simpleMatch```을 사용해 필터를 적용한다.

- ```abc``` : 정확히 ```abc``` 로 매칭되면 true
- ```abc*``` : ```abc``` 로 시작하면 true
- ```*abc``` : ```abc``` 로 끝나면 true
- ```*abc*``` : ```abc``` 가 포함되어있다면 true


```*NoLog*``` 로 작성했기 때문에 실행하는 **method** 이름에 ```NoLog``` 가 속해 있다면 true 를 반환한다. 즉, 로그를 출력하는 로직으로 내려가지 않고 ```method의 invoke```를 호출한다. 

### 실행 결과

로그를 출력하기 위해 ```http://localhost:8080/v1/order-printLog?itemId=apple``` 을 요청하면 로그가 찍히고, ```http://localhost:8080/v1/order-noLog?itemId=apple``` 을 요청하면 로그는 찍히지 않고 주문만 처리된다.
<br>

## JDK Dynamic proxy 정리

#### JDK Dyanmic proxy 적용 전

![](https://velog.velcdn.com/images/evelyn82ny/post/5c172f73-a533-4031-8474-a71678ab3dcb/image.png)

Interface 기반에 JDK Dyanmic proxy를 적용하지 않고 직접 Proxy 생성해 적용하면 위와 같은 구조가 된다. 프록시를 적용하고자 하는 객체에 대해 프록시 객체를 일일이 만들어야 하는 문제가 있다. 또한 같은 부가 기능을 적용하는 것이 목적이었다면 만들어진 프록시 객체들의 코드 내용이 같아 중복된 코드가 생긴다는 문제가 발생한다.

#### JDK Dyanmic proxy 적용 후

![](https://velog.velcdn.com/images/evelyn82ny/post/9f01638e-d245-4523-8108-1838af83bc86/image.png)

JDK dynamic proxy를 사용하면 프록시 객체가 동적으로 만들어지기 때문에 프록시를 적용할 객체에 대한 프록시 객체(클래스)를 따로 만들지 않아도 된다.

또한, 같은 부가 기능을 적용하기 위한 로직을 1번만 작성해 공통으로 적용할 수 있어 **단일 책임 원칙(SRP)** 을 지킬 수 있다.
