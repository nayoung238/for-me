## InternalResourceViewResolver

```java
import org.springframework.web.servlet.mvc.Controller;

@Component("/springmvc/controller")
public class controller implements Controller {
    
    @Override
    public ModelAndView handleRequest(HttpServletRequset request, HttpServletResponse response) throws Excpection {
        return new ModelAndView("new-form");
    }
}
```
Handler 가 View 의 논리이름만 작성해도 정상적으로 작동하는 이유는 ViewResolver 가 물리 주소로 변경시키기 때문이다.<br>
spring boot 는 **InternalResourceViewResolver** 라는 view resolver 를 자동으로 등록한다.<br>

<br>

## prefix, suffix 설정

```properties
spring.mvc.view.prefix=/WEB-INF/views/   <- view 경로
spring.mvc.view.suffix=.jsp              <- view 파일종류
```
application.properties 파일에 아래와 같이 view 논리이름을 제외한 경로와 파일 종류를 설정한다. 
전체 경로를 작성하는 것보다 prefix, suffix 를 설정하는 것을 권장한다.

```java
public class UrlBasedViewResolver extends AbstractCachingViewResolver implements Ordered {
    
    public static final String REDIRECT_URL_PREFIX = "redirect:";
    public static final String FORWARD_URL_PREFIX = "forward:";
    
    @Nullable
    private Class<?> viewClass;
    
    private String prefix = "";
    private String suffix = "";
}
```
```java
public class InternalResourceViewResolver extends UrlBasedViewResolver {
    private static final boolean jstlPresent = ClassUtils.isPresent(
            "javax.servlet.jsp.jstl.core.Config", InternalResourceViewResolver.class.getClassLoader());
    
    @Nullable
    private Boolean alwaysInclude;
    
    public InternalResourceViewResolver(String prefix, String suffix) {
        this();
        setPrefix(prefix);
        setSuffix(suffix);
    }
}
```
자동 등록된 **InternalResourceViewResolver** 라는 view resolver 는 handler 가 view 의 논리 이름만 반환해도 설정된 prefix, suffix 로 물리 주소로 변경시킨다.<br>
InternalResourceViewResolver 코드의 일부를 보면 **UrlBasedViewResolver**를 상속하고 있고, **prefix 와 suffix**로 물리 주소로 변환한다.

```java
@Bean
ViewResolver internalResourceViewResolver(){
    return new InteralResourceViewResolver("/WEB-INF/views/", ".jsp");
}
```
위 코드와 같은 형태로 직접 작성해도 되지만 springboot 가 **InternalResourceViewResolver** 를 등록한 뒤 알아서 해주기 때문에 작성할 필요가 없다.

<br>

## forward()

spring boot 가 자동 등록하는 view resolver 중 일부는 다음과 같다.

- BeanNameViewResolver : Bean 이름으로 view 를 찾아서 반환한다.
- InternalResourceViewResolver : JSP 로 처리할 수 있는 view 를 반환한다.

InternalResourceViewResolver 는 JSP 처럼 forward()를 호출해서 처리가능할때만 사용한다.

```java
@Override
protected AbstractUrlBasedView buildView(String viewName) throws Exception {
    InternalResourceView view = (InternalResourceView) super.buildView(viewName);
    
    if(this.alwaysInclude != null) {
        view.setAlwaysInclude(this.alwaysInclude);
    }
    view.setPreventDispatchLoop(true);
    return view;
}
```

InternalResourceViewResolver 코드 일부를 보면 최종적으로 InternalResourceView 를 반환한다. 
View 도 Interface 로 구현되었기 때문에 forward() 가 필요하다.
즉, view.render()가 호출되면 InternalResourceView 는 forward()를 통해 JSP 를 실행한다.<br>

JSP 를 제외한 다른 View template 은 forward() 호출없이 바로 렌더링된다.
