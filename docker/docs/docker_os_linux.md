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

<br>

## Linux

**C group** 과 **namespaces** 은 Linux 기능이다. 하지만 MAC OS 에서 돌려도 잘돌아간다. 

![png](/docker/img/docker_os.png)

터미널에 ```docker version``` 을 작성해 OS를 확인하니 linux 였다. 즉, MAC OS에서 Docker를 사용하면 다음과 같은 구조가 된다.

![png](/docker/img/docker_os_linux.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)