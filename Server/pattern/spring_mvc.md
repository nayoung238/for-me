# Spring MVC

![png](/Server/_img/Spring_MVC/springmvc_architecture.png)

> 출처: https://gmlwjd9405.github.io/2018/12/20/spring-mvc-framework.html

<br>

# DispatcherServlet

![png](/Server/_img/Spring_MVC/springmvc_architecture_dispatcher_servlet.png)

![png](/Server/_img/Spring_MVC/DispatcherServlet_diagram.png)

- HTTP request handlers/controllers 를 위한 central dispatcher
- web request 를 처리하기 위해 등록된 handler 를 보냄
- JavaBeans configuration mechanism 을 기반
- ```@RequestMapping``` 은 HandlerMapping 에 해당되거나 Dispatcher 에 HandlerAdapter 가 있는 경우 처리됨
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
- HttpMethod가 ```PATCH``` 이거나 ```null``` 일 경우: ```processRequest()``` 호출
- 그외의 경우: HttpServlet의 ```service()``` 호출
<br>

### HttpServlet.service(HttpServletRequest, HttpServletResponse)

```java
protected void service(HttpServletRequest req, HttpServletResponse resp)
        throws ServletException, IOException {
        
    String method = req.getMethod();
    
    if (method.equals(METHOD_GET)) { doGet(req, resp);}
    else if (method.equals(METHOD_POST)) { doPost(req, resp);} 
    else if (method.equals(METHOD_PUT)) { doPut(req, resp);} 
    else if (method.equals(METHOD_DELETE)) { doDelete(req, resp);}
}
```
- Public service에서 HTTP request을 수신하고 doMethod 호출
- javax.servlet.Servlet.service 의 HTTP-specific version의 Method로 Override 할 필요없음 
<br>

### processRequest(HttpServletRequest, HttpServletResponse)

```java
protected final void processRequest(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException{   
    try {
        doService(request,response);
    }
}
```
- 요청을 처리하고 결과에 상관없이 event 발생
- event는 ```doService()```가 처리
<br>

```java
// FrameworkServlet
protected abstract void doService(HttpServletRequest request, HttpServletResponse response)
			throws Exception;
```

```java
// DispatcherServlet
@Override
protected void doService(HttpServletRequest request, HttpServletResponse response) throws Exception {
		logRequest(request);
}
```
- GET, POST, PUT and DELETE 에 대한 callback 수신하여 요청 처리 작업을 수행하기 위해 하위 클래스는 해당 method를 구현해야 한다.
- FrameworkServlet의 abstract method인 ```doService()```를 하위 클래스인 DispatcherServlet에서 재구현한다.
<br>

## HandlerMapping

![png](/Server/_img/Spring_MVC/springmvc_architecture_handler_mapping.png)

```java
package org.springframework.web.servlet;

public interface HandlerMapping {...}
```
- Default: ```BeanNameUrlHandlerMapping``` or ```RequestMappingHandlerMapping```
- Servlet's application context 내에 Bean 으로 등록되어 있음
- HandlerMapping interface 를 구현하거나 Overriding 할 수 있음
<br>

- DispatcherServlet은 지정된 순서로 각 HandlerInterceptor의 preHandle method를 호출한다.
- 모든 preHandle method가 true를 반환하면 Handler 자체를 호출한다.
<br>

## HandlerAdapter

- Default: ```HttpRequestHandlerAdapter```, ```HttpRequestHandler```, ```RequestMappingHandlerAdapter``` 등 
- Handler interface 를 사용
- Application context 내에 Bean 으로 등록
<br>

## HttpServlet

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
}
```
- ```doGet()```: Servlet이 ```HTTP GET``` 요청을 지원하는 경우 ```doGet()```을 재구현
- ```doPost()```: Servlet이 ```HTTP POST``` 요청을 지원하는 경우 ```doPost()```을 재구현
<br>

- FrameworkServlet의 ```service()```가 호출되면 HttpMethod에 따 doMethod를 호출