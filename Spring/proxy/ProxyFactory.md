# ProxyFactory

Interface 기반에는 JDK Dynamic proxy를 적용하고 Concrete class에는 CGLIB를 적용해 Proxy를 생성한다.
JDK Dynamic proxy는 ```InvocationHandler```를 구현해야하고, CGLIB는 ```MethodInterceptor```를 구현해야 공통 로직을 적용할 수 있다.<br>

이처럼 유사하지만 조금은 다른 기술을 통합해 일관성있게 접근할 수 있도록 Spring이 **ProxyFactory**라는 기술을 제공한다.
즉, 상황에 맞는 Interface를 구분하며 구현할 필요없이 Spring이 알아서 알맞는 Proxy를 적용한다.<br>

ProxyFactory는 Interface기반이면 JDK Dynamic proxy를, Concrete class이면 CGLIB를 사용해 Proxy를 생성한다. 
그러므로 개발자는 JDK Dynamic proxy의 ```InvocationHandler```나 CGLIB의 ```MethodInterceptor```를 신경쓰지 않고 Proxy가 호출하는 부가기능인 ```Advice```만 작성하면 된다.

<br>

# Advice: MethodInterceptor

```java
package org.aopalliance.intercept;

@FunctionalInterface
public interface MethodInterceptor extends Interceptor {
    @Nullable
    Object invoke(@Nonnull MethodInvocation invocation) throws Throwable;

}
```

MethodInterceptor를 구현하여 Advice를 만들 수 있다. 
```MethodInterceptor``` -> ```Interceptor``` -> ```Advice``` 순서로 Interface를 상속한다.
<br>

매개변수인 ```MethodInvocation invocation```는 ```JoinPoint``` -> ```Invocation``` -> ```MethodInterceptor``` 순으로 Interface를 상속한다. ```MethodInterceptor``` 내부에는 현재 Proxy 객체의 instance, target(핵심 기능을 수행하는) 정보, args, Method 정보 등의 내용이 포함되어있다.
<br>

CGLIB가 제공하는 ```MethodInterceptor```와 이름이 같기 때문에 Package 이름에 주의해야 한다.

- CGLID: package org.Springframework.cglib.proxy
- ProxyFactory: pacage org.aopalliance.intercept
  <br>

> TimeAdvice 및 Interface 기반 Proxy 생성 커밋: https://github.com/evelyn82ny/design-pattern/commit/3d5cb8c0ed2c1c61ab3f9e06d0f3eaa297a5bf8f

핵심 기능을 처리하는데 얼마나 걸리는지 출력하는 기능을 모든 Method에 적용하기 위해 아래와 같이 TimeAdvice를 작성한다.

```java
import org.aopalliance.intercept.MethodInterceptor;
import org.aopalliance.intercept.MethodInvocation;

@Slf4j
public class TimeAdvice implements MethodInterceptor {

    @Override
    public Object invoke(MethodInvocation invocation) throws Throwable {
        log.info("TimeProxy 실행");
        long startTime = System.currentTimeMillis();

        Object result = invocation.proceed();

        long endTime = System.currentTimeMillis();
        long resultTime = endTime - startTime;
        log.info("Time Proxy 종료: result time = {}ms", resultTime);
        return result;
    }
}
```

```invocation.proceed();``` 부분에서 핵심 기능을 처리하는 target을 호출하고 그 결과를 받는다. 
ProxyFactory를 생성하는 단계에서 **Advice**와 **target** 정보를 주입하기 때문에 ```MethodInvocation invocation``` 내부에 target 정보가 있다.

<br>

## Interface 기반 proxy 생성

```java
ServiceInterface target = new ServiceImpl();
ProxyFactory proxyFactory = new ProxyFactory(target);
proxyFactory.addAdvice(new TimeAdvice());

ServiceInterface proxy = (ServiceInterface) proxyFactory.getProxy();
```
Interface 기반인 ServiceImpl 클래스에 대해 Proxy를 적용하고자 한다.

### new ProxyFactory(target);

ProxyFactory 생성시 핵심 기능이 있는 ServiceImpl 클래스(target)를 넘겨주면 target 정보를 기반으로 Proxy를 만들어낸다.
<br>
target이 Interface 기반이라면 JDK Dynamic proxy를, target이 Concrete class라면 CGLIB를 사용해 Proxy 생성 및 반환한다.

<br>

### proxyFatory.addAdvice(new TimeAdvice());

