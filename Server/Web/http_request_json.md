# Request JSON

HTTP message body 에 데이터를 직접 담아서 요청하는 경우 ```@RequestParam``` 과 ```@ModelAttribute``` 를 사용할 수 없다.<br>

HTTP message body 에 데이터를 직접 담아서 요청하는 경우는 주로 HTTP API 에서 사용되며 JSON 형식이 많이 사용된다.<br>
단순 TEXT 와 JSON 형식을 처리하는 방법에 대해 알아본다.

<br>

# 단순 TEXT 를 HTTP message body 에 담아 전송

## getInputStream method

```java
@PostMapping("/request-body-string")
public void requestBodyString(HttpServletRequest request, HttpServletResponse response) throws IOException {
        ServletInputStream inputStream = request.getInputStream();
        String messageBody = StreamUtils.copyToString(inputStream, StandardCharsets.UTF-8);
    
        response.getWriter().write("ok");
}
```

```HttpServletRequest``` 의 ```getInputStream()``` 으로 HTTP message body 데이터를 읽는다.<br>
String은 바이트 코드이므로 ```StreamUtils.copyToString()```에 어떤 encoding으로 문자를 바꿀 것인지 설정해야 한다.<br>

> Encoding: 사용자가 입력한 문자나 기호들을 컴퓨터가 이용할 수 있는 Binary data로 만드는 것<br>
> Decoding: Binary data를 사람이 읽을 수 있는 문자로 복구하는 것<br>
> 
> Character Set: Encoding과 Decoding은 정해진 규칙에 의해 수행된다. 
> 이렇게 미리 정해진 규칙을 Character set(문자셋)이라 하며 ASCII, EBCDIC 등이 있다.<br>
> 
> reference: https://limvo.tistory.com/7

<br>

## InputStream, OutputStream

위 코드에선 Servlet 의 모든 기능이 필요하지 않으니 stream 만 파라미터로 사용하도록 수정한다.

```java
@PostMapping("/request-body-string")
public void requestBodyString(InputStream inputStream, Writer responseWriter) throws IOException {
        String messageBody = StreamUtils.copyToString(inputStream, StandardCharsets.UTF-8);
        response.getWriter().write("ok");
}
```

- ```InputStream (Reader)``` : HTTP request message body 의 내용을 직접 조회
- ```OutputStream (Writer)``` : HTTP response message body 에 결과를 직접 출력

<br>

## HttpEntity

```java
@PostMapping("/request-body-string")
public HttpEntity<String> requestBodyString(HttpEntity<String> httpEntity) throws IOException {
        String messageBody = httpEntity.getBody();
        return new HttpEntity<>("ok");
}
```

HttpEntity 는 HTTP header, body 정보를 조회할 수 있다.<br>
HttpEntity 사용 시 HTTP message Converter 가 HTTP body 에 있는 데이터를 알아서 문자로 encoding 해주기 때문에 직접 작성할 필요없다.<br>
즉, ```httpEntity.getBody()``` method 는 이미 변환된 HTTP body 를 꺼낸다.<br>

HTTP response body 에 message 에 직접 데이터를 넣어 반환하며 서로 message 를 주고 받는 형태가 된다.<br>
이땐 view 조회가 불가능하다.

<br>

## @RequestBody

```java
@ResponseBody
@PostMapping("/request-body-string")
public String requestBodyString(@RequestBody String messageBody) throws IOException {
        return "ok";
}
```

많이 사용되는 방식으로 ```@RequestBody``` 로 HTTP message body 정보를 조회한다.<br>
```@ResponseBody``` 로 응답 결과를 HTTP message body 에 직접 담아 전달한다.<br>
이때도 view 조회가 불가능하다.

<br>

# HTTP API - JSON 형식

## getInputStream

```java
private ObjectMapper objectMapper = new ObjectMapper();

@PostMapping("/request-body-json")
public void requestBodyJson(HttpServletRequest request, HttpServletResponse response) throws IOException {
        ServletInputStream inputStream = request.getInputStream();
        String messageBody = StreamUtils.copyToString(inputStream, StandardCharsets.UTF-8);
        
        Member member = objectMapper.readValue(messageBody, Member.class);
        response.getWriter().write("ok");
}
```
HttpServletRequest 로 HTTP message body 에서 데이터를 읽어와 문자로 변환한다.<br>

문자로 된 JSON data 를 java object 인 Member 로 변환하기 위해 Jackson 라이브러리인 **ObjectMapper**를 사용한다.<br>
objectMapper의 ```readValue()``` 로 JSON data 를 Member type 으로 변경한다.

<br>

## @RequestBody

```java
private ObjectMapper objectMapper = new ObjectMapper();

@ResponseBody
@PostMapping("/request-body-json")
public String requestBodyJson(@RequestBody String messageBody) throws IOException {
        Member member = objectMapper.readValue(messageBody, Member.class);
        return "ok";
}
```
```@RequestBody``` 로 HTTP message 에서 data 를 읽어 messageBody에 저장한다.<br>
문자로 된 JSON data 인 messageBody 를 objectMapper 의 ```readValue()``` method 를 통해 java object로 변환한다.

<br>

## @RequestBody 에 객체 지정

```java
@ResponseBody
@PostMapping("/request-body-json")
public Member requestBodyJson(@RequestBody Member member) {
        return member;
}
```

```@RequestBody``` 에 직접 만든 객체를 사용하면 문자로 변환한 뒤 다시 JSON 으로 변환 과정이 필요없다.<br>
```HttpEntity``` 나 ```@RequestBody``` 를 사용하면 **HTTP message Converter** 가 HTTP message body 의 내용을 원하는 문자나 객체로 변환해주기 때문이다.<br>

```@RequestBody``` 는 **생략이 불가능**하다.<br>
```@ModelAttribute``` 와 ```@RequestParam``` 을 생략하면 단순타입은 ```@RequestParam``` 으로, 나머지는 ```@ModelAttribute``` 로 인식한다.<br>
```@RequestBody``` 를 생략하면 ```@ModelAttribute``` 가 적용되므로 HTTP message body 에 처리되지 않고, **Query parameter 로 처리**된다.<br>

또한, JSON 으로 처리하려면 HTTP 요청시 content-type 이 application/json 으로 설정해야한다.<br>
```@ResponseBody``` 사용하면 JSON 으로 요청받은 data 를 JSON 으로 그대로 응답한다.<br>
```@RequestBody``` 는 **JSON 요청**을 **HTTP message Converter**에 의해 객체로 변환하고<br>
```@ResponseBody``` 는 변환된 객체를 **HTTP message Converter**에 의해 **JSON**으로 응답한다.