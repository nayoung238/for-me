# Docker Image 생성

Docker Hub에 있는 Image를 가져와 사용해도 되고 직접 Image를 만들어 사용하거나 Docker Hub에 올려 공유할 수 있다.

<br>

## Docker Image & File

![png](/_docker/_img/docker_image_creation_process.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- Image: Container를 만들기 위해 필요한 설정이나 종속성들을 갖고 Software package
- File: Docker Image를 만들기 위한 설정 파일 (Container가 어떻게 행동해야 하는지에 대한 설정을 정의)
- 결국 Docker file로 Docker Image를 만듦
<br>

### Base Image

![png](/_docker/_img/base_image.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

Docker File을 만드는 첫번째 단계는 Base Image를 명시하는 것이다.

- Docker Image는 여러 Layer로 구성
- 그중에서 Base Image는 이 Image의 기반이 되는 부분
- Layer는 중간 단계의 이미지

<br>

## Docker Image 만들기

### Docker File 생성

```dockerfile
FROM alpine
RUN command
CMD ["echo", "hello"]
```

### FROM

- Base Image 명시
- 즉, Image 생성 시 기반이 되는 Image layer
- <Image 이름>:<tag> 형식으로 작성 -> ubuntu:18.04
- tag 안붙이면 가장 최신것으로 설정

### RUN

- 추가적으로 필요한 파일들을 다운로드 (File snapshot)
- Docker Image가 생성되기 전에 수행할 Shell 명령어

### CMD

- Container 시작 시 실행될 명령어 명시
- Container가 시작되었을 때 실행할 파일 또는 Shell script
- 해당 명령어는 Docker File 내에 1회만 쓸 수 있음

<br>

```dockerfile
FROM alpine
CMD ["echo", "hello"]
```

- ```hello``` 를 출력하는 기능은 ubuntu, centos와 같이 큰 Base image가 필요없기 때문에 작은 Alpine Base Image 사용
- ```hello``` 를 출력하기 위해 ```echo```를 사용해야 하는데 Alpine 안에 echo를 사용하게 할 수 있는 파일이 있기 때문에 **RUN** 생략
- Container 시작 시 실행될 명령어 ```echo hello```를 작성

<br>

## Docker server 가 인식하도록 설정

- 생성한 Docker file이 Docker client를 거쳐 Docker Server에 전달되어 인식해야함
- ```docker build ./``` 또는 ```docker build .``` 를 작성하면 되는데 ```./```를 권장
  - ```./``` 와 ```.``` 는 현재 Directory를 가리킴

![png](/_docker/_img/create_docker_image.png)

- 여러 과정을 거쳐 Docker Server가 인식하면 맨 마지막 줄에 해당 Image에 대한 Id 값을 넘겨줌
- 넘겨준 ID 앞부분을 Copy하여 ```docker run ID앞부분(일부)``` 를 작성하면 ```hello``` 가 출력
- ```docker run ID앞부분(일부)``` 를 작성하면 해당 Image를 토대로 Container가 만들어지며 CMD에 작성한 명령어가 실행되어 ```hello``` 가 출력 

<br>

![png](/_docker/_img/build_previous_versions_result.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- 다른 자료를 보니깐 이전에는 build하면 위와 같이 출력된 것 같다라고 추측...
- 처리되는 과정은 똑같을 것
- Step 2를 보면 임시 Container를 생성하고 다시 지움

![png](/_docker/_img/build_process(1).png)

![png](/_docker/_img/build_process(2).png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- 일단 Base Image인 Alpine Image를 Docker Hub에서 가져옴 (Id: a24bb4013296)
- 그리고 ```CMD ["echo", "hello"]``` 를 처리하기 위해 임시 Container (Id: fa6c364fdc39)를 생성
- 임시 Container의 Hard disk에 File snapshot 추가 및 시작 시 실행할 명령어 추가
- 설정된 임시 Container를 토대로 새로운 Image가 만들어짐 (Image Id: aff4e9afc538)
- 그리고 임시 Container 삭제
<br>

![png](/_docker/_img/docker_image_creation_process(2).png)

Base Image에 다른 종속성이나 새로운 명령어를 추가하면 임시 Container를 만든 후 해당 Container를 토대로 새로운 Image를 생성한다.

<br>

## Docker Image에 이름 설정

![png](/_docker/_img/set_docker_image_name.png)

새로 생성된 Docker Image의 ID를 기억하기 쉽게 이름을 설정할 수 있다.

- ```docker build -t (자신의 Docker Id)/(저장소, 프로젝트 이름):(버전) ./``` 으로 작성
- ```docker run -it (자신의 Docker Id)/(저장소, 프로젝트 이름)``` 작성 시 성공적으로 출력됨