핵심 기능을 처리하는데 얼마나 걸리는지 출력하는 부가 기능인 Advice를 ```addAdvice()```로 설정한다. 
그 후 Proxy는 설정되어있는 Advice를 실행한다.

<br>

### getProxy();

target은 ServiceInterface 기반의 클래스이므로 JDK Dynamic proxy를 통해 만들어진 Proxy가 반환한다. 
반환된 Proxy 정보는 ```class com.sun.proxy.$Proxy11```로 JDK Dynamic proxy를 통해 만들어진 Proxy임을 알 수 있다.

<br>

## Concrete class Proxy 생성

> CGLIB로 proxy 생성: https://github.com/evelyn82ny/design-pattern/commit/d2bc97a947658d5005201964e9d127272c1dd91e

```java
ConcreteService target = new ConcreteService();
ProxyFactory proxyFactory = new ProxyFactory(target);
proxyFactory.addAdvice(new TimeAdvice());

ConcreteService proxy = (ConcreteService) proxyFactory.getProxy();
```
Interface가 없는 Concrete class에 대해 Proxy를 만드는 방법도 동일하다. 
생성된 Proxy 정보는 ``` class nayoung.designpattern.CGLIB.service.ConcreteService$$EnhancerBySpringCGLIB$$7dee5642```로 CGLIB를 통해 Proxy가 생성되었음을 알 수 있다.

<br>

## setProxyTargetClass

> setProxyTargetClass 커밋: https://github.com/evelyn82ny/design-pattern/commit/9fe31696df4e7cc6a6eab3ef1670c5966416279c

```java
ServiceInterface target = new ServiceImpl();
ProxyFactory proxyFactory = new ProxyFactory(target);
proxyFactory.setProxyTargetClass(true);
proxyFactory.addAdvice(new TimeAdvice());

ServiceInterface proxy = (ServiceInterface) proxyFactory.getProxy();
```

Interface 기반은 JDK Dynamic proxy로 만들어지지만 ```setProxyTargetClass(true);``` 옵션을 true로 설정하면 CGLIB를 통해 Proxy를 생성할 수 있다.
<br>
생성된 Proxy 정보는 ```class nayoung.designpattern.JDKDynamicProxy.service.ServiceImpl$$EnhancerBySpringCGLIB$$8bcab63e```로 CGLIB를 통해 Proxy가 생성되었음을 알 수 있다.

<br>

## ProxyFactory 장점

ProxyFactory를 사용하기전에는 클래스에 구조에 따라 구현해야하는 것이 지정되어있었다.

- Interface 기반: ```InvocationHandler``` 구현
- Concrete class: ```MethodInterceptor``` 구현

하지만 ProxyFacotory를 사용하면 어떤 클래스인지 상관없이 pacage org.aopalliance.intercept 의 ```MethodInterceptor``` 을 구현한 Advice만 만들면 된다. 
ProxyFactory 내부에서 JDK Dynamic proxy인 경우 ```InvocationHandler```가 Advice를 호출하도록, CGLIB인 경우 ```MethodInterceptor```가 Advice를 호출하도록 개발되었기 때문이다.

<br>

# Advisor

- Advice: 부가 기능 같은 공통 로직
- Pointcut: Advice를 적용할 범위
- Advisor: 1개의 Pointcut과 1개의 Advice로 구성

> DefaultPointcutAdvisor test 커밋: https://github.com/evelyn82ny/design-pattern/commit/8fe2015cf9653b32fce32893c337405f01d0a8ce

```java
ServiceInterface target = new ServiceImpl();
ProxyFactory proxyFactory = new ProxyFactory(target);
DefaultPointcutAdvisor advisor = new DefaultPointcutAdvisor(Pointcut.TRUE, new TimeAdvice());
proxyFactory.addAdvisor(advisor);

ServiceInterface proxy = (ServiceInterface) proxyFactory.getProxy();
```

### DefaultPointcutAdvisor

Advisor interface의 가장 일반적인 구현체이다. 생성자를 통해 1개의 Pointcut과 1개의 Advice를 주입한다.
<br>
위 코드에서는 ```Pointcut.TRUE``` 를 주입했는데 이는 모든 메소드에 적용됨을 의미한다.

<br>

### proxyfactory.addAdvice()

위에서 Advice를 설명할 때는 proxyfactory 의 ```addAdvice()```만 사용했다. 
그렇다면 Advice를 어디에 적용해야 하는지에 대한 Pointcut을 설정하지 않았다는 뜻인데 에러가 발생하지 않았다.
<br>

