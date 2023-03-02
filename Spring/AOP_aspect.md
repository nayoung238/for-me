## Spring AOP: @Aspect

개발을 하다가 불필요한 쿼리를 줄이기 위해 콘솔창에 뜨는 로그를 확인했다. 하지만 로그가 너무 많아 어떤 과정의 어떤 메서드에서 발생했는지 제대로 파악하기 어려워 불편을 겪었고 이를 해결하기 위해 **요청 처리과정에서 사용되는 메서드명을 출력**하는 기능을 만들기로 결정했다. 

어떤 메서드명인지 출력하는 기능은 부가 기능이고 요청을 처리하는 기능은 핵심 기능이다. 처음에는 핵심 기능이 있는 메서드에 부가 기능을 추가했지만 다음과 같은 문제가 발생했다.

- 핵심 기능을 처리하는 메서드에 부가 기능 코드가 더 길어서 가독성이 떨어짐
- 여러 메서드에 똑같은 부가 기능 코드가 중복되므로 유지 보수가 어려움

위 문제를 해결하기 위해 이것저것 알아보니 **디자인 패턴, Spring AOP** 를 알게되었고 이 2가지를 공부한 결과, 현재 부가 기능을 추가하고 싶은 경우에 원하는 객체에 프록시 패턴을 적용해 **핵심 기능과 부가 기능을 완전히 나누게 되었다**. 앞으로도 많이 쓸 기능이라 생각하여 해당 게시물에 기록하며, 실제 프로젝트에 적용한 커밋은 아래 링크를 통해 확인할 수 있다.


# Proxy

클라이언트는 대리자를 통해 서버에게 간접적으로 요청할 수 있는데 이 대리자를 **Proxy** 라고 한다. Proxy 객체로 접근제어, 캐싱, 부가 기능 추가 등을 구현할 수 있다. 

Proxy pattern과 Decorator pattern은 Proxy를 사용하지만 사용 의도가 다르다.

- Proxy pattern: Proxy로 **접근 제어**하는 것이 목적
- Decorator pattern: Proxy로 **새로운 기능을 추가**하는 것이 목적

즉, 두 패턴은 프록시를 사용하지만 의도가 다르다. 두 패턴에 대한 자세한 내용은 아래 링크에서 자세히 파악할 수 있다.

> 자세한 내용: https://velog.io/@evelyn82ny/proxy-pattern-decorator-pattern

<br>

## JDK 동적 프록시와 CGLIB 프록시

- JDK dynamic 프록시: interface 기반에 적용
- CGLIB 프록시: concreter 클래스에 적용

```java
public interface UserRepository extends JpaRepository<User, Long> {}
```
```text
class com.sun.proxy.$Proxy127
```
UserRepository 는 interface 이므로 JDK dynamic 프록시가 적용된다.
<br>

```java
@Service
public class UserService {}
```
```text
class com.gokimpark.instaclone.domain.user.UserService$$EnhancerBySpringCGLIB$$f1bed78
```
UserService 는 Concrete class 이므로 CGLIB 프록시가 적용된다.

> 자세한 내용: https://velog.io/@evelyn82ny/JDK-Dynamic-Proxy-CGLIB

<br>

# AOP 용어

AOP 에 ```Join Point```, ```Pointcut```, ```Advice```, ```Target```, ```Aspect```, ```Advisor``` 등 알아야 하는 용어가 있다.

## Pointcut & Advice

**요청 처리과정에서 사용되는 메서드명을 출력**하는 기능을 부가 기능이라하며 이를 Advice 라고한다. 부가 기능(Advice)을 어디에 적용할 지 선택하고 판별하는 기능을 Pointcut이라고 한다.

내가 만든 부가 기능이 모든 객체에 언젠간 필요하겠지라는 마음으로 모든 객체에 적용하면 적용되는 객체만큼 프록시 객체가 만들어진다. 즉, 굳이 필요없는 객체에 대해서 프록시를 적용하면 자원이 낭비되는 문제가 발생한다. 이를 막기 위해 Pointcut 이 필요하다.
<br>

## Join point

부가 기능(Advice)이 적용될 수 있는 지점을 Join point 라고 한다.

Spring AOP는 스프링 컨테이너가 관리하는 Spring Bean에만 AOP를 적용할 수 있다. Method, Constructor, field 값 또는 static method 등 적용될 수 있는 많은 지점이 존재하는데 Spring AOP의 Join point는 **Method**으로 제한된다.
<br>

