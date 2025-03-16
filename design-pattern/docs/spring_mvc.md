# Spring MVC

![png](/design-pattern/img/spring_mvc_architecture.png)

> 출처: https://gmlwjd9405.github.io/2018/12/20/spring-mvc-framework.html
<br>

![png](/design-pattern/img/DispatcherServlet2.png)

> 출처: https://mangkyu.tistory.com/18

빨간색 부분인 RestController부터 Service, Repository 까지는 개발자가 개발한다.
*HTTP 요청이 들어오면 적절한 Controller를 찾고 요청한 데이터를 파싱하는 일은 파란색 부분에서 처리되는데 이는 SpringBoot에서 알아서 처리해주기 떄문에 막상 이론을 설명하라고 하면 제대로 설명할 수 없어 이를 해결하고자 해당 게시물을 작성해본다*.

<br>

일단 Spring MVC 패턴은 Model, View 그리고 Controller로 분리한 디자인 패턴이다. Model로 데이터를 나타내며 일반적으로 **POJO(객체지향적인 원리에 충실하면서 필요에 따라 재활용될 수 있는 방식으로 설계된 오브젝트)**로 구성하며 Bean으로 생성된다. View는 디스플레이 데이터로 HTML로 생성한다. Controller는 View와 Model 사이의 Interface 역할을 하며 요청에 대한 결과를 Model에 담아 View에게 전달한다.
<br>

간단한 서비스라면 Controller에 핵심 기능과 View를 같은 코드에 작성해도 된다. 하지만 개발에서 중요하게 말하는 유지보수가 좋은 코드와는 거리가 멀다. Front를 고치는 과정에서 서버 코드에도 영향을 미칠 수 있기 때문이다. 그래서 Controller와 View를 분리한다. 그러면 Controller에서 처리한 내용을 사용자에게 보여주기 위해 View를 출력해야 하는데 View에 사용되는 데이터를 Model 객체로 전달한다. 이것을 MVC 패턴이라고 한다.

<br>

# DispatcherServlet

![png](/design-pattern/img/spring_mvc_architecture.png)

SpringBoot에서 제공하는 기술로 모든 HTTP 요청이 일단 DispatcherServlet으로 들어간다. 그리고 요청에 맞는 적절한 Controller를 찾아 요청을 위임한다. 즉, 핵심 기능을 처리하는 Controller 앞에서 처리하기 때문에 Front Controller라고도 한다. 

![png](/design-pattern/img/DispatcherServlet_diagram.png)

- HTTP Request handlers/controllers를 위한 central dispatcher
- Web Request를 처리하기 위해 등록된 handler를 보냄
- JavaBeans configuration mechanism 기반
- ```@RequestMapping``` 은 HandlerMapping에 해당되거나 Dispatcher에 HandlerAdapter가 있는 경우 처리됨
<br>

- SpringBoot는 ```DispatcherServlet```을 자동등록하고 모든 경로(**urlPatterns="/"**)에 대해 mapping 
- Servlet이 호출되면 부모인 HttpServlet의 ```service()``` 가 호출
- FrameworkServlet는 HttpServlet의 ```service()```를 Override 함 
<br>

## FrameworkServlet

```java
@Override
protected void service(HttpServletRequest request, HttpServletResponse response)
        throws ServletException, IOException {

    HttpMethod httpMethod = HttpMethod.resolve(request.getMethod());
    if (httpMethod == HttpMethod.PATCH || httpMethod == null) {
        processRequest(request, response);
    }
    else {  
        super.service(request, response);  // HttpServlet 으로 이동
    }
}
```
- HttpMethod가 ```PATCH``` 이거나 ```null``` 일 경우 ```processRequest()``` 호출
- 그외의 경우 HttpServlet의 ```service()``` 호출
<br>

### HttpServlet.service(HttpServletRequest, HttpServletResponse)