```proxyfactory.addAdvice()``` 로 Advice만 설정해도 내부에서 ```Pointcut.TRUE```로 설정하기 때문에 결국 ```DefaultPointcutAdvisor(Pointcut.TRUE, new TimeAdvice())```로 자동 변환횐다.

<br>

# Pointcut

Spring이 제공하는 Pointcut은 다음과 같다.

```java
package org.springframework.aop;

public interface Pointcut {

	/**
	 * Return the ClassFilter for this pointcut.
	 * @return the ClassFilter (never {@code null})
	 */
	ClassFilter getClassFilter();

	/**
	 * Return the MethodMatcher for this pointcut.
	 * @return the MethodMatcher (never {@code null})
	 */
	MethodMatcher getMethodMatcher();


	/**
	 * Canonical Pointcut instance that always matches.
	 */
	Pointcut TRUE = TruePointcut.INSTANCE;

}
```

클래스가 맞는지 확인하는 ```getClassFilter()```와 method가 맞는지 확인하는 ```getMethodMatcher()``` 둘다 True를 반환해야 Advice를 적용한다.

> Pointcut 구현 커밋: https://github.com/evelyn82ny/design-pattern/commit/aaaa99fa1397c4e6ae1d4484ab0f55d3cfd9a29e

```java
static class MyPointcut implements Pointcut {

	@Override
	public ClassFilter getClassFilter() {
		return ClassFilter.TRUE;
	}

	@Override
	public MethodMatcher getMethodMatcher() {
		return new MyMethodMatcher();
	}
}
```
<br>

여러 method 중 **save** method 에만 Advice를 적용하기 위한 MyPointcut를 구현했다.

```java
static class MyMethodMatcher implements MethodMatcher {
	private String matchName = "save";

	@Override
	public boolean matches(Method method, Class<?> targetClass) {
		boolean result = method.getName().equals(matchName);
		log.info("pointcut 호출 method = {} target class = {}", method.getName(), targetClass);
		log.info("pointcut 결과 result = {}", result);
		return result;
	}

	@Override
	public boolean isRuntime() {
		return false;
	}

	@Override
	public boolean matches(Method method, Class<?> targetClass, Object... args) {
		return false;
	}
}
```

해당 method 이름이 **save** 가 아니면 ```matches```에서 False를 반환한다. 
즉, ```getMethodMatcher()```에서 최종적으로 False를 반환하기 때문에 해당 method에는 Advice가 적용되지 않는다.

```text
[Test worker] INFO nayoung...AdvisorTest - pointcut 호출 method = find target class = class nayoung.designpattern.JDKDynamicProxy.service.ServiceImpl
[Test worker] INFO nayoung...AdvisorTest - pointcut 결과 result = false
[Test worker] INFO nayoung...service.ServiceImpl - find 호출

[Test worker] INFO nayoung...AdvisorTest - pointcut 호출 method = save target class = class nayoung.designpattern.JDKDynamicProxy.service.ServiceImpl
[Test worker] INFO nayoung...AdvisorTest - pointcut 결과 result = true
[Test worker] INFO nayoung...TimeAdvice - TimeProxy 실행
[Test worker] INFO nayoung...service.ServiceImpl - save 호출
[Test worker] INFO nayoung...TimeAdvice - Time Proxy 종료: result time = 1ms
```
하지만 위와 같이 Pointcut을 직접 구현할 일은 거의 없다.

<br>

## NameMatchMethodPointcut

> NameMatchMethodPointcut 테스트 커밋: https://github.com/evelyn82ny/design-pattern/commit/e513995723ce51842c13305cd07a46bac75a0e1a

Spring이 제공하는 ```NameMatchMethodPointcut```을 사용해 Method 이름과 매칭해 Advice를 적용할 수 있다.

```java
ServiceInterface target = new ServiceImpl();
ProxyFactory proxyFactory = new ProxyFactory(target);
NameMatchMethodPointcut pointcut = new NameMatchMethodPointcut();
pointcut.setMappedName("save");

DefaultPointcutAdvisor advisor = new DefaultPointcutAdvisor(pointcut, new TimeAdvice());
proxyFactory.addAdvisor(advisor);

ServiceInterface proxy = (ServiceInterface) proxyFactory.getProxy();
```

save method에만 Advice를 적용하려고 한다면 ```NameMatchMethodPointcut```의 ```setMappedName()```으로 save를 설정한다.

