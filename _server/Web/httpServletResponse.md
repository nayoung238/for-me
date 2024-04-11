## HTTP response message

HTTP response message 의 구조는 다음과 같다.

![png](/_server/_img/http_response_message.png)

- start line : HTTP method, protocol 등
- header : host, content type 등
- body : form parameter 형식 조회, message body 데이터 조회

<br>

## HttpServletResponse

HttpServletRequest, HttpServletResponse는 HTTP request message, HTTP response message를 편리하게 사용하도록 도와주는 객체이다.

HttpServletResponse는 HTTP response message에 HTTP 응답코드 지정, header와 body를 생성하고 쿠키나 Redirect 기능을 제공한다.

```java
@WebServlet(name="responseHeaderServlet", urlPatterns = "/response-header")
public class ResponseHeaderServlet extends HttpServlet {
    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        // [status-line]
        // Status code (200) indicating the request succeeded normally.
        response.setStatus(HttpServletResponse.SC_OK);  

        // [response-headers]
        response.setHeader("Content-Type", "text/plain;charset=utf-8");
        response.setHeader("Cache-Control", "no-cache, no-store, must-revalidate");

        // [message-body]
        PrintWriter writer = response.getWriter();
        writer.println("response ok");  // 원하는 message body 를 적는다.
    }
}
```
response.setStatus(HttpServletResponse.SC_OK) 로 원하는 응답코드를 설정하고 response.setHeader()로 원하는 header 를 추가한다. 
HttpServletResponse 에 여러 상태가 정의되어있다.

- SC_OK = 200 (the request succeeded normally)
- SC_FOUND = 302 (the resource reside temporarily under a different URI)
- SC_NOT_FOUND = 404 (he requested resource is not available)

![png](/_server/_img/http_servlet_response_result.png)

<br>

## HTTP response message - text, HTML

HTTP 응답을 단순 text 로 반환하려면 getWriter()를 사용한다.

```java
PrintWriter writer = response.getWriter();
writer.println("response ok");  // 원하는 message body 를 적는다.
```

<br>

HTTP 응답으로 HTML 을 반환하려면 content-type 을 "text/html" 으로 지정한다.

```java
response.setContentType("text/html");
```
<br>

## HTTP response message - JSON

HTTP 응답을 JSON 으로 반환하려면 content-type 을 "application/json" 으로 지정한다.

```java
private ObjectMapper objectMapper = new ObjectMapper();

    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //Content-type : application/json
        response.setContentType("application/json");
        response.setCharacterEncoding("utf-8");  // 의미없는 전달

        HelloData helloData = new Data();
        helloData.setUsername("kim");
        helloData.setAge(20);

        // {"username":"kim", "age":20}
        String result = objectMapper.writeValueAsString(Data);
        response.getWriter().write(result);
    }
}
```
**application/json** 은 **utf-8** 형식을 사용하도록 정의되어 있다.<br>
그러므로 ```response.setCharacterEncoding("utf-8");```을 작성하지 않아도 된다.
