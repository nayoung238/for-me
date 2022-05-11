# Cookie and Session

클라이언트는 HTTP 메시지에 정보를 담아 서버에게 요청을 보내는데 HTTP 프로토콜는 비연결성과 상태를 유지하지 않는 특징이 있다.<br>

- connectionless 비연결성 : 클라이언트 요청에 따른 응답을 보낸 후 클라이언트와 서버의 연결을 끊는다.
- stateless 무상태 : 클라이언트의 상태 정보를 기억하지 않는다.

데이터가 유지되지 않는 HTTP 프로토콜의 특징 때문에 사용자가 웹 사이트에게 요청을 할 때마다 자신을 인증을 해야 한다면 엄청난 자원과 시간이 낭비될 것이다. 어떤 클라이언트인지 알리기 위해 매번 HTTP 메시지에 클라이언트의 모든 정보를 담아 보내는 것은 비효율적이며 해커에게 정보를 털릴 수 있는 좋은 환경을 만들어 주는 것과 다를 바 없다.<br>

데이터가 유지되지 않는 HTTP 프로토콜의 특징을 보완하면서 데이터가 일정 기간 동안 안전하게 유지되도록 하기 위해 쿠키와 세션을 사용한다.

<br>

# 쿠키와 세션의 동작 방식

## 처음 접근하는 경우

![png](/Web/_img/cookie_and_session_flow.png)

1. 클라이언트가 로그인하겠다는 요청을 보내면
2. 서버는 DB 에서 해당 사용자를 찾는다.
3. 사용자가 존재하는 경우 서버는 클라이언트의 Request-Header 의 Cookie 를 채우기 위한 session-id 생성을 시도한다.
4. 서버는 해커로부터 데이터를 지키기 위한 **해석 불가능한 session-id** 를 생성한다.
5. 클라이언트 요청에 대한 응답을 session-id 와 함께 보낸다.
6. 클라이언트는 서버에게 전달받은 **session-id 가 식별자인 JSESSIONID 쿠키**를 로컬 PC에 저장한다.
7. 서버에게 또 다른 요청을 보내는 경우 JSESSIONID 쿠키를 서버에게 전달해 해당 사용자라는 것을 알린다.

위 동작 방식을 통해 쿠키와 세션을 간단하게 정의하면 다음과 같다.

- 쿠키 : 해당 사이트에 대한 사용자 식별자 key 로 **클라이언트 측에 저장**된다.
- 세션 : 해당 사이트에 대한 **여러 사용자를 구분**하기 위한 고유한 식별자로 **서버 측에 저장**된다.

<br>

## JSESSIONID 가 이미 존재하는 경우

해당 사이트에 대해 **JSESSIONID 를 가지고 있는 경우** 서버에게 요청을 보내는 과정은 다음과 같다.

![png](/Web/_img/cookie_and_session_flow2.png)

1. 서버에게 요청을 보낼 때 쿠키 보관소에 있는 JSESSIONID 쿠키를 함께 보낸다.
2. 서버는 전달받은 JSESSIONID 쿠키의 session-id 와 **세션 저장소에 있는 세션을 매칭**해 object 를 찾는다.
3. 해당 object 의 데이터를 가지고 오거나 요청을 처리한다.
4. 클라이언트 요청에 대한 응답을 보낼 때 JSESSIONID 쿠키도 함께 보낸다.

클라이언트가 서버에 요청을 보낼 때 자신이 어떤 사용자인지 알리는 쿠키를 보내는데 **Spring 에서 session 을 생성하면 해석 불가능한 session-id** 을 함께 만들어준다. 자신이 어떤 사용자인지 알리기 위해 모든 정보를 보내는 것보다 해석이 불가능한 session-id 만 전달하면 해커가 가져갈 수 있는 것은 session-id 뿐이다.<br>

> 물론 해커가 session-id만 훔쳐간다해서 안전한 것은 아니다.<br>
> 훔친 쿠키로 해커가 HTTP 요청을 보내면 서버가 세션 저장소에 해당 객체를 찾아 해커에게 보내는 세션 하이재킹 공격이 발생한다.
>
> HTTPS 를 사용하거나 세션에 유효 시간을 지정해 해결할 수 있다.

<br>

# Cookie 쿠키

