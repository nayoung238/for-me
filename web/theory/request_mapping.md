# request mapping

Handler(Controller)가 호출되기 위해선 HandlerMapping 과 HandlerAdapter 가 필요하다.

![png](/web/_img/mvc_architecture.png)

## HandlerMapping

handlerMapping 에서 Handler (실제로는 Controller) 를 찾는다. 우선순위는 다음과 같다.

0. RequestMappingHandlerMapping : annotation 기반의 controller 인 @RequestMapping 에서 사용
1. BeanNameUrlHandlerMapping : spring Bean 의 이름으로 handler 를 찾는다.

<br>

## HandlerAdapter

HandlerMapping 을 통해 찾은 Handler 를 실행할 수 있는 Handler Adapter 가 필요하다. 우선순위는 다음과 같다.

0. RequestMappingHandlerAdapter : annotation 기반의 controller 인 @RequestMapping 에서 사용
1. HttpRequestHandlerAdapter : HttpRequestHandler 처리
2. SimpleControllerHandlerAdapter : Controller Interface (annotation 이 아닌 web.servlet.mvc 에 정의된 Interface Controller)

HandlerMapping 과 HandlerAdapter 에서 가장 우선순위가 높은 것은 **RequestMappingHandlerMapping**, **RequestMappingHandlerAdapter** 이다. 
@RequestMapping 의 앞글자를 따서 만든 이름이며 현재 가장 많이 사용되는 방식이다.

<br>

- @RequestMapping 기반의 controller
- Interface Controller 를 구현한 controller
- Interface HttpRequestHandler 를 구현한 controller

3가지 controller 에 대해 알아볼 것이며 @RequestMapping 기반의 controller 을 주로 사용한다.

<br>

# @RequestMapping 기반 Controller

Controller 로 등록하기 위해 @Controller 와 @RequestMapping annotation 을 추가한다.

## @Controller

```java
@Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Ducumented
@Component
public @interface Controller {
    /**
     * The value may indicate a suggestion for a logical component name,
     * to be turned into a Spring bean in case of an autodetected component.
     * @return the suggested component name, if any (or empty String otherwise)
     */
    @AliasFor(annotation = Component.class)
    String value() default "";
}
```

@Controller 은 스프링이 자동으로 Spring Bean 으로 등록하며 @Component 를 포함하고 있어서 스캔대상이 된다.<br>
spring MVC 에서 RequestMappingHandlerMapping 이 @Controller 가 있는 클래스를 Handler 로 인식한다.<br>

<br>

## @RequestMapping

```java
@Controller
@RequestMapping("/springmvc/members")
public class MemberController {
    
    private MemberRepository memberRepository = MemberController.getInstance();
    
    @RequestMapping("/new-form")
    public ModelAndView newForm() {
        return new ModelAndView("new-form");
    }
    
    @RequestMapping
    public ModelAndView members() {
        List<Member> members = memberRepository.findAll();
        ModelAndView mv = new ModelAndView("members");
        mv.addObject("members", members);
        return mv;
    }
}
```

@RequestMapping 은 요청 정보를 매핑한다.
**RequestMappingHandlerMapping**은 spring Bean 중 **@RequestMapping** 또는 **@Controller**가 클래스 레벨에 있는 경우를 mapping 정보로 인식한다.
또한, RequestMappingHandlerMapping 이 @RequestMapping 과 요청 정보가 동일한 것을 찾아 해당 메소드를 호출한다.<br>

데이터 처리를 위해 **Spring Framework 에 Interface 로 정의된 Model 을 사용**한다. 
데이터는 파라미터로 넘어온 model 에 저장하고 View 논리 이름만 반환한다.<br>

class 레벨에 @RequestMapping 을 두는 경우 모든 method 레벨의 중복을 처리하며 method 레벨과 조합해서 사용된다. 
```/new-form``` 과 ```/new-form/``` 는 다른 url 이지만, 같은 ```/new-form``` 으로 mapping 된다.<br>

<br>

## @RequestParam

```java
@RequestMapping("/save")
public ModelAndView save(HttpServletRequest request, HttpServletResponse response) {
    String username = request.getParameter("username");
    int age = Integer.parseInt(request.getParameter("age"));
    
    Member member = new Member(username, age);
    memberRespository.save(member);
    
    ModelAndView mv = new ModelAndView("save-result");
    mv.addObject("member", member);
    return mv;
}
```
받고자 하는 parameter 를 **HTTP Servlet** 으로 전달받으면 번거롭게 getParameter 을 사용해야 한다.

```java
@PostMapping("/save")
public String save(@RequestParam("username") String username, 
                    @RequestParam("age") int age, Model model) {
    
    Member member = new Member(username, age);
    memberRepository.save(member);
    
    model.addAttribute("member", member);
    return "save-result";
}
```
하지만 받고자 하는 parameter 를 **@RequestParam**으로 설정하면 그대로 가져다 쓰면된다.

<br>