## Target

Advice가 적용된 객체를 Target이라고 한다.

Spring Bean 대상이 되는 객체를 생성한 후 Spring Bean으로 등록하기 전에 프록시 적용 여부를 판별하기 위해 Bean PostProcessor에 전달한다. 

스프링 컨테이너에서 모든 ```Advisor```를 해당 객체의 모든 method에 매칭해본다. 매칭 결과 하나라도 Advice 적용 대상이 되면 해당 객체에 대한 프록시 객체가 생성되며 이 프록시 객체가 스프링 빈으로 등록된다. 만약 매칭되는 method가 없는 경우 해당 객체는 원본 자체가 스프링 빈으로 등록된다.
<br>

## Aspect

부가 기능(Advice)과 부가 기능을 어디에 적용할지 선택하는 기능인 Pointcut으로 구성된 Advisor를 생성하는 기능을 Aspect라고 한다. 

```java
@Aspect @Component
public class TraceAspect {
	
    @Around("적용 범위 지정") // Pointcut
    public void advice(ProceedingJoinPoint joinPoint) {} // Advice
}
```
Pointcut과 Advice를 정의한 클래스에 ```@Aspect```를 추가한다. 작성한 Aspect 를 AOP 로 사용하기 위해 Spring Bean 으로 직접 등록해야 한다. ```@Aspect``` annotation 은 component scan 의 대상이 아니기 때문이다.

```@Bean```, ```@Component```, ```@Import``` 을 사용해 Spring Bean 으로 등록한다.

<br>

# 📍프로젝트에 적용하기

![](/_img/Spring/aop_aspect_result.png)

- AOP 적용 커밋: https://github.com/evelyn82ny/instagram-api/commit/ba14a69be6be85de3273b1229255aa3a93ca001f

기존 프로젝트 부가 기능을 추가하면 위와 같이 어떤 메소드를 실행했을 때 쿼리가 발생했는지 알 수 있다.

# Advice: 부가 기능 작성하기

```java
@Aspect
@Component
public class TraceAspect {

    @Around("execution(* com.gokimpark.instaclone.domain..*(..))")
    public Object doTrace(ProceedingJoinPoint joinPoint) throws Throwable {
        try {
            log.info("[Trace begin] {}", joinPoint.getSignature());
            Object result = joinPoint.proceed();
            log.info("[Trace end] {}", joinPoint.getSignature());
            return result;
        } catch (Exception e) {
            log.info("[Trace exception] {} -> {}", joinPoint.getSignature(), e.getMessage());
            throw e;
        }
    }
}
```
Aspect에 Advice(부가기능)와 Pointcut(부가기능을 적용하는 범위)을 작성한다. 위 코드를 보면 ```@Around``` annotation 부분이 Pointcut이고 아래 method가 Advice이다.

Pointcut으로 ```@Around```를 사용한다면 Advice를 작성한 method의 parameter에 ```ProceedingJoinPoint```를 추가한다. ```ProceedingJoinPoint```  내부에 호출된 객체와 메서드, 전달 인자 등 여러 정보가 포함되어 있다. 

또한 ```@Around```를 사용하면 **핵심 기능을 수행하는 target 을 직접 호출**해야 한다. Advice를 작성한 method의 ```joinPoint.proceed();``` 코드가 target을 호출한다. 즉, ```proceed()```를 작성하지 않으면 핵심 기능은 처리되지 않는 문제가 발생한다.

만들고자하는 부가 기능은 핵심 기능을 처리하기 전과 후에 method 이름을 로그로 출력하는 것이다. 그렇기 때문에 핵심 기능을 호출하는 부분 전후에 ```[Trace begin]```과 ```[Trace end]```를 작성하고 method 이름을 출력하기 위한 ```getSignature()```을 작성한다.

위와 같이 작성했다면 target에서 예외가 발생하지 않으면 ```[Trace begin]```과 ```[Trace end]``` 로그가 출력된다. 만약 target 에서 예외가 발생했다면 ```[Trace begin]```과 ```[Trace exception]``` 로그가 출력된다.

return 값이 존재하기 때문에 이를 반환하거나 **조작**할 수 있다.
<br>

# Pointcut

Advice 적용 범위를 지정하는 기능인 Pointcut 을 작성하는 방법은 여러가지가 존재한다.

![](/_img/Spring/aop_aspect_file_structure.png)

## @Around

