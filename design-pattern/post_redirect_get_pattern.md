## Post-Redirect-Get pattern

PRG(Post-Redirect-Get) 패턴은 웹 개발에 권장되는 디자인 패턴 중 하나로, 원리는 다음과 같다.

1. 클라이언트는 HTTP POST 요청을 한다.
2. 서버는 요청에 대한 응답으로 **지정한 URL 과 GET 방식으로 redirect 되는 302 상태**를 설정한다.
3. 자동 리다이렉션을 통해 서버가 설정한 URL에 접근하게 된다.

즉, HTTP POST 요청에 대한 응답을 **지정한 URL**과 **GET 요청**으로 설정하기 위한 패턴을 PRG(Post-Redirect-Get) 패턴이라고 한다.

<br>

## PRG 패턴 적용 전

POST 요청에 대해 PRG 패턴을 적용하지 않으면 발생되는 문제는 다음과 같다.

![png](/_img/design-pattern/before_applying_prg_pattern.png)

- 클라이언트에서 사과 2개를 주문하는 POST 요청을 서버에게 보냈다.
- 서버는 해당 데이터를 DB에 반영한 뒤, 잘 처리했다는 200 OK 상태를 응답으로 보낸다.
- 200 OK 응답을 받은 클라이언트의 화면에는 여전히 주문하는 페이지가 노출된다.

이 상태에서 url을 새로고침하면 이전에 주문했던 사과 2개를 다시 POST 요청되는 상황이 발생한다. 서버는 또다시 사과 2개 주문을 DB에 반영해 같은 클라이언트가 사과 4개를 주문한 것으로 처리된다.

이처럼 **새로고침이나 뒤로 가기를 했을 경우 이전에 보내진 POST 요청이 다시 요청되는 경우를 방지**하고자 PRG 패턴을 적용한다.

<br>

## PRG 패턴 적용 후

결제 이후 주문 결과 창이 노출되는 것은 PRG 패턴이 적용된 상태이기 때문이다.

![png](/_img/design-pattern/after_applying_prg_pattern.png)

주문 후 url 새로고침으로 인한 재주문을 막기 위해 POST 요청에 대한 응답에 http 302 상태와 Location 헤더에 응답에 대한 결과 창 url을 작성한다. **http 302 상태 코드는 redirect시 요청 메서드를 GET으로 변경**시키므로 Location 헤더에 작성된 url로 자동 리다이렉션 된다.

즉, 주문 요청에 대한 응답을 302 상태 코드와 Location 헤더에 주문에 대한 결과 창 url로 작성했다면, 주문 요청 시 응답에 대해 자동 라다이렉션 되어 주문 결과 창이 노출된다. 주문 결과 창을 노출하는 GET 방식이므로 url을 새로고침해도 재주문되는 문제를 막을 수 있다.

잘못된 요청을 서버 측에서 무조건 막도록 설정해야 하지만, PRG 패턴을 통해 클라이언트 측에서 잘못된 요청을 한 번 더 막아주면 사용성이 좋아지며 오류가 줄어든다.

<br>

### 작성 예시

```java
@PostMapping("/items/")
public String create(BookForm form){
    // 비즈니스 로직
    return "redirect:/items";
}
```