```java
package javax.servlet.http;

public abstract class HttpServlet extends GenericServlet {

    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {...}

    protected void doPost(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {...}

    protected void doPut(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {...}

    protected void doDelete(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {...}

    /*
            Public service에서 HTTP request을 수신하고 doMethod 호출
            javax.servlet.Servlet.service 의 HTTP-specific version의 Method로 Override 할 필요없음
     */
    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        String method = req.getMethod();

        if (method.equals(METHOD_GET)) {
            doGet(req, resp);
        } else if (method.equals(METHOD_POST)) {
            doPost(req, resp);
        } else if (method.equals(METHOD_PUT)) {
            doPut(req, resp);
        } else if (method.equals(METHOD_DELETE)) {
            doDelete(req, resp);
        }
    }
}
```
- FrameworkServlet의 ```service()```가 호출되면 HttpMethod에 따라 doMethod를 호출
- ```doGet()```: Servlet이 ```HTTP GET``` 요청을 지원하는 경우 ```doGet()```을 재구현
- ```doPost()```: Servlet이 ```HTTP POST``` 요청을 지원하는 경우 ```doPost()```을 재구현
<br>

## processRequest(HttpServletRequest, HttpServletResponse)

```java
protected final void processRequest(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException{   
    try {
        doService(request,response);
    }
}
```
- HttpMethod가 ```PATCH``` 이거나 ```null``` 일 경우 호출되는 메서드
- 요청을 처리하고 결과에 상관없이 event 발생 
- event는 ```doService()```가 처리
<br>

```java
// FrameworkServlet
protected abstract void doService(HttpServletRequest request, HttpServletResponse response) throws Exception;
```

```java
// DispatcherServlet
@Override
protected void doService(HttpServletRequest request, HttpServletResponse response) throws Exception {
    logRequest(request);

    try {
        doDispatch(request, response);
    }
}
```
```java
/*
        Handler에 대한 actual dispatching을 처리
        HandlerAdapter 나 handler가 가능한 방법을 알아서 찾는다.
 */
protected void doDispatch(HttpServletRequest request, HttpServletResponse response) throws Exception {...}
```
- GET, POST, PUT and DELETE 에 대한 callback 수신하여 요청 처리 작업을 수행하기 위해 하위 클래스는 해당 method를 구현해야 한다.
- FrameworkServlet의 abstract method인 ```doService()```를 하위 클래스인 **DispatcherServlet에서 재구현**한다.
<br>

## Servlet

```java
@WebServlet(name="helloServlet", urlPatterns = "/hello")
public class HelloServlet extends HttpServlet {
@Override
protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String username = request.getParameter("username");

        response.setContentType("text/plain"); // content type (response header)
        response.setCharacterEncoding("utf-8"); // content type (response header)
    }
}
```
```@RequestMapping``` 을 사용하지 않고 요청을 처리하는 메서드를 작성한다면 위와 같이 작성하면 된다.
helloServlet이라는 이름을 갖는 Servlet이며 ```localhost:8080/hello``` 로 들어오는 요청을 처리한다.
HttpServlet의 **service** 메서드를 **@Override**하여 작성해야만 한다.
<br>

> 즉, Servlet을 지금의 Controller와 같은 개념이라고 생각하면 될 것같다. (확신할 수 없음)

<br>

HTTP Request Message를 기반으로 HttpServletRequest와 HttpServletResponse 객체를 생성해 파라미터로 전달한다. 결과를 HttpServletResponse 객체에 담아 전달한다. 이때 어떤 형식으로 파싱할 것인지 지정할 수 있다.

> 그러면 Handler Adapter에서 HttpServletResponse 객체에 담긴 데이터를 지정한 타입으로 파싱해 HTTP Response Message에 담아 클라이언트에게 전달한다. (파싱하는 지점이 Handler Adapter 라는 것을 확신할 수 없음)


## HandlerMapping

![png](/design-pattern/img/spring_mvc_architecture_handler_mapping.png)

