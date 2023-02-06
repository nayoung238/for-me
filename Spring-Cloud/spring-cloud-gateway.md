## Spring Cloud Gateway - Load Balancer

특정 Application을 여러 instance로 실행시키면 Load Balancer가 제대로 작동되는지 확인해봤다.

```yml
eureka:
  client:
    register-with-eureka: true
    fetch-registry: true
    service-url:
      defaultZone: http://localhost:8761/eureka
```

Gateway와 모든 Microservice의 ```application.yml```에 위 내용을 추가한다.

<br>

![](/_img/Spring-Cloud/instances_currently_registered_with_eureka.png)

```MY-FIRST-SERVICE``` 라는 Application을 2개 실행했으며, 랜덤 포트로 설정해 61302와 61307 포트 번호가 할당되었다.

<br>

### Port 번호 출력하기

```java
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.env.Environment;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;

@RestController
@RequestMapping("/first-service")
@Slf4j
public class FirstServiceController {

    Environment environment;

    @Autowired
    public FirstServiceController(Environment environment) {
        this.environment = environment;
    }

    @GetMapping("/port-check")
    public String portCheck(HttpServletRequest request) {
        log.info("Server Port = {}", request.getServerPort());
        return String.format("First service Port -> %s", environment.getProperty("local.server.port"));
    }
}
```

```MY-FIRST-SERVICE```라는 Application의 API를 호출했을 때 어떤 인스턴스가 실행되는지 확인하기 위해 위와 같이 작성했다. 그 결과 모든 인스턴스가 **Round-Robin 방식**으로 돌아가면서 실행되었다.

```yml
INFO 12303 --- [o-auto-1-exec-1] c.n.f.web.FirstServiceController         : Server Port = 61302
INFO 12303 --- [o-auto-1-exec-2] c.n.f.web.FirstServiceController         : Server Port = 61302
INFO 12303 --- [o-auto-1-exec-3] c.n.f.web.FirstServiceController         : Server Port = 61302
```

```yml
INFO 12318 --- [o-auto-1-exec-1] c.n.f.web.FirstServiceController         : Server Port = 61307
INFO 12318 --- [o-auto-1-exec-2] c.n.f.web.FirstServiceController         : Server Port = 61307
INFO 12318 --- [o-auto-1-exec-3] c.n.f.web.FirstServiceController         : Server Port = 61307
```