```text
[Test worker] INFO nayoung...service.ServiceImpl - find 호출

[Test worker] INFO nayoung...advice.TimeAdvice - TimeProxy 실행
[Test worker] INFO nayoung...service.ServiceImpl - save 호출
[Test worker] INFO nayoung...advice.TimeAdvice - Time Proxy 종료: result time = 1ms
```
find와 save method를 호출하면 find method에는 Advice가 적용되지 않고 save method에만 Advice가 적용됨을 알 수 있다.
<br>

Advice를 적용하기 위한 Pointcut의 방법은 여러가지이다.

- ```NameMatchMethodPointcut```: method 이름 기반으로 매칭하며 내부에서 ```PatternMatchUtils```를 사용
- ```JdkRegexMethodPointcut```: JDK 정규 표현식 기반으로 매칭
- ```TruePointcut```: 항상 True 반환
- ```AnnotationMatchingPointcut```: Annotation 매칭
- ```AspectJExpressionPointcut```: aspectJ 표현식으로 매칭

Spring이 제공하는 여러 Pointcut 중 ```AspectJExpressionPointcut```을 주로 사용한다.

<br>

## N개 Advisor -> N개 Proxy

target을 실행하기 전에 2개의 Advice를 수행하려고 한다.<br>
```client``` -> ```proxy2(Advisor2)``` -> ```proxy1(Advisor1)``` -> ```target``` 구조가 되도록 구현했다.

> MultiAdvisor test 커밋: https://github.com/evelyn82ny/design-pattern/commit/0ac54c4e2ee1eb6a79004e271cd52b807cbb7d51

```java
// proxy1(Advisor1) -> target
ServiceInterface target = new ServiceImpl();
ProxyFactory proxyFactory1 = new ProxyFactory(target);
DefaultPointcutAdvisor advisor1 = new DefaultPointcutAdvisor(Pointcut.TRUE, new Advice1());
proxyFactory1.addAdvisor(advisor1);
ServiceInterface proxy1 = (ServiceInterface) proxyFactory1.getProxy();

// client -> proxy2(Advisor2)
ProxyFactory proxyFactory2 = new ProxyFactory(proxy1);
DefaultPointcutAdvisor advisor2 = new DefaultPointcutAdvisor(Pointcut.TRUE, new Advice2());
proxyFactory2.addAdvisor(advisor2);
ServiceInterface proxy2 = (ServiceInterface) proxyFactory2.getProxy();

proxy2.save();
```

```text
[Test worker] INFO nayoung...MultiAdvisorTest$Advice2 - advice2 호출
[Test worker] INFO nayoung...MultiAdvisorTest$Advice1 - advice1 호출
[Test worker] INFO nayoung...service.ServiceImpl - save 호출
```

2개의 Advice 모두 Pointcut이 ```Pointcut.TRUE``` 이므로 둘다 적용된다.
하지만 2개의 Advice를 적용하기 위해 각 Advice에 대한 Proxy를 생성헀기 때문에 총 2개의 Proxy가 생성되었다.
이 방식은 N개의 Advice를 적용려면 N개의 Proxy를 만들어야 한다.
<br>

Spring은 이 문제를 해결해 1개의 Proxy로 여러 Advisor를 적용할 수 있는 기능을 제공한다.

<br>

## N개 Advisor -> 1개 Proxy

> 여러 Advisor를 1개의 Proxy가 처리하는 test 커밋: https://github.com/evelyn82ny/design-pattern/commit/dda93dee011e04ff1cdd4f94110a79b7ba8612e6

```java
DefaultPointcutAdvisor advisor2 = new DefaultPointcutAdvisor(Pointcut.TRUE, new Advice2());
DefaultPointcutAdvisor advisor1 = new DefaultPointcutAdvisor(Pointcut.TRUE, new Advice1());

ServiceInterface target = new ServiceImpl();
ProxyFactory proxyFactory = new ProxyFactory(target);
proxyFactory.addAdvisor(advisor2);
proxyFactory.addAdvisor(advisor1);

ServiceInterface proxy = (ServiceInterface) proxyFactory.getProxy();
```
여러 Advisor를 ProxyFactory에 추가하고 1개의 Proxy만 생성한다.

```text
[Test worker] INFO nayoung...MultiAdvisorTest$Advice2 - advice2 호출
[Test worker] INFO nayoung...MultiAdvisorTest$Advice1 - advice1 호출
[Test worker] INFO nayoung...service.ServiceImpl - find 호출
```
추가한 순서대로 Advice가 실행된다.
<br>

Spring AOP로 1개의 target에 여러 Adivce를 적용해도 **1개의 Proxy만 만들어지는 것이 핵심**이다.