쿠키란 웹 사이트에 대한 세션 키를 가지고 있는 기록 정보 파일로 key(session-id)와 값을 가진 text 형식의 파일로 클라이언트에 저장된다.<br>
session 이 존재한다면 서버에게 요청을 보낼 때 브라우저가 알아서 Request-header 에 쿠키를 넣어 보낸다.<br>
클라이언트에 300개의 쿠키가 저장할 수 있고 하나의 도메인 당 20개의 값만 가질 수 있으며 쿠기 1개는 최대 4KB 까지 저장 가능하다.<br>

## 쿠키 생성하기

> commit: https://github.com/evelyn82ny/session/commit/70bddebe44f81b18be2a6d216a5c031a502ea6a6

```java
 @PostMapping("/login")
 public String login(@Validated @ModelAttribute LoginForm form, BindingResult bindingResult,
                         HttpServletResponse httpServletResponse){

    // 로그인 성공 시 해당 멤버애 대한 쿠키 생성
    Cookie cookie=new Cookie("memberId",String.valueOf(loginMember.getId()));
    httpServletResponse.addCookie(cookie);
    // 생략
}
```
로그인 검증에 성공하면 해당 회원에 대한 쿠키를 생성해 ```HttpServletResponse``` 에 담아 전달한다.<br>
쿠키에 시간 정보를 생략했기 때문에 **브라우저 종료 시 로그아웃되는 세션** 쿠기이다.<br>

## @CookieValue 적용하기

```java
public String homeLogin(@CookieValue(name = "memberId", required = false)Long memberId, Model model) {
    
    // 로그인하지 않은 사용자 접근
    if(memberId == null) return "home"; 

    // 로그인을 시도했지만 해당하는 회원이 없는 경우
    Member loginMember = memberRepository.findById(memberId);
    if(loginMember == null) return "home";
    
    // 로그인 성공시 해당 회원정보를 출력하기 위해 Model에 담는다.
    model.addAttribute("member", loginMember);
    return "loginHome";
}
```
@CookieValue annotation 으로 쿠키를 편리하게 조회한다.<br>
로그인하지 않은 사용자도 홈 화면에 접근하기 때문에 ```required = false``` 로 설정한다.<br>

## 세션 쿠키와 지속 쿠키

쿠키는 세션 쿠키와 지속 쿠키로 나눠지는데 기준은 **만료 기간**이다.

- 영속 쿠키 : 만료 날짜를 입력하면 해당 날짜까지 유지
- 세션 쿠키 : 만료 날짜를 생략하면 브라우저를 종료하기 전까지만 유지

![png](/Web/_img/use_of_cookie.png)

광고창 밑에 **오늘 그만 보기** 버튼을 누르면 '해당 사용자는 오늘까지 광고를 노출하지 않는다.' 라는 데이터가 있기 때문에 하루 동안 해당 사이트에 다시 접근해도 광고창이 뜨지 않는 영속 쿠키이다. 이렇게 사용자의 상태를 파악하고 있는 데이터가 쿠키이며 **text 형식으로 클라이언트에 저장**된다.<br>

### setMaxAge 설정

> commit: https://github.com/evelyn82ny/session/commit/1181940a77710bee96a065e511991a585413af8e

```java
private void expireCookie(HttpServletResponse httpServletResponse, String cookieName) {
    Cookie cookie = new Cookie(cookieName, null);
    cookie.setMaxAge(0);
    httpServletResponse.addCookie(cookie);
}
```
cookie 의 MaxAge 를 ```0``` 으로 설정하면 쿠키가 즉시 종료된다.

![png](/Web/_img/not_apply_uuid_id.png)

member2 가 로그인하면 웹 브라우저에 멤버 아이디가 노출된다. 개인 정보가 노출되는 것도 문제지만 더 큰 문제는 value 에 ```3``` 을 입력하면 아래와 같이 아이디가 ```3``` 인 멤버로 로그인된 상태로 바뀐다.<br>

![png](/Web/_img/not_apply_uuid_id2.png)

쿠키 정보는 웹 브라우저에 보관되고 네트워크 요청마다 계속해서 클라이언트와 서버 사이에서 전달된다.<br>
지금은 아이디만 노출되지만 주민번호나 신용카드 같은 중요한 정보가 계속해서 노출된다면 해커는 분명 해당 정보를 훔쳐갈 것이다. 이러한 문제를 해결하기 위해 Session 개념을 적용한다.

<br>

# Session

일정 시간 동안 브라우저로부터 들어오는 일련의 요구를 **하나의 상태**로 보고 그 상태를 일정하게 유지하는 기술을 Session 이라고 한다. 다시 말해 사용자가 웹 브라우저를 통해 웹 서버에 접속한 시점부터 웹 브라우저를 종료하기 전까지의 사용자의 요구사항을 처리해주면서 상태를 유지시키는 기술이 세션이다.<br>

