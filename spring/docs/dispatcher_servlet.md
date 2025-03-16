## dispatcher servlet

- Spring MVC is designed the front controller pattern where a central servlet
- DispatcherServlet uses Spring configuration to discover the delegate components it needs for request mapping, view resolution, exception handling and more
- 요청을 처리히고 적절한 응답으로 랜더링하기 위해 특별한 빈에게 위임하는데 대표적으로 HandlerMapping과 HandlerAdapter가 존재한다. WAC에 존재...?

<br>

## ApplicationContext

- Spring container라고도 함
- Spring Container의 최상위 Interface인 BeanFactory에 부가 기능을 추가
    - 부가 기능: 국제화, 환경 변수 관련 처리, 애플리케이션 이벤트, 리소스 조회 등
- 즉, BeanFactory Interface의 하위 Interface가 ApplicationContext
- ApplicationContext의 구현체는 XML 기반 또는 자바 클래스로 만들 수 있는데 어떤 방법을 사용해도 BeanDefinition으로 추상화해 빈으로 생성
- 빈을 싱글톤으로 관리하기 때문에 싱글톤 컨테이너라고도 함 

## Context Hierarchy

- Servlet WAC와 Root WAC로 분리함으로써 Web 기술과 도메인을 분리할 수 있고 도메인이 웹에 의존하는 상황 같이 의존 방향이 역전되거나 상호 의존하는 상황을 막을 수 있음

## HandlerMapping

- 전후처리를 위해 Interceptor 목록을 가지고 핸들러를 요청한다.
- 2가지의 메인 HandlerMapping 구현
- RequestMappingHandlerMapping: @RequestMapping 지원
- SimpleUrlHandlerMapping: Handler에 대한 URL 경로 패턴의 명시적 등록을 관리
- BeanNameUrlHandlerMapping

## HandlerAdapter

- DispatcherServlet이 요청에 매핑되는 Handler를 찾을 수 있도록 도와준다.
- 예를 들어 Annotated controller를 호출하기 위해선 Annotation을 해결한다.
- 이런 세부사항으로 부터 DispatcherServlet을 보호한다.

## Interceptor

- Controller의 Handler를 호출하기 전과 후에 요청과 응답을 참조하거나 가공할 수 있는 일종의 필터 (추가 작업을 한다.)
    - Controller의 Handler: 요청한 URL에 따라 실행되어야 하는 메서드
- 권한, 로그인 체크같은 추가 작업
- 이를 핸들러마다 추가하면 중복된 코드로 인한 메모리 낭비와 수정 시 해당 핸들러를 모두 수정해야 한다는 문제가 있어 Interceptor를 통해 문제를 해결
- Interceptor 적용 유무의 기준이 되는 URL을 servlet-context.xml에 설정하면 Spring에서 일괄적으로 해당 URL 경로의 핸들러에 Intercept를 적용
<br>

- ```org.springframework.web.servlet.HandlerInterceptor``` Interface를 구현하거나
- ```org.springframework.web.servlet.handler.HandlerInterceptorAdapter``` 추상 클래스를 오버라이딩하여 Interceptor 구현 가능
- HandlerInterceptorAdaptor 추상 클래스는 HandlerInterceptor Interface를 상속받아서 구현되었다.