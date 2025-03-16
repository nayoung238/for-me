## Random Port 설정

같은 Microservice를 여러 인스턴스로 동작하게 한다면 포트 번호를 모두 다르게 설정해야 한다. 
인스턴스가 증가할수록 포트 번호를 관리하기 번거로우니 개발자가 직접 관리하지 않고 Random Port를 사용한다.

일단 개발자가 직접 관리하는 방법을 알아보자.
<br>

## Configurations 수정

![](/spring/img/copy_configuration.png)

같은 서비스를 추가 실행하기 위해 ```Run/Debug Configuration```에서 Configuration을 복사한다.
그 후 2개의 인스턴스를 실행하면 1개의 인스턴스에서 에러가 발생하는데, 2개의 인스턴스가 같은 포트번호를 사용하기 때문이다.

그러므로 ```Environment/VM options```에 ```-Dserver.port=9002``` 을 추가해주면 해당 인스턴스는 9002번 포트를 사용하게 되므로 에러가 발생하지 않는다.

<br>

## -Dserver.port 사용

```bash
% mvn spring-boot:run -Dspring-boot.run.jvmArguments='-Dserver.port=9003'
```
위와 같이 작성하면 9003번 포트를 사용하는 인스턴스가 생성된다.
<br>

```bash
% mvn clean
% mvn compile package
```
기존에 만든 Build 정보를 모두 지우고, 컴파일한 소스를 packaging 양식인 war 또는 jar에 맞춰 프로젝트 내 지정한 경로(eg.target) 디렉토리에 생성한다.

```bash
% java -jar -Dserver.port=9004 ./target/user-service-0.0.1-SNAPSHOT.jar
```
위와 같이 작성하면 9004번 포트를 사용하는 인스턴스가 생성된다. 하지만 위와 같은 방식들은 인스턴스끼리 포트 번호가 중복되지 않게 관리해야 한다는 번거로움이 있다. 이는 Random Port로 해결할 수 있다.

<br>

## 포트 번호 0 설정

```yml
server: port: 0
```
포트 번호를 0으로 설정하면 Random Port를 사용하겠다는 의미이다.
즉, 개발자가 인스턴스마다 포트 번호의 중복을 확인할 필요가 없어 관리하기 쉽다.

하지만 막상 여러 인스턴스를 실행하면 Spring Eureka 페이지에선 Instance 정보가 1개만 나온다.
이를 해결하기 위해 아래의 내용을 추가한다.

```yml
eureka:
  instance:
    instanceId: ${spring.cloud.client.hostname}:${spring.application.instance_id:${random.value}}
```

```application.yml```에 instanceId를 등록하면 각 인스턴스의 정보가 출력된다.