은행 업무같이 보안이 중요한 경우 웹 브라우저를 닫지 않아도 다시 로그인을 요구하는 경우가 종종있다. 이는 세션 만료 기간을 지정해 지정시간 간격으로 로그인을 요구해 보안을 강화하려는 목적이다.<br>

**웹 브라우저를 종료하면 세션이 삭제**되므로 쿠키보다 상대적으로 보안이 좋다.<br>

## UUID 로 session-id 생성

> commit: https://github.com/evelyn82/session/commit/c1a403f251615b0cc6d325c887c33c401d85fc3f

클라이언트와 서버사이에서 중요한 정보를 직접 전달하는 대신 **클라이언트를 식별할 수 있는 session-id** 만 주고 받도록 설정해봤다. 식별자인 session-id 는 해석이 불가능하도록 UUID(universally unique identifier)로 설정했으며, UUID 사용시 ```34fa0045-32b8-4a85..```같이 해석 불가능한 상태인 값을 받게 된다.

```java
private Map<String, Object> sessionStore = new ConcurrentHashMap<>();

public void createSession(Object value, HttpServletResponse httpServletResponse){
    String sessionId = UUID.randomUUID().toString();
    sessionStore.put(sessionId, value);

    Cookie cookie = new Cookie(SESSION_COOKIE_NAME, sessionId);
    httpServletResponse.addCookie(cookie);
}
```

```java
public Object getSession(HttpServletRequest httpServletRequest){
    Cookie sessionCookie = findCookie(httpServletRequest, SESSION_COOKIE_NAME);
    if(sessionCookie == null){
        return null;
    }
    return sessionStore.get(sessionCookie.getValue());
}
```
클라이언트가 요청을 보냈을 때 HttpServletRequest 에 전달된 쿠키를 찾아 요청을 처리한다.

### controller 에 session 적용

> commit: https://github.com/evelyn82/session/commit/caf20d58690e1408c35ad2e9b013e33faa8de509

![png](/Web/_img/apply_random_uuid.png)

controller 에 Session 을 적용하면 로그인 시 Value 에 회원 정보가 아닌 식별자만 노출된다.

<br>

## 세션 타임아웃 설정

```java
public String logout(HttpServletRequest httpServletRequest){
    
    HttpSession session = httpServletRequest.getSession(false);
    if(session != null){
        session.invalidate();
    }
    return "redirect:/";
}
```
로그아웃 시 세션을 삭제하기 위해 ```session.invalidate()``` 를 호출한다.<br>

지난날들을 생각해보면 로그아웃 버튼을 누르기보단 웹 브라우저 자체를 종료해 버린 경우가 더 많았다. 웹 브라우저 자체를 종료하면 알아서 로그아웃될 것이라 믿고 있었기 때문이다.<br>
하지만 웹 브라우저 자체를 종료한다고 해서 로그아웃이 절대 보장되지 않는다. 또한 웹 사이트마다 세션 종료 방식이 다른데 그 이유는 세션 타임아웃을 설정할 수 있기 때문이다.<br>

**HTTP 는 비연결성이므로 서버 입장에서 해당 사용자가 웹 브라우저의 종료를 인식할 수 없다.**<br>
즉, 서버는 판단이 어렵기 때문에 세션 데이터를 마음대로 삭제할 수 없어 개발자가 아래와 같이 application.properties 에 세션 종료 기준을 잡아준다.<br>

```properties
server.servlet.session.timeout=1800  // 초 단위
```

HttpSession 이 제공하는 timeout 설정 방식은 사용자가 서버에 **최근 요청한 시간을 기준**으로 설정한 시간만큼 유지해준다. 세션 생성 시점을 기준으로 종료된다면 해당 웹 사이트에 오래 머무를 경우 사용자는 계속해서 로그인을 해야 되는 번거로움이 생긴다.<br>
그럼 timeout 시간을 길게 설정하면 되지 않나? 라고 생각하겠지만 이는 메모리 사용이 누적되기 때문에 적당한 시간을 선택하는 것이 합리적이다.<br>

Session 의 LastAccessedTime 최근 세션 접근 시간을 기준으로 timeout 시간이 지나면 WAS 가 내부에서 해당 세션을 제거한다.<br>


