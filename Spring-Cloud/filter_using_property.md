## Filter using Property

```yml
server:
  port: 8080

spring:
  application:
    name: gateway-service
  cloud:
    gateway:
      routes:
        - id: first-service
          uri: http://localhost:8081/
          predicates:
            - Path=/first-service/**
          filters:
            - AddRequestHeader=first-request, first-request-header-test
            - AddResponseHeader=first-response, first-response-header-test
```

Gateway의 ```Application.yml```에 ```spring:cloud:gateway:routers:filters```를 추가하면 자바 코드를 작성할 필요없다.
<br>

```java
@RestController
@RequestMapping("/first-service")
@Slf4j
public class FirstServiceController {

    @GetMapping("/message")
    public String message(@RequestHeader("first-request") String header) {
        log.info(header);
        return "Hello world in First service";
    }
}
```
first-service의 message API 접근하기 위해 ```http://localhost:8080/first-service/message``` 작성하면 RequestHeader와 ResponseHeader의 결과는 다음과 같다. 

### RequestHeader 결과

```text
INFO 8092 --- [nio-8081-exec-2] c.n.f.web.FirstServiceController         : first-request-header-test
```

### ResponseHeader 결과

![](/_img/Spring-Cloud/filter_using_property_response_header.png)

<br>

## Microservice 추가

```yml
spring:
  application:
    name: gateway-service
  cloud:
    gateway:
      routes:
        - id: first-service
          uri: http://localhost:8081/
          predicates:
            - Path=/first-service/**
          filters:
            - AddRequestHeader=first-request, first-request-header-test
            - AddResponseHeader=first-response, first-response-header-test
        - id: second-service
          uri: http://localhost:8082/
          predicates:
            - Path=/second-service/**
          filters:
            - AddRequestHeader=second-request, second-request-header-test
            - AddResponseHeader=second-response, second-response-header-test
```

새로운 Microservice에 대해 Filter를 수행하고 싶다면 위와 같이 작성하면 된다.