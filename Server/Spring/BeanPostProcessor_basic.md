# BeanPostProcessor 기본 원리

Spring이 빈 저장소에 등록하기 위해 생성한 객체를 빈 저장소에 등록하기 전에 조작하고 싶다면 BeanPostProcessor를 사용하면 된다.
BeanPostProcessor로 객체를 조작할수도 있고, 다른 객체로 변경할수도 있다.

![](https://velog.velcdn.com/images/evelyn82ny/post/57a9f052-c47d-4cbe-900d-da6c037e25a6/image.png)

> 출처: 스프링 핵심 원리 -고급편(김영한)

1. 생성: Spring Bean 대상이 되는 Bean 객체를 생성
2. 전달: Bean 객체를 빈 저장소에 등록하기 전에 BeanPostProcessor에 전달
3. 작업: 전달된 Bean 객체를 조작 또는 다른 객체로 변경
4. 등록: BeanPostProcessor는 Bean을 반환해 빈 저장소에 등록

BeanPostProcessor에서 Bean이 조작되거나 변경되지 않았다면 Bean 원본 객체가 그대로 등록되고, 그게 아니면 변경된 상태로 빈 저장소에 등록된다. 만약 원본 객체가 변경되었다면 원본 객체는 Spring Bean 으로 아예 등록되지 않는다.

> * BeanPostProcessor test 커밋: https://github.com/evelyn82ny/design-pattern/commit/d4b31c4b6c0d8709eaa7b6a52543c4ae8d5fa725

위 BeanPostProcessor를 A, B객체로 비슷하게 구현해보자.

```java
static class A {
        public void helloA() {
            log.info("class A");
        }
}

static class B {
        public void helloB() {
            log.info("class B");
        }
}
```
A, B 객체는 자신만의 Method를 가지고 있다.

```java
static class BeanPostProcessorConfig {
        @Bean(name = "beanA")
        public A a() {
            A objectA = new A();
            log.info("object A = " + objectA);
            return objectA;
        }

        @Bean
        public AtoBPostProcessor postProcessor() {
            return new AtoBPostProcessor();
        }
}
```
A 객체를 **beanA**라는 이름으로 Bean으로 등록하려고 한다.

```java
static class AtoBPostProcessor implements BeanPostProcessor {
	@Override
	public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
		log.info("beanName = {}, bean = {}", beanName, bean);
		
        if(bean instanceof A) {
			B returnObject = new B();
			log.info("returnObject = " + returnObject);
			return returnObject;
		}
		return bean;
	}
}
```
하지만 A 객체가 생성되면 B 객체를 반환하고, 생성된 객체가 A 객체가 아니면 해당 객체를 그대로 반환한다. 
이처럼 객체를 조작하거나 변경하기 위해 BeanPostProcessor를 사용하며 Spring에 제공하는 BeanPostProcessor Interface를 구현하면 된다.<br>

위 코드를 실행하면 다음과 같은 로그가 출력된다.

<br>

```text
BeanPostProcessorTest$BeanPostProcessorConfig - 
object A = nayoung...BeanPostProcessorTest$A@327120c8

BeanPostProcessorTest$AtoBPostProcessor - 
beanName = beanA, bean = nayoung...BeanPostProcessorTest$A@327120c8

BeanPostProcessorTest$AtoBPostProcessor - 
returnObject = nayoung...BeanPostProcessorTest$B@72efb5c1
```

생성한 A객체와 ```AtoBPostProcessor()```로 넘어간 객체가 서로 동일한 것을 알 수 있다. 
```AtoBPostProcessor()```는 A 객체면 B 객체로 변경해 반환하기 때문에 최종적으로 반환되는 객체는 생성한 A객체와 다른 것을 확인할 수 있다.<br>

즉, 빈 저장소에는 **beanA** 이란 이름을 갖지만 class B인 Bean이 등록되어있다.

```java
void postProcessor() {
	ApplicationContext applicationContext = new AnnotationConfigApplicationContext(BeanPostProcessorConfig.class);

	B b = applicationContext.getBean("beanA", B.class);
	b.helloB();

	Assertions.assertThrows(NoSuchBeanDefinitionException.class, () -> applicationContext.getBean(A.class));
}
```
```text
BeanPostProcessorTest$B - class B
```
**beanA** 이란 이름의 class B인 Bean을 찾아 Method를 실행하면 정상적으로 수행되며 ```AtoBPostProcessor()```로 인해 A 객체는 Bean으로 등록되지 않는다.
<br>

이렇게 생성한 객체를 빈 저장소에 저장하기 전에 완전히 다른 객체로 바꿀 수 있다. 즉, 아래와 같이 프록시 객체로 변경할 수 있다는 의미이다.

![](https://velog.velcdn.com/images/evelyn82ny/post/9df0a539-a9ff-4c17-bc61-74d00067dd94/image.png)

> 출처: 스프링 핵심 원리 -고급편(김영한)

Interface 기반은 JDK Dynamic proxy를 사용해서, Concrete class는 CGLIB를 사용해 실제 객체를 Bean으로 등록하지 않고 적용할 Proxy 객체를 Bean으로 등록했다. 이는 위와 같은 원리이다.

<br>

### BeanPostProcessor Interface

객체를 조작하거나 다른 객체로 변경하기 위해 BeanPostProcessor를 사용하며 이는 Spring에서 Interface로 구현한다.

```java
public interface BeanPostProcessor {
	@Nullable
	default Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
		return bean;
	}
    
    @Nullable
	default Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
		return bean;
	}
}
```
- ```postProcessBeforeInitialization```: Apply this BeanPostProcessor to the given new bean instance before any bean initialization callbacks
- ```postProcessAfterInitialization```: Apply this BeanPostProcessor to the given new bean instance after any bean initialization callbacks

<br>

## 적용

> PackageLogTraceProxyPostProcessor 생성 커밋: https://github.com/evelyn82ny/design-pattern/commit/b3428a9de932d9a02935927a3c8100387aa8f23e

핵심 기능을 처리하며 Log를 찍는 ```xxPrintLog``` mehotd에만 Proxy를 적용하기 위해 PackageLogTraceProxyPostProcessor를 생성했다.

```java
@Override
public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
	log.info("param beanName = {}, bean = {}", beanName, bean.getClass());

	String packageName = bean.getClass().getPackageName();
    if(!packageName.startsWith(basePackage)) {
		return bean;
    }

	ProxyFactory proxyFactory = new ProxyFactory(bean);
	proxyFactory.addAdvisor(advisor);

	Object proxy = proxyFactory.getProxy();
	log.info("create proxy: target = {} proxy = {}", bean.getClass(), proxy.getClass());
	return proxy;
}
```
```baseName```을 ```*PrintLog```로 설정했으며 이를 충족하지 못하면 원본 객체을 반환하고, 충족했다면 Proxy객체를 생성해 반환한다.
<br>
Interface 기반 클래스, Concrete class 그리고 ```@Component```를 사용하는 3가지 경우에 적용했으며 결과는 다음과 같다.

### Interface 기반 클래스

```text
INFO 2609 --- [           main] .d.c.p.PackageLogTraceProxyPostProcessor : 
param beanName = itemRepositoryV1, 
bean = class nayoung.designpattern.app.proxy.basedOnInterface.ItemRepositoryV1Impl

INFO 2609 --- [           main] .d.c.p.PackageLogTraceProxyPostProcessor : 
create proxy: target = class nayoung.designpattern.app.proxy.basedOnInterface.ItemRepositoryV1Impl proxy = class com.sun.proxy.$Proxy52
```
Inteface 기반하는 클래스는 **JDK Dynamic proxy**가 적용된다.

<br>

### Concrete 클래스

```text
INFO 2609 --- [           main] .d.c.p.PackageLogTraceProxyPostProcessor : 
param beanName = orderRepositoryV2, 
bean = class nayoung.designpattern.app.proxy.basedOnConcreteClass.ItemRepositoryV2

INFO 2609 --- [           main] .d.c.p.PackageLogTraceProxyPostProcessor :
create proxy: target = class nayoung.designpattern.app.proxy.basedOnConcreteClass.ItemRepositoryV2 
proxy = class nayoung.designpattern.app.proxy.basedOnConcreteClass.ItemRepositoryV2$$EnhancerBySpringCGLIB$$e18e9f55
```
Inteface가 없는 Concrete 클래스는 **CGLIB**가 적용된다.

<br>

### @Component 사용하는 클래스

```text
INFO 2609 --- [           main] .d.c.p.PackageLogTraceProxyPostProcessor : 
param beanName = itemRepositoryV3, 
bean = class nayoung.designpattern.app.proxy.componentscan.ItemRepositoryV3

INFO 2609 --- [           main] .d.c.p.PackageLogTraceProxyPostProcessor : 
create proxy: target = class nayoung.designpattern.app.proxy.componentscan.ItemRepositoryV3 proxy = class 
nayoung.designpattern.app.proxy.componentscan.ItemRepositoryV3$$EnhancerBySpringCGLIB$$b869ee59
```
```@Component```를 사용하면 Runtime 시 Spring이 ComponentScan하여 Bean으로 등록한다. 그렇기 때문에 PackageLogTraceProxyPostProcessor를 사용하는 BeanPostProcessorConfig에 직접 Bean으로 등록되지 않는 Interface 기반 클래스(V1)와 Concrete 클래스(V2)를 ```@Import```를 사용해 수동으로 Bean 등록한다.

```java
@Import({AppV1Config.class, AppV2Config.class})
public class BeanPostProcessorConfig {..}
```

```@Component``` 사용하는 클래스는 CGLIB가 적용되는데 Spring은 기본적으로 CGLIB를 통해 Proxy를 만든다.