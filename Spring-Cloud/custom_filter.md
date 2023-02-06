## AbstractGatewayFilterFactory로 CustomFilter 생성

```java
import lombok.extern.slf4j.Slf4j;
import org.springframework.cloud.gateway.filter.GatewayFilter;
import org.springframework.cloud.gateway.filter.factory.AbstractGatewayFilterFactory;
import org.springframework.http.server.reactive.ServerHttpRequest;
import org.springframework.http.server.reactive.ServerHttpResponse;
import org.springframework.stereotype.Component;
import reactor.core.publisher.Mono;

@Component
@Slf4j
public class CustomFilter extends AbstractGatewayFilterFactory<CustomFilter.Config> {

    public CustomFilter() {
        super(Config.class);
    }

    @Override
    public GatewayFilter apply(Config config) {
        // Custom Pre Filter. Suppose we can extract JWT and perform Authentication
        return ((exchange, chain) -> {
            ServerHttpRequest request = exchange.getRequest();
            ServerHttpResponse response = exchange.getResponse();

            log.info("Custom PRE Filter: request uri -> {}", request.getId());

            // Custom POST Filter. Suppose we can call error response handler based on error code.
            return chain.filter(exchange).then(Mono.fromRunnable(() -> {
                log.info("Custom POST Filter: response code -> {}", response.getStatusCode());
            }));
        });
    }

    public static class Config {
        // Put the configuration properties
    }
}
```

```AbstractGatewayFilterFactory```의 ```apply()```를 작성하면 CustomFilter를 생성할 수 있다. 위 코드는 Pre와 Post 시점에 간단한 로그를 출력하는 CustomFilter이다.
<br>

Netty를 사용하므로 비동기 처리를 수행하는 ```ServerHttpRequest```와 ```ServerHttpResponse```를 사용한다.

<br>

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
            - CustomFilter
```

특정 Microservice에 CustomFilter를 적용하기 위해 ```application.yml```에 ```spring:cloud:gateway:routes:filters```에 CustomFilter를 추가한다. 즉, Filter를 적용하고자 하는 라우터에 수동으로 추가해야 한다.

<br>

```java
@RestController
@RequestMapping("/first-service")
@Slf4j
public class FirstServiceController {

    @GetMapping("/custom-filter-check")
    public String customFilterCheck() {
        return "Hello World in First service";
    }
}
```

CustomFilter 적용 결과를 확인하기 위해 Microservice의 Controller에 위와 같은 API를 작성한 뒤 ```http://localhost:8080/first-sevice/custom-filter-check``` 를 입력한다.

<br>

```text
INFO 9002 --- [ctor-http-nio-2] c.n.a.filter.CustomFilter                : Custom PRE Filter: request uri -> 928e993c-3
INFO 9002 --- [ctor-http-nio-2] c.n.a.filter.CustomFilter                : Custom POST Filter: response code -> 200 OK
```
Gateway Service에서 위와 같은 로그 출력을 확인할 수 있다.