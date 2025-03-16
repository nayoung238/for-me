# Docker

- Container를 사용하여 Application을 더 쉽게 만들고 배포 및 실행할 수 있도록 설계된 도구
- Container 기반의 오픈소스 가상화 플랫폼
<br>

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

![png](/docker/img/docker_image_container.png)

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

![png](/docker/img/create_container_result.png)

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

## Hypervisor

- 가상화 기술이 나오기 전에는 한대의 서버를 하나의 용도로만 사용했기 때문에 안정적이지만 사용하지 않는 부분은 그대로 방치되는 단점
- 논리적 공간으로 분할하려 VM이라는 독립적인 가상 환경의 서버가 만들어짐
- Hypervisor: Host 컴퓨터에서 다수의 OS(Guest)를 동시에 실행하기 위한 논리적 Platform이며 HW를 가상화하면서 HW와 각각의 VM을 모니터링하는 중간 관리자
- Virtual machine monitor 또는 Virtual machine manager 줄여서 VMM 라고도 부름

![png](/docker/img/two_types_of_hypervisors.png)

> 출처: 위키 백과

<br>

### Native Hypervisor (Bare-metal)

- Hypervisor가 Hardware에서 직접 실행
- Hypervisor가 Hardware를 직접 제어하기 때문에 자원을 효율적으로 사용 가능하고 별도의 Host OS가 없어 Overhead가 적음
- Guest OS는 Hardware 위에서 2번째 수준으로 실행
- 여러 Device driver를 세팅해야 하므로 설치가 어렵다는 단점

<br>

### Hosted Hypervisor

- 일반적인 Software처럼 Host OS 위에서 실행
- Hardware 자원을 VM 내부의 Guset OS에 Emulator 하는 방식으로 Overhead가 큼
- VM 내부에서 동작되는 Guset OS는 Hardware에서 3번쨰 수준으로 실행
- Guest OS 종류에 대한 제약이 없고 구현이 쉬움
- VM의 대표적인 종류는 VMware Server, VMware Workstation, VirtualBox 등이 있고 일반적으로 많이 사용되는 방식

<br>

## Docker 구조

![png](/docker/img/hypervisor_based_VM_structure.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- Hypervisor에 의해 구동되는 VM은 각 VM마다 독립된 가상 Hardware 지원을 할당받음
- 논리적으로 분리되어 있어서 특정 VM의 오류가 다른 VM에게 영향을 미치지 않음
<br>

![png](/docker/img/docker_structure.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- Docker Container와 VM은 Hardware에서 격리된 환경에 Application을 배치
<br>

- Docker의 경우 Hypervisor와 Guest OS가 필요하지 않아 가벼움
- Docker의 경우 Application을 실행하기 위해 Host OS 위에 Application의 실행 패키지인 Image를 배포하기만 하면 됨
- VM의 경우 Application을 실행하기 위해 VM을 띄우고 자원을 할당한 다음 Guest OS를 부팅해야 하므로 훨씬 복잡함
<br>

- Docker container에서 돌아가는 Application은 같은 Host의 다른 Container와 동일한 Kernel을 공유
- 즉, Container 내부에서 실행되는 Process는 Host 시스템에서 볼 수 있음
- Docker를 통해 MongoDB Container를 시작하면 Host의 일반 Shell에 ```ps -ef grep mongo```를 실행하면 프로세스가 표시됨

<br>

## C group & namespaces

![png](/docker/img/container_isolation.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

Linux Kernel 기능인 **C group** 과 **namespaces** 로 Container와 Host에서 실행되는 다른 프로세스 사이의 벽을 만듦

- C group (Control group)
    - CPU, Memory, Network Bandwith, HD I/O 등 Process 그룹의 시스템 리소스 사용량을 관리
    - 특정 Application의 사용량이 많다면 C group에 집어넣어 CPU와 Memory 사용 제한 가능
- namespaces
    - 하나의 시스템에서 프로세스를 격리시킬 수 있는 가상화 기술
    - 별개의 독립된 공간을 사용하는 것처럼 격리된 환경을 제공하는 경량 프로세스 가상화 기술