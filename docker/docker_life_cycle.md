## 생명 주기

![png](/_docker/_img/docker_life_cycle.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

## Running

- ```docker run 이미지이름``` 을 싱행하면 Container가 생성 및 실행됨
- 즉, ```docker create 이미지이름``` 과 ```docker start 이미지이름``` 이 둘다 처리됨

![png](/_docker/_img/docker_create.png)

![png](/_docker/_img/docker_start.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

## Stopped

![png](/_docker/_img/docker_stop_and_kill.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

실행중인 Container를 중지하는 방법엔 ```stop``` 과 ```kill``` 2가지가 있다.
둘다 Container를 중지시키지만 차이가 있다.

- Stop: 그동안 하던 작업들을 완료하고 Container 중지
- Kill: 바로 Container 중지

<br>

## Deleted

- docker rm (삭제할 Containerd의 ID 또는 이름)
- docker system prune : 사용하지 않는 Container, Image 그리고 Network 모두 삭제
    - 실행중인 Container에 영향을 주지 않음