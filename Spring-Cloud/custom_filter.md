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

<br>

## GlobalFilter와 CustomFilter 간 순서 제어

default는 당연히 GlobalFilter가 먼저 시작되고, 모든 CustomFilter가 종료된 후에 GlobalFilter가 종료된다. 만약 CustomFilter를 먼저 처리하고 싶다면 아래와 같이 순서를 적용하면 된다.

### Ordered.HIGHEST_PRECEDENCE

가장 높은 우선순위로 설정하려면 ```Ordered.HIGHEST_PRECEDENCE``` 를 추가한다.

```java
import lombok.Data;
import lombok.extern.slf4j.Slf4j;
import org.springframework.cloud.gateway.filter.GatewayFilter;
import org.springframework.cloud.gateway.filter.OrderedGatewayFilter;
import org.springframework.cloud.gateway.filter.factory.AbstractGatewayFilterFactory;
import org.springframework.core.Ordered;
import org.springframework.http.server.reactive.ServerHttpRequest;
import org.springframework.http.server.reactive.ServerHttpResponse;
import org.springframework.stereotype.Component;
import reactor.core.publisher.Mono;

@Component
@Slf4j
public class Custom2Filter extends AbstractGatewayFilterFactory<Custom2Filter.Config> {

    public Custom2Filter() {
        super(Config.class);
    }

    @Override
    public GatewayFilter apply(Config config) {

        return new OrderedGatewayFilter((exchange, chain) -> {
            ServerHttpRequest request = exchange.getRequest();
            ServerHttpResponse response = exchange.getResponse();

            log.info("Custom2 Filter baseMessage: {}", config.getBaseMessage());
            if(config.isPreLogger()) {
                log.info("Custom2 Filter Start: request id -> {}", request.getId());
            }

            return chain.filter(exchange).then(Mono.fromRunnable(() -> {
                if(config.isPostLogger()) {
                    log.info("Custom2 Filter End: response code -> {}", response.getStatusCode());
                }
            }));
        }, Ordered.HIGHEST_PRECEDENCE);
    }

    @Data
    public static class Config {
      private String baseMessage;
      private boolean preLogger;
      private boolean postLogger;
    }
}
```
CustomFilter에 ```Ordered.HIGHEST_PRECEDENCE```를 작성하면 제일 높은 우선순위가 된다.

```yml
server:
  port: 8080

spring:
  application:
    name: gateway-service
  cloud:
    gateway:
      default-filters:
        - name: GlobalFilter
          args:
            baseMessage: Spring Cloud Gateway GlobalFilter
            preLogger: true
            postLogger: true
      routes:
        - id: first-service
          uri: http://localhost:8081/
          predicates:
            - Path=/first-service/**
          filters:
            - CustomFilter
            - name: Custom2Filter
              args:
                baseMessage: Spring Cloud Gateway Custom2Filter
                preLogger: true
                postLogger: true
            
```

```application.yml```에 우선순위가 가장 높은 Custom2Filter를 ```spring:cloud:gateway:routes:filters```에 추가한다. ```first-service``` 라우터는 1개의 GlobalFilter와 2개의 CustomFilter, Custom2Filter가 적용되며 결과는 다음과 같다.

```yml
INFO 10771 --- [ctor-http-nio-2] c.n.a.filter.Custom2Filter               : Custom2 Filter baseMessage: Spring Cloud Gateway Custom2Filter
INFO 10771 --- [ctor-http-nio-2] c.n.a.filter.Custom2Filter               : Custom2 Filter Start: request id -> f5860c92-1

INFO 10771 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter baseMessage: Spring Cloud Gateway GlobalFilter
INFO 10771 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter Start: request id -> f5860c92-1

INFO 10771 --- [ctor-http-nio-2] c.n.a.filter.CustomFilter                : Custom PRE Filter: request uri -> f5860c92-1
INFO 10771 --- [ctor-http-nio-2] c.n.a.filter.CustomFilter                : Custom POST Filter: response code -> 200 OK

INFO 10771 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter End: response code -> 200 OK

INFO 10771 --- [ctor-http-nio-2] c.n.a.filter.Custom2Filter               : Custom2 Filter End: response code -> 200 OK
```

CustomFilter인 Custom2Filter에 ```Ordered.HIGHEST_PRECEDENCE```를 설정했기 때문에 GlobalFilter보다 먼저 시작하고, 모든 CustomFilter와 GlobalFilter가 종료된 다음에 종료되는 것을 확인할 수 있다.
<br>

### Ordered.LOWEST_PRECEDENCE

반대로 가장 낮은 우선순위로 설정하려면 ```rdered.LOWEST_PRECEDENCE``` 추가한다.

```java
@Override
public GatewayFilter apply(Config config) {

    return new OrderedGatewayFilter((exchange, chain) -> {
        ServerHttpRequest request = exchange.getRequest();
        ServerHttpResponse response = exchange.getResponse();

    log.info("Custom2 Filter baseMessage: {}", config.getBaseMessage());
    if(config.isPreLogger()) {
        log.info("Custom2 Filter Start: request id -> {}", request.getId());
    }

    return chain.filter(exchange).then(Mono.fromRunnable(() -> {
        if(config.isPostLogger()) {
            log.info("Custom2 Filter End: response code -> {}", response.getStatusCode());
        }
    }));
    }, Ordered.LOWEST_PRECEDENCE);
}
```

```yml
INFO 10821 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter baseMessage: Spring Cloud Gateway GlobalFilter
INFO 10821 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter Start: request id -> 2340dfdb-1

INFO 10821 --- [ctor-http-nio-2] c.n.a.filter.CustomFilter                : Custom PRE Filter: request uri -> 2340dfdb-1

INFO 10821 --- [ctor-http-nio-2] c.n.a.filter.Custom2Filter               : Custom2 Filter baseMessage: Spring Cloud Gateway Custom2Filter
INFO 10821 --- [ctor-http-nio-2] c.n.a.filter.Custom2Filter               : Custom2 Filter Start: request id -> 2340dfdb-1
INFO 10821 --- [ctor-http-nio-2] c.n.a.filter.Custom2Filter               : Custom2 Filter End: response code -> 200 OK

INFO 10821 --- [ctor-http-nio-2] c.n.a.filter.CustomFilter                : Custom POST Filter: response code -> 200 OK
INFO 10821 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter End: response code -> 200 OK
```