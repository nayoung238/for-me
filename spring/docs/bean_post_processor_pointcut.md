# BeanPostProcessor - pointcut

Spring Boot는 ```AnnotationAwareAspectJAutoProxyCreator```라는 BeanPostProcessor가 Spring bean에 자동 등록된다.

- 자동으로 Proxy 생성
- Spring bean으로 등록된 Advisor들을 자동으로 찾아 Proxy가 필요한 곳에 적용
- ```@AspectJ```와 관련된 AOP 기능도 자동으로 처리

![png](/spring/img/bean_post_processor_3.png)

> 출처: 스프링 핵심 원리 -고급편(김영한)


1. 생성: Spring Bean 대상이 되는 객체를 생성한다. (```@Bean```, Component scan 등)
2. 전달: Bean을 빈 저장소에 저장하기 전에 BeanPostProcessor로 전달
3. 모든 Advisor bean 조회: Spring container에서 모든 Advisor 조회
4. 프록시 적용 대상 체크: 3단계에서 조회한 Advisor의 Pointcut으로 해당 객체가 프록시 적용대상인지 아닌지를 판단.
    * 객체의 클래스 정보와 해당 객체의 모든 Method를 Pointcut에 매칭
    * 그 중 1개라도 매칭되면 Proxy 적용 대상
5. 프록시 생성: 4단계에서 1개라도 매칭되면 Proxy를 생성(이때는 원본 객체가 Bean으로 등록되지 않음)
    * 만약 Proxy 적용대상이 아니라면 원본 객체가 Spring Bean으로 등록
6. Bean 등록: Proxy 또는 원본 객체를 Spring bean으로 등록

<br>

> BeanPostProcessor interface를 구현 커밋: https://github.com/imzero238/design-pattern/commit/b3428a9de932d9a02935927a3c8100387aa8f23e

```java
public class PackageLogTraceProxyPostProcessor implements BeanPostProcessor {...}
```
이전에는 Spring이 제공하는 BeanPostProcessor interface를 구현했지만 Spring Boot는 ```AnnotationAwareAspectJAutoProxyCreator```이 알아서 다 해주기 때문에 더 이상 직접 구현할 필요가 없다.

<br>

## Pointcut

> Proxy 자동 적용 커밋: https://github.com/imzero238/design-pattern/commit/567198f919eb3818c0c164da7fc2e13635678ff7

```java
@Bean
public Advisor advisor(LogTrace logTrace) {
	NameMatchMethodPointcut pointcut = new NameMatchMethodPointcut();
    pointcut.setMappedName("*PrintLog");
    LogTraceAdvice advice = new LogTraceAdvice(logTrace);
    return new DefaultPointcutAdvisor(pointcut, advice);
}
```
위와 같이 1개의 Pointcut과 1개의 Advice를 갖는 Advisor만 생성한다.
```AnnotationAwareAspectJAutoProxyCreater```이 해당 Pointcut을 보고 알아서 Proxy를 적용한다.
<br>

Pointcut은 2번 사용된다.

- 해당 객체가 Proxy 적용 대상인지 판단 (생성 단계)
- Advice 적용 여부 (사용 단계)

<br>

### 해당 객체가 Proxy 적용 대상인지 판단 (생성 단계)

해당 객체를 빈 저장소에 저장하기 전 BeanPostProcessor에 넘겨 Proxy 적용 대상 여부를 판단한다.
클래스와 모든 Method에 Spring container에 있는 모든 Advisor를 매칭한다.
<br>

### Advice 적용 여부 (사용 단계)

해당 객체 중 특정 Method가 Advisor를 매칭되어 Proxy 적용 대상이 되었다. 
이는 특정 Method가 아닌 다른 Method를 사용할 때는 Advice를 적용하면 안 된다는 뜻이다.

<br>

## execution 사용

Advice를 적용하기 위해 Proxy 객체를 생성하는 것은 결국 자원을 사용한다는 의미이다. 
즉, Pointcut으로 적용 범위를 조율하는 것이 중요한다.

> execution 사용 커밋: https://github.com/imzero238/design-pattern/commit/fa6dc11c3018009eab646f7ad38853448f2e4528

Pointcut을 설정할때 ```setExpression(execution)``` 으로 자세한 범위 설정을 할 수 있다.

```java
@Bean
public Advisor setExecutionAdvisor(LogTrace logTrace) {
	AspectJExpressionPointcut pointcut = new AspectJExpressionPointcut();
    pointcut.setExpression("execution(* nayoung.designpattern.app..*(..))");
    LogTraceAdvice advice = new LogTraceAdvice(logTrace);
    return new DefaultPointcutAdvisor(pointcut, advice);
}
```
위와 같이 Pointcut을 설정하면 app package에 포함되는 모든 클래스와 method에 Advice를 적용한다.
<br>
주문을 처리하는 기능에서 Log 출력 유무에 따라 ```orderAndPrintLog``` 와 ```orderAndNoLog``` method로 분리했다. 
위와 같이 Pointcut을 적용하면 결국 Log를 출력하지 않는 ```orderAndNoLog``` method에도 Advice가 적용되는 문제가 발생한다.

<br>

```java
@Bean
public Advisor setExecutionAdvisor2(LogTrace logTrace) {
	AspectJExpressionPointcut pointcut = new AspectJExpressionPointcut();
    pointcut.setExpression(
        "execution(* nayoung.designpattern.app..*(..)) 
    	&& !execution(* nayoung.designpattern.app..*NoLog(..))");
    LogTraceAdvice advice = new LogTraceAdvice(logTrace);
    return new DefaultPointcutAdvisor(pointcut, advice);
}
```
Log를 출력하지 않는 ```orderAndNoLog``` method에 Advice가 적용되는 것을 막기 위해 ```execution(* nayoung.designpattern.app..*NoLog(..))``` 를 추가한다.