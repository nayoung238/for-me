# Computer science

🖥 [Operating system](#operating-system)<br>
⚡️ [Network](#network)<br>
💌 [Server](#server)<br>
🛠 [Design Pattern](#design-pattern)<br>
📈 [Database](#database)<br>
⌨️ [Language](#language)<br>
🗄 [Docker](#docker)<br>

<br>

## Operating system

- [OS란](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/what_is_OS.md)
- [Process](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/process.md)
- [process state](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/process_state.md)
- [Kernel mode & Interrupt](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/interrupt.md)

### Memory

- [Memory](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/memory.md)
- [virtual address space](https://velog.io/@evelyn82ny/virtual-address-space)
- [Memory management](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/memory_management.md)
- [Page Placement Policy](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/page_placement_policy.md)
- [Paging](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/paging.md)
- [Page table 구조](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/page_table_structure.md)
- [Segmentation](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/segmentation.md)
- [참고: iOS와 Android 차이](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/difference_between_iOS_and_Android.md)
  
### Scheduling

- [FIFO, SJF, RR, HRRN](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/scheduling.md)
- [MLFQ](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/MLFQ.md)

### Concurrency & Synchronization

- [Thread](https://velog.io/@evelyn82ny/concurrency-thread)
- [Lock](https://velog.io/@evelyn82ny/synchronization-lock)
- [Semaphore](https://velog.io/@evelyn82ny/synchronization-semaphore)

> Reference
> - [operating systems: three easy pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)

<br>

## Network

- [OSI 7계층](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/OSI-7-layer.md)
- [unicast, broadcast, multicast, anycast](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/unicast_BUM_traffic.md)
- [MAC address](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/MAC-address.md)
- [IP address](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/IP-address.md)
- [TCP / IP protocol](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/TCP-IP-protocol.md)
- [IPv4 기반의 주소표현 sockaddr_in 구조체](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/sockaddr.md)
- [IP 주소 할당하기](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/allocate-ip.md)
<br>

- [127.0.0.1](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/localhost.md)
- [4-way handshake & half close](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/4-way-handshake.md)
- [TCP 와 UDP](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/TCP-VS-UDP.md)
- [TCP / IP protocol](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/TCP-IP-protocol.md)
- [TCP의 데이터 경계](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/boundary-of-tcp-transmission-data.md)
- [TCP 함수호출 과정](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/TCP-system-call.md)
- [UDP 는 연결이라는 개념이 존재하지 않는다.](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/UDP-system-call.md)
<br>

- [socket, bind, listen, accept, connect](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/socket.md)
- [file descriptor, open, close, read, write](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/file-descriptor.md)
<br>

- [Iterative server & Echo client](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/iterative-server-and-echo-client.md)
- [DNS - gethostbyname()으로 서버 정보 요청하기](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/dns.md)
<br>

- [blocking vs non-blocking](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/blocking-vs-non-blocking.md)
- [Nagle algorithm](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/nagle-algorithm.md)
<br>

- [자식 프로세스의 종료를 체크하는 wait, signal](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/zombie-check.md)
- [multiprocess](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/multiprocess.md)
- [multiplexing](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/multiplexing.md)
- [pipe](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/pipe.md)

> Reference
> - [TCP에 우아한 종료란 없다?](https://sunyzero.tistory.com/269)
> - [ssize_t](https://lacti.github.io/2011/01/08/different-between-size-t-ssize-t/)

<br>

## Server

### web

- [쿠키와 세션](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/cookie_and_session.md)
- [request mapping](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/request_mapping.md)
- [view resolver](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/view_resolver.md)
- [Redirect 와 Forward 차이](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/redirect_forward.md)
<br>

- [HTTP method](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/http_method.md)
- [httpServletRequest](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/httpServletRequest.md)
- [httpServletResponse](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/httpServletResponse.md)
- [http request: query](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/http_request_query.md)
- [http request: JSON](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/http_request_json.md)

### Spring: SQL

- [PK mapping](https://velog.io/@evelyn82ny/primary-key-mapping)

### Spring: Proxy

- [ProxyFactory](https://github.com/evelyn82ny/Computer-science/blob/master/Server/proxy/ProxyFactory.md)
- [BeanPostProcessor 기본 원리](https://github.com/evelyn82ny/Computer-science/blob/master/Server/proxy/BeanPostProcessor_basic.md)
- [BeanPostProcessor - Pointcut 적용 원리](https://github.com/evelyn82ny/Computer-science/blob/master/Server/proxy/BeanPostProcessor_pointcut.md)
- [@Aspect](https://velog.io/@evelyn82ny/Spring-AOP-Aspect)

<br>

## Design Pattern

- [Singleton pattern](https://github.com/evelyn82ny/Computer-science/blob/master/Server/pattern/singleton_pattern.md)
- [MVC pattern](https://github.com/evelyn82ny/Computer-science/blob/master/Server/pattern/spring_mvc.md)
- [Template method pattern](https://velog.io/@evelyn82ny/template-method-pattern)
- [Strategy pattern](https://velog.io/@evelyn82ny/strategy-pattern)
- [Template callback pattern](https://velog.io/@evelyn82ny/template-callback-pattern)
- [Proxy & Decorator pattern](https://velog.io/@evelyn82ny/proxy-pattern-decorator-pattern)

<br>

## Database

- [Join](https://github.com/evelyn82ny/Computer-science/blob/master/Database/Join.md)
- [ACID](https://hanamon.kr/%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4-%ED%8A%B8%EB%9E%9C%EC%9E%AD%EC%85%98%EC%9D%98-acid-%EC%84%B1%EC%A7%88/)

<br>

## Language

### Java

- [Inheritance](https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/Inheritance.md)
- [Abstract class](https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/AbstractClass.md)
- [Data type](https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/DataType.md)
- [CallByValue](https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/CallByValue.md)

> Reference
> - [Garbage Collection](https://d2.naver.com/helloworld/1329)

<br>

## Docker

- [Docker가 뭔데?](https://github.com/evelyn82ny/Computer-science/blob/master/Docker/what_is_docker.md)
- [Container는 뭔데?](https://github.com/evelyn82ny/Computer-science/blob/master/Docker/what_is_container.md)
- [Container 생성](https://github.com/evelyn82ny/Computer-science/blob/master/Docker/create_container.md)
- [Docker's OS: Linux](https://github.com/evelyn82ny/Computer-science/blob/master/Docker/docker_os_linux.md)
- [Docker's life cycle](https://github.com/evelyn82ny/Computer-science/blob/master/Docker/docker_life_cycle.md)
- [Docker Image 생성](https://github.com/evelyn82ny/Computer-science/blob/master/Docker/create_docker_image.md)