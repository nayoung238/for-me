## Image

- 코드, 런타임, 시스템 도구, 시스템 라이브러리 및 설정과 같은 응용 프로그램을 실행하는데 필요한 모든 것을 포함하는 독립적이며 실행 가능한 소프트웨어 패키지
- 즉, Application을 실행하는데 필요한 모든 것을 포함
   - Container가 시작될 때 실행되는 명령어
   - File Snapshot: 컴퓨터 파일과 Directory를 Copy한 것
<br>

![png](/docker/img/create_container.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

1. Docker-Cli 에 ```docker run 이미지``` 를 입력하면 Docker-server에 전달
2. Docker-server에 Container를 위한 Image가 이미 Cache 되어있는지 확인
3. Cache에 없다면 Docker-Hub에서 가져옴
4. Docker Image에 있는 File snapshot을 Container Hard Disk 에 옮김
5. Docker Image에 있는 명령어를 실행

<br>

## Alpine Image : ls commend

![png](/docker/img/alpine_ls_command.png)

Docker-Cli 에 ```docker run alpine ls``` 으로 작성하면 아래 과정이 이루어지고 현재 Directory의 파일 리스트가 출력된다.

<br>

![png](/docker/img/alpain_ls_command_process.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- Alpine Image에 대한 Container를 생성할 때 해당 Image에 있던 File snapshots(bin, dev, etc 등)이 Container 안에 있는 Hard disk로 다운됨
- Image 이름 뒤에 다른 명령어(ls)가 있기 때문에 기본 명령어는 무시되고 ls 명령어가 실행됨
<br>

## hello-world : ls commend

![png](/docker/img/hello_world_ls_command.png)

- Docker-Cli 에 ```docker run hello-world ls``` 를 입력했더니 ```exec: "ls": executable file not found``` 오류 출력
- Alpine Image에 대해 ls 명령어를 실행하면 오류가 발생하지 않음
- 이는 Alpine Image의 File snapshots에는 ls 명령어를 사용 가능하게 하는 파일이 존재해서 오류가 발생하지 않음

<br>

## docker ps

- ```docker ps``` 는 Container list를 반환해주는 명령어로 현재 가동중인 Container만 출력됨
- 여기서 ps 는 Process status

![png](/docker/img/alpine_ping.png)

- ```docker run alpine ping localhost```으로 alpine Image를 실행시키고
- ```docker ps``` 로 가동중인 Container를 확인
<br>

### CONTAINER ID

- Container의 고유 아이디 Hash 값
- 실제로는 더 길지만 일부분만 출력

### IMAGE

- Container 생성 시 사용한 Docker Image

### COMMAND

- Container 시작 시 실행될 명령어
- 대부분 Image에 내장되어 있으므로 별도 설정이 필요없음

### CREATED

- Container가 생성된 시간

### STATUS

- Container 상태
- Up : 실행중
- Exited : 종료
- Pause : 일시정지

### PORTS

- Container가 개방한 Port와 Host에 연결한 Port

### NAMES

- Container의 고유한 이름
- Container 생성 시 --name 옵션으로 이름을 설정하지 않으면 Docker Engine이 임의로 형용사와 명사를 조합해 설정
- CONTAINER ID 와 마찬가지로 중복 안됨
- docker rename 명령어로 이름 변경 가능
- docker rename original-name changed-name
<br>

## 원하는 항목만 보기

![png](/docker/img/docker_ps_format_command.png)

많은 항목중 ```NAMES``` 와 ```STATUS``` 만 보고싶다면 ```docker ps --format 'table{{.Names}}\t{{.Status}}'``` 로 작성한다.

<br>

## 모든 Container 나열

모든 Container를 보고 싶다면 ```docker ps -a``` 로 작성한다.

<br>

## 실행중인 Container에 명령어 전달

![png](/docker/img/docker_exec_ls_command.png)

이미 실행중인 Container에 명령어를 전달할 땐 ```exec``` 사용한다.

<br>

## sh command

![png](/docker/img/exec_it_sh.png)

실행중인 Container에 명령어를 전달할 때 ```docker exec -it 컨테이너아이디 명령어``` 으로 매번 작성하기 번거롭다면 ```sh``` 를 사용한다.<br>
```docker exec -it 컨테이너아이디 sh``` 로 작성하면 실행중인 Container안에서 터미널 환경을 구축한다.
