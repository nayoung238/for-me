# HttpServletRequest

HttpServletRequest, HttpServletResponse 는 HTTP request message, HTTP response message 를 편리하게 사용하도록 도와주는 객체이다.<br>

HttpServletRequest 는 HTTP request message 를 파싱하고 그 결과를 HttpServletRequest 객체에 담는다.<br>
HttpServletRequest 는 **종료되기 전까지 임시 저장소** 기능을 제공한다.<br>

- ```request.setAttribute(name, value)``` 로 데이터를 임시 저장하고
- ```request.getAttribute(name)``` 로 임시 저장된 데이터를 제공한다.
- ```request.getSession(create:true)``` 로 세션 관리 기능도 제공한다.

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
```@WebServlet``` annotation 에서 name은 Servlet 이름, urlPatterns은 매핑할 url 이다.<br>
```localhost:8080/hello``` 요청시 @WebServlet 은 HelloServlet 을 실행한다.<br>

HTTP request message를 기반으로 request, response 객체를 생성하고 ```/hello``` url 로 매핑된 helloServlet 을 호출한다.<br>
response 객체에 response message를 담아 client에게 보내면 **WAS가 HTTP response message를 만들어 전송**한다.<br>

request.getParameter 로 원하는 쿼리를 파싱 가능하다.<br>
response.set 으로 원하는 content type 을 response header 에 설정 가능하다.

![png](/spring/img/http_servlet_request_result.png)

```localhost:8080/hello?username=kim``` 작성시 content type 원하는 설정으로 되었으며 query string 이 파싱된 것을 볼 수 있다.

<br>

## GET - url query parameter

chrome에 hello 라고 검색하면 아래 url에서 볼 수 있듯이 많은 Query data가 포함된다.

> google.com/search?q=hello&rlz=1C5CHFA_enKR944KR944&oq=hello&aqs=chrome.0.69i59l3j0j0i433j69i60l3.1622j1j4&sourceid=chrome&ie=UTF-8

Message-Body 없이 **url 의 Query parameter에 모든 데이터를 포함**해 전달하는 방식으로 검색, 필터, 페이징 등에서 많이 사용하는 방식이다.<br>
```?``` 으로 시작하고 ```&``` 로 parameter 를 추가한다.<br>

아래 코드로 모든 파라미터 조회가 가능하다.

```java
@WebServlet(name="requestParamServlet", urlPatterns = "/request-param")
public class RequestParamServlet extends HttpServlet {
    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.getParameterNames().asIterator()
        .forEachRemaining(paramName -> System.out.println(paramName + " = " + request.getParameter(paramName)));
    }
}
```
<br>

파라미터 이름이 같은 데이터가 들어올 수 있지만 첫번째 값만 반환되고 거의 사용되지 않는다.

```java
String[] usernames = request.getParameterValues("username"); // 중복 파라미터를 모두 조회
```

<br>

## POST - HTML Form

![png](/spring/img/username_age_input_form.png)

회원가입, 상품주문 같은 형식에 HTML Form 이 주로 사용된다.

![png](/spring/img/http_request_message_for_input_form.png)

HTTP request message 의 content-type 은 HTTP message body 의 데이터 형식을 지정한다.<br>
**Content-Type: application/x-www-form-urlencoded**이면 **HTML Form으로 전달**한다는 의미이다.<br>
HTML Form 에서 data 입력 후 전송하면 웹 브라우저에서 HTTP request message 를 생성하여 **message body 에** query parameter 형식과 동일하게 전달된다.<br>

GET - url query parameter 방식에선 message body 를 사용하지 않고 **url에 Query parameter를 포함**시켰고,<br>
POST - HTML Form 방식은 content-type 지정 후 **Message body에 데이터를 포함**해 전송하므로 client 입장에선 두 방식에 차이가 있다.<br>

하지만 서버 입장에선 두 방식의 형식이 동일하므로 ```request.getParameter()```로 구분없이 조회한다.<br>
즉, ```request.getParameter()```는 GET - url query parameter 와 POST - HTML Form 을 지원한다.<br>

<br>

## HTTP API

Message body에 JSON, XML, TEXT 정보를 담아 전송하는 방식으로 REST API 라고 하는 HTTP API 에서 주로 사용된다.<br>
현재 대부분의 데이터 형식은 JSON 이며 POST, PUT, PATCH 에서도 사용가능하다.<br>

```java
@WebServlet(name="requestBodyStringServlet", urlPatterns = "/request-body-string")
public class RequestBodyStringServlet extends HttpServlet {

    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        // message body 내용을 byte 코드로 얻는다.
        ServletInputStream inputStream = request.getInputStream();

        // byte 코드를 string으로 변경 (encoding 정보 포함)
        String messageBody = StreamUtils.copyToString(inputStream, StandardCharsets.UTF_8);
    }
}
```
inputStream 을 사용하여 text 로 주고 받는다. 
하지만 요즘은 text 가 아닌 **JSON** 형식을 주로 사용한다.

```java
@Getter @Setter
public class Data {
    private String username;
    private int age;
}
```
파싱하고자 하는 객체를 만든다.<br>

JSON 결과를 파싱해서 자바 객체로 변환하려면 Jackson, Gson 같은 **JSON 변환 라이브러리**를 사용해야한다.<br>
SpringBoot MVC 는 Jackson 라이브러리 (**ObjectMapper**) 를 기본적으로 제공한다.

```java
@WebServlet(name="requestBodyJsonServlet", urlPatterns = "/request-body-json")
public class RequestBodyJsonServlet extends HttpServlet {

    private ObjectMapper objectMapper = new ObjectMapper();
    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        ServletInputStream inputStream = request.getInputStream();
        String messageBody = StreamUtils.copyToString(inputStream, StandardCharsets.UTF_8);

        HelloData helloData = objectMapper.readValue(messageBody, Data.class);
        System.out.println("helloData.getUsername() = " + helloData.getUsername());
        System.out.println("helloData.getAge() = " + helloData.getAge());
    }
}
```
Message를 읽는 것은 단순 text 와 동일하게 inputStream 을 사용한다.<br>
ObjectMapper의 ```readValue()```로 message body에 원하는 객체를 파싱한다.