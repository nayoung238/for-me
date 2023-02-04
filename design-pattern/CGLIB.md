# CGLIB

JDK Dynamic proxy는 Interface 기반에만 적용할 수 있다. 그렇기 때문에 interface가 없는 Concrete class에 proxy를 적용하려면 CGLIB를 사용한다.

CGLIB(Code Generator Library)는 바이트코드를 조작해서 동적으로 클래스를 생성하는 기술을 제공하는 라이브러리이다. Spring framework가 CGLIB를 Spring 내부 소스 코드에 포함했기 때문에 별도의 외부 라이브러리를 추가하지 않아도 바로 사용가능하다.

## MethodInterceptor

> CGLIB test 커밋: https://github.com/evelyn82ny/Design-pattern/commit/dbc5a27970a4bfb3950ca8c813e7dac1d5df6353

JDK Dynamic proxy에 실행 로직을 적용하기 위해 ```InvocationHandler```를 구현한다. 같은 원리로 CGLIB도 ```MethodInterceptor```를 구현한다.

```java
public interface MethodInterceptor extends org.springframework.cglib.proxy.Callback {
    java.lang.Object intercept(java.lang.Object o, java.lang.reflect.Method method, 
    							java.lang.Object[] objects, 
                                org.springframework.cglib.proxy.MethodProxy methodProxy) throws java.lang.Throwable;
}
```
- ```Objec object```: CGLIB가 적용된 객체
- ```Method method```: 호출된 Method
- ```Object[] objects```: Method를 호출하면서 전달된 인수
- ```MethodProxy methodProxy```: Method 호출에 사용
<br>

공통 로직으로 설정할 TimeMethodInterceptor를 아래와 같이 구현한다.

```java
public class TimeMethodInterceptor implements MethodInterceptor {

    private final Object target;

    public TimeMethodInterceptor(Object target) {
        this.target = target;
    }

    @Override
    public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
        log.info("TimeProxy 실행");
        long startTime = System.currentTimeMillis();

        Object result = methodProxy.invoke(target, objects);

        long endTime = System.currentTimeMillis();
        long resultTime = endTime - startTime;
        log.info("TimeProxy 종료 resultTime = {}", resultTime);
        return result;
    }
}
```
Concrete class에 proxy를 적용하기 위해 CGLIB를 사용하기 때문에 ```InvocationHandler```가 아닌 ```MethodInterceptor```을 구현한다.

target을 호출하기 위해 ```invoke()```를 호출해야 하는데 CGLIB는 Method가 아닌 **MethodProxy**의 ```invoke()```를 사용하는 것을 권장한다.
<br>

## CGLIB로 Proxy 생성

```java
public class CglibTest {

    @Test
    void CGLIB() {
        ConcreteService target = new ConcreteService();

        Enhancer enhancer = new Enhancer();
        enhancer.setSuperclass(ConcreteService.class);
        enhancer.setCallback(new TimeMethodInterceptor(target));

        ConcreteService proxy = (ConcreteService) enhancer.create();
        log.info("target class = {}", target.getClass());
        log.info("proxy class = {}", proxy.getClass());

        proxy.call();
    }
}
```

CGLIB는 ```Enhancer```를 사용해 Proxy를 생성한다.

- ```setSuperclass()```: 어떤 Concrete class를 상속받아 Proxy를 생성할지 지정
- ```setCallback()```: Proxy에 적용할 실행 로직을 할당
- ```create()```: ```setSuperclass()```에 지정한 클래스를 상속받아 Proxy 생성

### 실행 결과

```text
INFO nayoung.designpattern.CGLIB.CglibTest - target class = class nayoung.designpattern.CGLIB.service.ConcreteService
INFO nayoung.designpattern.CGLIB.CglibTest - proxy class = class nayoung.designpattern.CGLIB.service.ConcreteService$$EnhancerByCGLIB$$126d2f82
INFO nayoung.designpattern.CGLIB.TimeMethodInterceptor - TimeProxy 실행
INFO nayoung.designpattern.CGLIB.service.ConcreteService - ConcreteService 호출
INFO nayoung.designpattern.CGLIB.TimeMethodInterceptor - TimeProxy 종료 resultTime = 36
```

CGLIB로 생성한 Proxy 정보는```ConcreteService$$EnhancerByCGLIB$$126d2f82```로 ```Concreate class 이름 $$ EnhancerByCGLIB $$ 임의의코드``` 규칙으로 생성된다. 참고로 JDK dynamic proxy로 생성한 Proxy 정보는 ```class com.sun.proxy.$Proxy11```로 생성된다.
<br>

## CGLIB 정리

Interface가 없는 Concrete class에 Proxy를 적용하려면 CGLIB를 사용한다. CGLIB는 상속으로 Proxy를 생성하기 때문에 다음과 같은 제약이 있다.

- 부모 클래스의 생성자 확인: 자식 클래스를 동적으로 생성하기 때문에 기본 생성자가 필요
- Method에 **final**가 붙으면 해당 Method를 Overriding을 할 수 없어 프록시 로직이 동작하지 않음

그리고 CGLIB를 직접 사용하는 경우는 거의 없고 Spring AOP의 ProxyFactory를 사용한다.