```@Around``` 괄호 안에는 해당 Advice를 적용하고 싶은 범위를 작성한다. Advice를 Service와 Repository에 적용하려고 한다. 진행중인 프로젝트에서 Service와 Repository는 domain package의 하위 패키지에 존재하기 때문에 com package 부터 domain package 까지 작성하면 된다.

```java
@Around("execution(* com.gokimpark.instaclon.domain..*(..))")
```

위와 같이 작성하면 domain package내에 있는 모든 클래스에 프록시가 적용된다. 그렇기 때문에 web package에 있는 Controller에는 프록시가 적용되지 않는다. (domain package 내에 있는 **Exception은 프록시가 적용되지 않음**)
<br>

## within

Advice 를 적용할 범위를 더 좁게 잡고 싶다면 ```within``` 을 사용하면 된다.

```java
 @Pointcut("within(com.gokimpark.instaclone.domain..*Service)")
```

위와 같이 작성할 경우 domain 패키지내의 모든 클래스 중 클래스 이름에 Service을 포함하고 있는 클래스에만 Advice가 적용된다.

```java
@Pointcut("within(com.gokimpark.instaclone.web..*Controller)")
```
위와 같이 작성할 경우 web 패키지내의 모든 클래스 중 클래스 이름에 Controller을 포함하고 있는 클래스에만 Advice가 적용된다.

***

```@Around```를 사용한다면 **다음 Advice나 핵심 기능을 처리하는 target을 호출하기 위해** ```joinPoint.proceed();``` **를 꼭 작성해야한다**. 실수로 작성하지 않았다면 핵심 기능은 처리되지 않는 문제가 발생한다.

아래에서 설명할 여러 방식은 다음과 같은 특징이 있다.

- ```ProceedingJoinPoint```가 아닌 ```Joinpoint```를 사용
- target을 호출하는 ```joinPoint.proceed()```를 작성하지 않아도됨

> * @Before, @AfterReturning test 커밋: https://github.com/evelyn82ny/instagram-api/commit/5725dab9241e617ccf3e2c383dd1769f17f1331a

## @Before

```java
@Before("com.gokimpark.instaclone.aspect.Pointcuts.serviceAndRepoLayer()")
public void doBefore(JoinPoint joinPoint) {
	log.info("**[Trace begin(@Before)] {}", joinPoint.getSignature());
}
```

* ```@Before```: JoinPoint 실행 이전에 실행
* ```@Around```를 사용해 작성한 Advice의 ```log.info("[Trace begin] {}", joinPoint.getSignature());```  부분을 위 코드로 대체가능
<br>


## @AfterReturning

```java
@AfterReturning(value = "com.gokimpark.instaclone.aspect.Pointcuts.serviceAndRepoLayer()", returning = "result")
public void doReturn(JoinPoint joinPoint, Object result) {
	log.info("**[Trace end(@AfterReturning)] {}", joinPoint.getSignature());
}
```
* ```@AfterReturning```: JoinPoint 정상 완료 후 실행
* ```@Around```를 사용해 작성한 Advice의 ```log.info("[Trace end] {}", joinPoint.getSignature());```  부분을 위 코드로 대체가능
<br>

## @AfterThrowing

```java
@AfterThrowing(value = "com.gokimpark.instaclone.aspect.Pointcuts.serviceAndRepoLayer()", throwing = "ex")
public void doThrowing(JoinPoint joinPoint, Exception ex) {
	log.info("**[exception(@AfterThrowing)] {} message = {}", joinPoint.getSignature(), ex.getMessage());
}
```
* ```@AfterThrowing```: JoinPoint에서 예외가 발생한 경우 실행
* ```@Around```를 사용해 작성한 Advice의 ```log.info("[Trace exception] {} -> {}", joinPoint.getSignature(), e.getMessage());```  부분을 위 코드로 대체가능
<br>

## @After

```java
@After(value = "com.gokimpark.instaclone.aspect.Pointcuts.serviceAndRepoLayer()")
public void doAfter(JoinPoint joinPoint) {
	log.info("**[Trace end(@After)] {}", joinPoint.getSignature());
}
```
- ```@After```: JoinPoint가 정상 또는 예외 발생에 관계없이 실행(finally)
<br>

### 적용 결과

Test 용으로 만든 ```@Before```, ```@AfterReturning``` 등이 원하는 지점에서 실행되는지 파악하기 위해 부가 기능으로 만든 **TraceAspect**와 함께 test를 진행했다.