## @RestController

@Controller 는 **반환 타입이 String 이면 view 이름으로 인식**하므로 view 를 찾고 렌더링된다.<br>
@RestController 는 view 이름으로 인식하지 않고 data 자체를 return 하는 목적으로 **HTTP message body**에 해당 data 를 추가한다.
즉, view 가 필요없고 API 만을 지원하는 서비스에 사용한다.<br>

@RestController 를 클래스 레벨에 작성시 모든 메소드가 적용되므로, 일부 메소드에만 적용시키려면 해당 메소드에 **@ResponseBody**를 작성한다.<br>

<br>

## HTTP method

 @RequestMapping 은 GET, POST 등 설정으로 HTTP method 를 구분한다. HTTP 요청이 GET 이라면 아래와 같이 설정한다.

```java
@RequestMapping(value = "/save", method = RequestMethod.GET)
```

하지만 위 코드를 **@GetMapping("/save")** 으로 간단하게 사용할 수 있다. 
GET, POST, PUT, PATCH, DELETE 모두 annotation 이 존재한다.<br>

@GetMapping 에 GET 에 아닌 다른 method 를 요청하면 **405(Method Not Allowed)** 상태코드를 반환한다.<br>

<br>

## @PathVariable

```java
@GetMapping("/members/{userId}")
public String findUser(@PathVariable("userId") String userId) {
    // 생략
}
```

전체 회원 중 회원 A를 찾는 리소스 경로는 **/members/memberA**, 회원 B를 찾는 리소스 경로는 **/members/memberB** 이다.
이는 리소스 경로에 식별자를 넣어 처리한다.

```java
@GetMapping("/members/{userId}")
public String findUser(@PathVariable String userId) {
    // 생략
}
```
@PathVariable 의 이름과 parameter 이름이 같으면 생략 가능하며, 아래 코드와 같이 다중 사용도 가능하다.

```java
@GetMapping("/members/{userId}/orders/{orderId}")
public String updateUser(@PathVarialbe String userId, @PathVariable Long orderId) {
    // 생략    
}
```

<br>

# Controller Interface 구현한 controller

> 여기서 설명하는 Controller 는 controller annotation 이 아니라 web.servlet.mvc 에 정의된 interface controller 이다.<br>
@Controller 와 interface Controller 는 다르다.

```java
@FunctionalInterface
public interface Controller {
    /**
     * Process the request and return a ModelAndView object which the DispatcherServlet
     * will render. A {@code null} return value is not an error: it indicates that
     * this object completed request processing itself and that there is therefore no
     * ModelAndView to render.
     * @param request current HTTP request
     * @param response current HTTP response
     * @return a ModelAndView to render, or {@code null} if handled directly
     * @throws Exception in case of errors
     */
    @Nullable
    ModelAndView handleRequest(HttpServletRequest request, HttpServletResponse response) throws Exception;
}
```
interface Controller 도 ModelAndView 객체를 반환한다.

```java
import org.springframework.web.servlet.mvc.Controller;

@Component("/springmvc/controller")
public class controller implements Controller {
    @Override
    public ModelAndView handleRequest(HttpServletRequest request, HttpServletResponse response) throws Exception {
        return new ModelAndView("new-form");
    }
}
```

해당 controller 는 @Component 에 의해 "/springmvc/controller" 라는 이름의 spring Bean 으로 등록된다.
handlerMapping 에서 해당 controller 를 **Bean 이름으로 URL 을 mapping** 하므로 **BeanNameUrlHandlerMapping** 이 처리한다.<br>

interface Controller 를 처리하는 HandlerAdapter 는 **SimpleControllerHandlerAdapter** 이다. SimpleControllerHandlerAdapter 는 handlerMapping 에 의해 반환된 handler(controller)를 내부에서 실행한 뒤 결과를 반환한다.<br>

<br>

# HttpRequestHandler Interface 구현한 controller

```java
@FunctionalInterface
public interface HttpRequestHandler {
    /**
     * Process the given request, generating a response.
     * @param request current HTTP request
     * @param response current HTTP response
     * @throws ServletException in case of general errors
     * @throws IOException in case of I/O errors
     */
    void handleRequest(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException;
}
```

interface HttpRequestHandler 는 Servlet 과 가장 유사한 형태의 Handler 이다.

```java
import org.springframework.web.HttpRequestHandler;

@Component("/springmvc/request-handler")
public class httpRequestHandler implements HttpRequestHandler {
    
    @Override
    public void httpRequestHandler(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOExcpetion {}
}
```

해당 handler 는 @Component 에 의해 "/springmvc/request-handler" 라는 이름의 spring Bean 으로 등록된다.
handlerMapping 에서 해당 handler 를 **Bean 이름으로 URL 을 mapping**하므로 **BeanNameUrlHandlerMapping**이 처리한다.<br>

**HttpRequestHandlerAdapter** 가 HttpRequestHandler interface 를 지원한다.