```java
package org.springframework.web.servlet;

public interface HandlerMapping {...}
```
- 사용자의 요청을 처리할 Controller를 찾는 Controller URL Mapping
- 요청 URL에 해당하는 Controller 정보를 저장하는 Table 존재
- table에 저장된 정보에 따라 해당 클래스 또는 메서드에 Mapping
- Default: ```BeanNameUrlHandlerMapping``` or ```RequestMappingHandlerMapping```
- Servlet's application context 내에 Bean 으로 등록되어 있음
- HandlerMapping interface 를 구현하거나 Overriding 할 수 있음
<br>

## HandlerInterceptor

```java
package org.springframework.web.servlet;

public interface HandlerInterceptor {

    /*
            HandlerMapping이 적절한 Handler를 결정한 후 
            HandlerAdapter가 Handler를 호출하기 전에 해당 Method를 호출한다.
            
            execution chain이 다음 interceptor나 handler 자체를 진행해야하는 경우 true를 반환
            그렇지 않은 경우 DispatcherServlet은 해당 Interceptor가 이미 응답을 처리했다고 가정
     */
    default boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        return true;
    }

    /*
            HandlerAdapter가 handler를 실행한 후
            DispatcherServlet가 view를 render 하기 전에 해당 Method를 호출한다.
            
            handler가 반혼한 ModelAndView를 파라미터로 받는다.
     */
    default void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler,
                            @Nullable ModelAndView modelAndView) throws Exception {
    }
}
```
- DispatcherServlet은 지정된 순서로 각 HandlerInterceptor의 preHandle method를 호출한다.
- 모든 preHandle method가 true를 반환하면 Handler 자체를 호출한다.
<br>

## HandlerAdapter

```java
package org.springframework.web.servlet;

public interface HandlerAdapter {
    
    /*
            파라미터로 전달받은 Handler를 지원하는지 안하는지 판별
     */
    boolean supports(Object handler);

    /*
            handler로 요청을 처리한다.
            ModelAndView 객체를 반환
     */
    @Nullable
    ModelAndView handle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception;

}
```
```java
package org.springframework.web.servlet;

public class ModelAndView {

    @Nullable
    private Object view;

    @Nullable
    private ModelMap model;

    @Nullable
    private HttpStatus status;
}
```

- Default: ```HttpRequestHandlerAdapter```, ```HttpRequestHandler```, ```RequestMappingHandlerAdapter``` 등 
- Handler interface 를 사용
- Application context 내에 Bean 으로 등록
<br>

# ViewResolver

![png](/design-pattern/img/spring_mvc_architecture_ViewResolver.png)

```java
package org.springframework.web.servlet;

public interface ViewResolver {
    
    @Nullable
    View resolveViewName(String viewName, Locale locale) throws Exception;
}
```
- Resolve the given view by name
- resolve 할 view 이름을 파라미터로 받는다.
- resolve 에 성공하면 ```View``` object를, 실패하면 null 을 return
<br>

```java
package org.springframework.web.servlet;

public interface View {
    @Nullable
    default String getContentType() { 
        return null;
    }
    
    void render(@Nullable Map<String, ?> model, HttpServletRequest request, HttpServletResponse response)
            throws Exception;
}
```
- 해당되는 model의 주어진 view를 render 한다.
- Rendering: 서버로부터 HTML 파일을 받아 브라우저에 뿌려주는 과정

<br>

# Model

- Controller에서 View로 객체를 전달하는데 사용
- Key-Value 구조
- Spring에서 제공하는 Model 인터페이스를 가장 많이 사용
- Model 인터페이스가 제공하는 ```addAttribute()``` 로 데이터를 Model에 삽입

<br>

# Controller

- Bean
- @Component를 구체화하여 주로 @Controller, @Service, @Repository 를 사용
- @RequestMapping에 명시된 Value(url)과 method가 일치하면 해당 메서드가 수행
- @RequestMapping보다는 @GetMapping, @PostMapping 같이 명시된 annotation을 주로 사용
<br>

- Class Level Mapping: 해당 클래스의 모든 메서드에 적용
- Handler Level Mapping: 특정 메서드에만 적용