# Request Parameter

HTTP request message 를 통해 client 에서 server 로 데이터 전달하는 방법은 다음과 같다.

- GET (query parameter) : url 의 query parameter 에 데이터 포함해서 전달
- POST (HTML Form) : message body 에 query parameter 형식과 동일하게 전달
- HTTP message body : HTTP message body 에 JSON, XML, TEXT 등의 형식 데이터를 담아 전달하며 주로 HTTP API 에서 사용
<br>

GET query parameter 전송 방식과 POST HTML Form 전송방식은 둘 다 같은 형식이므로 구분없이 조회가능하다.<br>
이것을 **Request parameter 조회**라고 한다.<br>

HttpServletRequest를 파라미터로 넘겨주고 getParameter로 쿼리 파라미터를 얻어도 되지만 ```@RequestParam```으로 더 편리하게 사용가능하다.

<br>

## @RequestParam

```java
@ResponseBody
@PostMapping("/request-param")
public String requestParam (@RequestParam("username") String memberName, @RequestParam int age) {
    return "ok";    
}
```

```@RequestParam```은 parameter 이름으로 binding 한다.<br>
```@RequestParam("name")```에 작성한 name 이 parameter 이름으로 사용된다.<br>
age 와 같이 HTTP parameter 이름이 변수명과 같으면 생략가능하다.<br>

**String, int 와 같이 단순 타입인 경우 @RequestParam 자체를 생략해도 된다.**<br>
**단순 타입**인 경우 @RequestParam 을 생략해도 알아서 query parameter 로 인식하기 때문이다.<br>

```@ResponseBody``` 는 ```@RestController``` 와 같이 View 조회가 아닌 HTTP message body에 내용을 저장한다.

<br>

## parameter 필수 여부

```java
@ResponseBody
@RequestMapping("/request-param-required")
public String requestParamRequired(
                @RequestParam(required = true) String username,
                @RequestParam(required = false) Integer age) {
        return "ok";
}
```

### required = true

```@RequestParam(required = true)``` 설정시 해당 Query parameter를 필수로 작성해야한다.<br>
```@RequestParam```의 required 옵션은 default가 **true로 설정**되어있다.<br>
만약 ```http://localhost:8080/request-param-required/?age=20``` 처럼 필수 parameter 를 생략하면 **400 예외**가 발생한다.

<br>

```text
http://localhost:8080/request-param-required/?username=
```
위 url 은 에러가 발생하지 않고 parameter 이름만 있고 값이 없는 경우는 **빈문자로 처리**한다.<br>
**빈문자와 null 은 서로 다르다.**

<br>

### required = false

```@RequestParam(required = false)``` 설정시 해당 Query parameter 를 생략해도된다.<br>
생략하면 **null** 값이 채워진다.<br>

age 같이 parameter value 에 숫자를 넣고 싶으면 **int 가 아닌 Integer**를 사용해야 한다.<br>
**int type 은 null 을 처리할 수 없고, Integer 는 객체이므로 null 값 처리가 가능**하다.

<br>

## default value

```java
@ResponseBody
@RequestMapping("/request-param-default")
public String requestParamDefault(
                @RequestParam(required = true, defaultValue = "guest") String username,
                @RequestParam(required = false, defaultValue = "-1") int age) {
    
        return "ok";
}
```

parameter 값을 넣지 않는 경우 defaultValue 로 기본 값을 적용할 수 있다.<br>
defaultValue 를 설정하면 무조건 값이 저장되므로 **required 설정은 무의미**해진다.

<br>

## @ModelAttribute

요청 파라미터를 객체로 만들어야 한다면 Spring에선 이 과정을 자동화해주는 ```@ModelAttribute``` 기능을 제공한다.<br>
요청 파라미터를 binding 받을 객체를 생성하고 ```@Data```를 적용한다.
```@Data``` 는 ```@Getter```, ```@Setter```, ```@ToString``` 등의 기능을 제공한다.

```java
@Data
public class Member{
    private String username;
    private int age;
}
```
```java
@ResponseBody
@RequestMapping("/model-attribute")
public String modelAttribute(@ModelAttribute Member member) {
    return "ok";    
}
```

요청이 들어오면 Spring MVC는 일단 Member 객체를 만든다.<br>
요청 파라미터로 **Member 객체의 property 를 찾아 setter 를 호출해 파라미터 값을 binding**한다.

<br>

```java
@ResponseBody
@RequestMapping("/model-attribute")
public String modelAttribute(Member member) {
    return "ok";
}
```
위에서 @RequestParam 도 생략해도 단순 타입을 파라미터로 인식하는 것이 가능하다고 했는데, ```@ModelAttribute``` 또한 생략이 가능하다.<br>
Spring에선 String, int 같은 단순 타입은 ```@RequestParam``` 으로, 나머지는 ```@ModelAttribute``` 로 인식한다.

<br>

### property

```getAge()```, ```setAge()``` method가 있으면 해당 객체는 age라는 property를 가지고 있다고 말한다.<br>
age property 값을 변경할 땐 ```setAge()```가, 조회할 땐 ```getAge()```가 호출된다.<br>

<br>

### BindException

Member 객체의 age 는 int type 이다.<br>
age 에 문자를 입력할 경우 BindException 이 발생하며 이를 바인딩 오류라고 한다.