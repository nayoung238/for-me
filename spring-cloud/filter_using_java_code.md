## Filter using Java Code

```java
import org.springframework.cloud.gateway.route.RouteLocator;
import org.springframework.cloud.gateway.route.builder.RouteLocatorBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class FilterConfig {

    @Bean
    public RouteLocator gatewayRoutes(RouteLocatorBuilder builder) {
        return builder.routes()
                .route(r -> r.path("/first-service/**")
                        .filters(f -> f.addRequestHeader("first-request", "first-request-header")
                                .addResponseHeader("first-response", "first-response-header"))
                        .uri("http://localhost:8081/"))
                .build();
    }
}
```

- PredicateSpec.path: client가 작성하는 경로
- UriSpec.uri: 해당 uri을 사용하는 Microservice로 이동
- GatewayFilterSpec.addRequestHeader: RequestHeader에 추가 (headerName, headerValue)
- GatewayFilterSpec.addResponseHeader: ResponseHeader에 추가 (headerName, headerValue)
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
filter가 제대로 작동하는지 확인하기 위해 ```log.info``` 를 작성한다. ```http://localhost:8080/first-service/message``` 작성 시 RequestHeader와 ResponseHeader 결과는 다음과 같이 제대로 추가된 것을 확인할 수 있다.


### RequestHeader 결과

```text
INFO 7634 --- [nio-8081-exec-1] c.n.f.web.FirstServiceController         : first-request-header
```

### ResponseHeader 결과

![png](/_img/Spring-Cloud/filter_using_java_code_response_header.png)

<br>

## Microservice 추가

위 Config는 1개의 Service에 대해 Filter를 수행한다. 만약 다른 Microservice에 대한 filter를 추가한다면 다음과 같이 작성한다.

```java
import org.springframework.cloud.gateway.route.RouteLocator;
import org.springframework.cloud.gateway.route.builder.RouteLocatorBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class FilterConfig {

    @Bean
    public RouteLocator gatewayRoutes(RouteLocatorBuilder builder) {
        return builder.routes()
                .route(r -> r.path("/first-service/**")
                        .filters(f -> f.addRequestHeader("first-request", "first-request-header")
                                .addResponseHeader("first-response", "first-response-header"))
                        .uri("http://localhost:8081/"))
                .route(r -> r.path("/second-service/**")
                        .filters(f -> f.addRequestHeader("second-request", "second-request-header")
                                .addResponseHeader("second-response", "second-response-header"))
                        .uri("http://localhost:8082/"))
                .build();
    }
}
```