부가 기능으로 만든 **TraceAspect**은 ```[Trace ...]``` 형식으로 출력되고, Test 용은 ```**[Trace ...(@...)]``` 형식으로 출력된다.

```text
[    Test worker] com.gokimpark.instaclone.aspect.Advice   : 
**[Trace begin(@Before)] UserDto com.gokimpark.instaclone.domain.user.UserService.createAccount(JoinDto)

[    Test worker] c.g.instaclone.aspect.TraceAspect        :
[Trace begin] UserDto com.gokimpark.instaclone.domain.user.UserService.createAccount(JoinDto)

[    Test worker] com.gokimpark.instaclone.aspect.Advice   : 
**[Trace begin(@Before)] Object org.springframework.data.repository.CrudRepository.save(Object)

[    Test worker] c.g.instaclone.aspect.TraceAspect        :
[Trace begin] Object org.springframework.data.repository.CrudRepository.save(Object)
```
사용자 가입이 정상적으로 수행되는 Log 중 일부이다. ```@Before```가 JoinPoint 실행 이전에 제대로 실행되는 것을 알 수 있다. 다음은 예외 상황을 발생시켰을 때 ```@AfterThrowing```이 제대로 실행되는지 test 했다.
<br>

```text
[    Test worker] o.h.engine.jdbc.spi.SqlExceptionHelper   :
Duplicate entry 'apple' for key 'users.UK_r43af9ap4edm43mmtq01oddj6'

[    Test worker] c.g.instaclone.aspect.TraceAspect        :
[Trace exception] Object org.springframework.data.repository.CrudRepository.save(Object) 
-> could not execute statement; SQL [n/a]; constraint [users.UK_r43af9ap4edm43mmtq01oddj6]; 
nested exception is org.hibernate.exception.ConstraintViolationException: could not execute statement

[    Test worker] com.gokimpark.instaclone.aspect.Advice   : 
**[exception(@AfterThrowing)] Object org.springframework.data.repository.CrudRepository.save(Object)
message = could not execute statement; SQL [n/a]; constraint [users.UK_r43af9ap4edm43mmtq01oddj6]; 
nested exception is org.hibernate.exception.ConstraintViolationException: could not execute statement
```
해당 Application은 사용자의 Username field 값이 중복되면 안된다. 만약 같은 Username으로 가입을 시도하면 **AccountException**이 발생하도록 구현했다.

위 경우는 이미 존재하는 **apple** username으로 가입을 시도했을 때 출력된 로그이며 Test 용으로 만든 ```@AfterThrowing```이 제대로 실행됨을 알 수 있다.
<br>


# @Target: annotation 만들기

```java
@Around("execution(* com.gokimpark.instaclone.domain..*(..))")
```
만약 위와 같이 작성했다면 Domain 패키지에 있는 모든 객체에 프록시가 적용된다. 그렇기 때문에 프록시가 적용된 모든 객체에 대한 로그가 출력된다. 하지만 특정 클래스에 대해서만 Advice을 적용하고 싶다면 annotation을 생성하면 된다.

```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface Trace {}
```

- ```@Target(ElementType.TYPE)``` : Class, interface (including annotation type) 또는 enum에 annotation 추가가능
- ```@Target(ElementType.METHOD)``` : Method에 annotation 추가가능

<br>

```java
public class TraceAspect {

    @Around("execution(* com..경로생략..domain..*Service(..)) && @target(trace)")
    public Object doTrace(ProceedingJoinPoint joinPoint, Trace trace) throws Throwable {
    	// 부가 기능 코드 생략 
    }
}
```

위와 같이 ```@Around```를  작성하면 **domain 패키지내의 Service 객체에 대해서만 프록시 객체가 생성되고, 이 프록시 객체 중 @Trace annotation이 있는 객체**만 부가 기능을 수행한다.

<br>

```java
@Trace @Controller
public class UserController{}
```
```java
@Controller
public class ProfileController{}
```
만약 위와 같이 UserController에만 @Trace 를 추가했다면 두 Controller에 프록시가 적용은 되지만 UserController만 Advice를 수행한다.
<br>

 @target 을 작성하는 방법은 2가지가 있다.

```java
@target(annotation 을 작성한 경로)
```
```java
@Annotation(@target(a))
public Object doTrace(ProceedingJoinPoint joinPoint, A a) throws Throwable {}
````
annotation이 있는 경로를 적거나 **파라미터에 추가해 파라미터명과 동일하게 작성**하면 된다.