## servlet 에서 제공하는 HttpSession

> SessionManager 구현 commit: https://github.com/evelyn82ny/session/commit/c1a403f251615b0cc6d325c887c33c401d85fc3f
> <br>HttpSession 구현 commit: https://github.com/evelyn82ny/session/commit/a70c8455119e5c124cc7a363cb3c885eb76f7df7

위에서 직접 구현해 본 SessionManager 의 모든 기능들을 서블릿에서 ```HttpSession``` 으로 제공한다.<br>
서블릿으로 HttpSession 을 생성하면 JSESSIONID 쿠키를 생성하고 UUID 와 같은 추청 불가능한 랜덤 값을 session-id로 설정한다.<br>

![png](/Web/_img/http_session_provided_by_servlet.png)

```java
public String homeLogin(HttpServletRequest httpServletRequest, Model model){
    HttpSession session=httpServletRequest.getSession(true/false);
}
```
세션을 생성하기 위해 getSession 을 사용하는데 옵션이 있다.

- true: 세션이 있으면 기존 세션을 반환하고 없으면 새로운 세션을 생성해 반환한다.(default)
- false: 세션이 있으면 기존 세션을 반환하고 없으면 null 을 반환한다. (즉, 새로운 세션을 생성하지 않는다.)

이미 로그인한 사용자는 세션 저장소에 해당 객체가 저장되어 있기 때문에 객체가 다른 작업으로 인해 정보를 요청한다면 새로 생성하지 않고 기존에 있던 세션을 반환한다.<br>
**로그인도 하지 않은 사용자에 대해 로그아웃을 시도할 땐 의미 없는 요청이므로 세션을 생성할 필요가 없기 때문에 false 로 설정**해 메모리 낭비를 막는다.

<br>

## Spring 에서 제공하는 @SessionAttribute

Spring 도 Session 을 편리하게 사용할 수 있게 `````@SessionAttribute````` 를 지원한다.<br>
@SessionAttribute 도 HttpSession 과 동일하게 쿠키이름이 JSESSIONID 이고 추정 불가능한 랜덤 값을 session-id로 설정한다. 또한, 세션 호출도 알아서 해주므로 직접 작성할 필요없다.<br>

> commit: https://github.com/evelyn82ny/session/commit/4bf7aa5bd473edf2bec257958b25dc305ef818c1

```java
public String homeLogin(
        @SessionAttribute(name = SessionConst.LOGIN_MEMBER, required = false) Member loginMember, 
         Model model) {
    
    // 세션 호출은 @SessionAttribute가 알아서 처리해주니 호출할 필요없다.
    if (loginMember == null) {
        return "home";
    }
}
```
@SessionAttribute required = false 로 설정하면 세션이 생성되지 않는다.

<br>

# 쿠키와 세션의 차이

쿠키와 세션의 가장 중요한 차이점은 저장위치와 **라이프 사이클이 다르다**는 것이다.<br>

![png](/Web/_img/diff_between_cookie_and_session.png)

### 라이프 사이클

- 쿠키의 만료 시간을 정하면 브라우저를 종료해도 쿠키가 삭제되지 않는 지속 쿠키가 있다.
- 세션은 사용자가 웹 브라우저를 종료하지 않아도 만료 기간을 설정해 삭제 가능하고 브라우저 종료시 세션이 삭제된다.

### 저장 위치

- 쿠키는 사용자의 정보를 사용자 로컬 PC에 저장하고
- 세션은 사용자의 요청 정보를 웹 서버가 session-id 를 만들어 서버에 저장하는 차이점이 있다.

### 속도
- 쿠키는 정보가 저장되어 있기 때문에 바로 참조가 가능해 속도가 빠르다.
- 세션은 session-id 로 서버에서 세션을 찾아야 하므로 상대적으로 속도가 느리다.

### 보안

- 로컬에 파일 형식으로 저장되는 쿠키는 클라이언트와 서버사이에 HTTP 요청/응답 과정에서 HTTP 메시지에 쿠키를 넣어 전달하기 때문에 털릴 가능성이 높아 보안에 취약하다.
- 서버 측에 저장되는 세션은 쿠키보다 상대적으로 안전하다.
- 하지만 세션은 서버에 저장되기 때문에 모든 사용자에 대한 세션을 저장하면 서버 측에서 소모되는 자원이 많아진다.
- 서버 자원의 낭비를 방지하면서 웹 사이트의 속도를 높이기 위해 쿠키와 세션을 함께 사용한다.