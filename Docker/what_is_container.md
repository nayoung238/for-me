## Container

- 실제 화물에서 사용되는 Container에 다양한 상품을 넣어 전송하는데 Server에서도 비슷한 개념으로 사용
- Container 안에 다양한 프로그램, 실행환경을 Container로 추상화하고 동일한 Interface를 제공해 프로그램의 배포 및 관리를 단순하게 함
<br>

- 코드와 모든 종속성을 패키지화하여 Application이 한 컴퓨팅 환경에서 다른 컴퓨팅 환경으로 빠르고 안정적으로 실행되도록 하는 Software의 표준 단위
- 쉽게 말해 간단하고 편리하게 프로그램을 실행시켜주는 것
- 리눅스와 윈도우 기반 Application 모두에서 사용할 수 있는 Containerized software는 Infra에 관계없이 항상 동일하게 실행됨
- Container는 Software를 환경으로부터 격리시키고 개발과 Staging 차이에도 불구하고 균일하게 작동하는 것을 보장
<br>

## Container image

![png](/Docker/_img/docker_image_container.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- 코드, 런타임, 시스템 도구, 시스템 라이브러리 및 설정과 같은 응용 프로그램을 실행하는데 필요한 모든 것을 포함하는 독립적이며 실행 가능한 소프트웨어 패키지
- Container image는 Runtime에 Container가 됨
- Docker Container의 경우 Docker Engine에서 실행될 때 Image가 Container가 됨
- Docker image를 이용해 Container를 생성하고 Docker container로 프로그램을 실행
<br>

- Docker-Cli 에 Commend 입력
- Docker-Server(Docker Daemon)가 입력된 Commend를 받아서 그것에 따라 Image 또는 Container를 실행하거나 다른 작업을 실행

터미널에 ```docker run hello-world```를 입력하면 ID가 ```a6db7d75181d``` 인 Container가 생성된다.

![png](/Docker/_img/create_container_result.png)

 1. The Docker client contacted the Docker daemon.
 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
    (arm64v8)
 3. The Docker daemon created a new container from that image which runs the
    executable that produces the output you are currently reading.
 4. The Docker daemon streamed that output to the Docker client, which sent it to your terminal.

<br>

### Cache 확인

만약 ```hello-world``` 이미지가 Cache에 없다면 ```Unable to find image 'hello-world:latest' locally```라는 메시지가 출력되고 Docker Hub에서 해당 이미지를 가져오고 Cache에 보관한다.

<br>

## Container 확인

![png](/Docker/_img/redis_cil.png)

Redis server를 실행하고 redis-cli에서 명령어를 실행해본다.<br>
다른 터미널에서 redis-cli 를 입력했지만 알 수 없는 명령어라고 뜬다.

![png](/Docker/_img/container(1).png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

```command not found: redis-cli``` 오류가 뜬 이유는 Redis-cli가 Redis-server가 있는 Container 밖에서 실행하기 때문에 Server에 접근할 수 없어 에러가 난다.
<br>

이를 해결하기 위해선 Redis-cli 를 Redis-server가 있는 Container 안에서 실행시켜야 한다.

![png](/Docker/_img/container(2).png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

```docker exec -it 컨테이너아이디 redis-cli``` 작성하면 해당 Container 안에서 redis-cli 가 실행되며 정상적으로 동작하는 것을 확인할 수 있다.
<br>

```-it``` 를 꼭 작성해야 한다.
interactive terminal 을 뜻하는데 ```-it``` 를 작성하지 않으면 redis-cli 를 켜고 바로 다시 나와서 의미가 없다.