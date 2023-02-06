## AbstractGatewayFilterFactory로 GlobalFilter 생성

```java
import lombok.Data;
import lombok.extern.slf4j.Slf4j;
import org.springframework.cloud.gateway.filter.GatewayFilter;
import org.springframework.cloud.gateway.filter.factory.AbstractGatewayFilterFactory;
import org.springframework.http.server.reactive.ServerHttpRequest;
import org.springframework.http.server.reactive.ServerHttpResponse;
import org.springframework.stereotype.Component;
import reactor.core.publisher.Mono;

@Component
@Slf4j
public class GlobalFilter extends AbstractGatewayFilterFactory<GlobalFilter.Config> {

    public GlobalFilter() {
        super(Config.class);
    }

    @Override
    public GatewayFilter apply(Config config) {
        return (((exchange, chain) -> {
            ServerHttpRequest request = exchange.getRequest();
            ServerHttpResponse response = exchange.getResponse();

            log.info("Global Filter baseMessage: {}", config.getBaseMessage());
            if(config.isPreLogger()) {
                log.info("Global Filter Start: request id -> {}", request.getId());
            }

            return chain.filter(exchange).then(Mono.fromRunnable(() -> {
                if(config.isPostLogger()) {
                    log.info("Global Filter End: response code -> {}", response.getStatusCode());
                }
            }));
        }));
    }

    @Data
    public static class Config {
        private String baseMessage;
        private boolean preLogger;
        private boolean postLogger;
    }
}
```

모든 라우터에 적용되는 GlobalFilter를 생성하고, pre와 post 시점에 로그를 출력하기 위해 메시지를 출력하는 baseMessage와 상태를 나타내는 preLogger, postLogger를 갖는 Config를 생성한다. 

<br>

```yml
# application.yml
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
        - id: second-service
          uri: http://localhost:8082/
          predicates:
            - Path=/second-service/**
```

모든 라우터에 적용하기 위해 ```spring:cloud:gateway:default-filters```에 filter 정보를 작성한다. Config의 값들도 작성한다.

- ```first-service```라는 라우터에는 GlobalFilter와 CustomFilter가 적용되고
- ```second-service```라는 라우터에는 GlobalFilter만 적용된다.


## GlobalFilter와 CustomFilter 순사

GlobalFilter와 CustomFilter가 적용된다면 무조건 GlobalFilter가 가정 먼저 실행되고, CustomFilter가 모두 종료된 후에 GlobalFilter가 종료된다. 

```yml
INFO 10189 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter baseMessage: Spring Cloud Gateway GlobalFilter
INFO 10189 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter Start: request id -> 9b9da915-2
INFO 10189 --- [ctor-http-nio-2] c.n.a.filter.CustomFilter                : Custom PRE Filter: request uri -> 9b9da915-2
INFO 10189 --- [ctor-http-nio-2] c.n.a.filter.CustomFilter                : Custom POST Filter: response code -> 200 OK
INFO 10189 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter End: response code -> 200 OK
```

```first-service```라는 라우터에는 GlobalFilter와 CustomFilter가 적용되었는데, 위 로그를 보면 GlobalFilter가 가장 먼저 실행되고, 마지막에 종료되는 것을 확인할 수 있다.

CustomFilter를 적용하지 않았다면 다음과 같다.

<br>

```yml
INFO 10189 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter baseMessage: Spring Cloud Gateway GlobalFilter
INFO 10189 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter Start: request id -> 9b9da915-1
INFO 10189 --- [ctor-http-nio-2] c.n.a.filter.GlobalFilter                : Global Filter End: response code -> 200 OK
```

```second-service```라는 라우터에는 CustomFilter가 적용되지 않으므로 GlobalFilter만 적용된다.