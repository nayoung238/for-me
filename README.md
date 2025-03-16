# for-developer

- 🖥 [Operating system](#operating-system)
- ⚡️ [Network](#network)
- 📚 [Database](#database)
- ⌨️ [Language](#language)
- 🌱 [Spring](#spring)
- 🛠 [Design Pattern](#design-pattern)
- 🐳 [Docker](#docker)

<br>

## Operating system

- [OS](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/operating_system.md)
- [프로세스 & 스레드](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/process_and_thread.md)
- [process state](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/process_state.md)
- [Kernel mode & Interrupt](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/interrupt.md)

### Memory

- [Memory Structure](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/memory_structure.md)
- [virtual address space](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/virtual_address_space.md)
- [Memory management](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/memory_management.md)
- [Page Replacement Policy](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/page_replacement_policy.md)
- [Paging](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/paging.md)
- [Page table 구조](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/page_table_structure.md)
- [Segmentation](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/segmentation.md)

### Scheduling

- [FIFO, SJF, RR, HRRN](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/scheduling.md)
- [MLFQ](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/multi_level_feedback_queue.md)

### Concurrency & Synchronization

- [Thread](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/thread.md)
- [ULT & KLT](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/ult_and_klt.md)
- [Lock](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/lock.md)
- Test [Semaphore](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/semaphore.md)
- Test [생산자 & 소비자 문제 구현](https://github.com/nayoung238/for-me/blob/main/operating-system/docs/producer_consumer_problem.md)
<br>

- Ref [operating systems: three easy pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)

<br>

## Network

- [OSI 7계층](https://github.com/nayoung238/for-me/blob/main/network/docs/OSI-7-layer.md)
- [Encapsulation & Decapsulation](https://github.com/nayoung238/for-me/blob/main/network/docs/encapsulation.md)
- [unicast, broadcast, multicast, anycast](https://github.com/nayoung238/for-me/blob/main/network/docs/unicast_BUM_traffic.md)
- [MAC address](https://github.com/nayoung238/for-me/blob/main/network/docs/MAC-address.md)
- [IP address](https://github.com/nayoung238/for-me/blob/main/network/docs/IP-address.md)
- [TCP / IP protocol](https://github.com/nayoung238/for-me/blob/main/network/docs/TCP-IP-protocol.md)
- [IPv4 기반의 주소표현 sockaddr_in 구조체](https://github.com/nayoung238/for-me/blob/main/network/docs/sockaddr.md)
- [IP 주소 할당하기](https://github.com/nayoung238/for-me/blob/main/network/docs/allocate-ip.md)
<br>

- [4-way handshake & half close](https://github.com/nayoung238/for-me/blob/main/network/docs/4-way-handshake.md)
- [TCP 와 UDP](https://github.com/nayoung238/for-me/blob/main/network/docs/TCP-VS-UDP.md)
- [TCP / IP protocol](https://github.com/nayoung238/for-me/blob/main/network/docs/TCP-IP-protocol.md)
- Test [TCP의 데이터 경계](https://github.com/nayoung238/for-me/blob/main/network/docs/boundary-of-tcp-transmission-data.md)
- [TCP 함수호출 과정](https://github.com/nayoung238/for-me/blob/main/network/docs/TCP-system-call.md)
- Test [UDP 는 연결이라는 개념이 존재하지 않음](https://github.com/nayoung238/for-me/blob/main/network/docs/UDP-system-call.md)
<br>

- [socket, bind, listen, accept, connect](https://github.com/nayoung238/for-me/blob/main/network/docs/socket.md)
- [file descriptor, open, close, read, write](https://github.com/nayoung238/for-me/blob/main/network/docs/file-descriptor.md)
<br>

- Test [Iterative server & Echo client](https://github.com/nayoung238/for-me/blob/main/network/docs/iterative-server-and-echo-client.md)
- Test [DNS - gethostbyname()으로 서버 정보 요청하기](https://github.com/nayoung238/for-me/blob/main/network/docs/dns.md)
<br>

- [blocking vs non-blocking](https://github.com/nayoung238/for-me/blob/main/network/docs/blocking-vs-non-blocking.md)
- [Nagle algorithm](https://github.com/nayoung238/for-me/blob/main/network/docs/nagle-algorithm.md)
<br>

- [자식 프로세스의 종료를 체크하는 wait, signal](https://github.com/nayoung238/for-me/blob/main/network/docs/zombie-check.md)
- Test [multiprocessing](https://github.com/nayoung238/for-me/blob/main/network/docs/multiprocess.md)
- Test [multiplexing](https://github.com/nayoung238/for-me/blob/main/network/docs/multiplexing.md)
- Test [pipe](https://github.com/nayoung238/for-me/blob/main/network/docs/pipe.md)

<br>

## Database

- [ACID & 트랜잭션 격리 수준](https://github.com/nayoung238/for-me/blob/main/database/docs/acid_transaction_isolation_level.md)
- [Clustered Index & Non-clustered Index](https://github.com/nayoung238/for-me/blob/main/database/docs/clustered_index_and_nonclustered_index.md)
- [MySQL 복제](https://github.com/nayoung238/for-me/blob/main/database/docs/replication.md)
<br>

- [Lost Update, Write Skew and Phantom](https://medium.com/@nayoung238/lost-update-write-skew-and-phantom-test-aa31887fdfb2) 이상 현상 테스트

<br>

## Language

### Java

- [Inheritance](https://github.com/nayoung238/for-me/blob/main/java/docs/Inheritance.md)
- [Abstract class](https://github.com/nayoung238/for-me/blob/main/java/docs/AbstractClass.md)
- [Data type](https://github.com/nayoung238/for-me/blob/main/java/docs/DataType.md)
- [CallByValue](https://github.com/nayoung238/for-me/blob/main/java/docs/CallByValue.md)
- [SOLID](https://github.com/nayoung238/for-me/blob/main/java/docs/SOLID.md)

<br>

## Spring

- [DispatcherServlet](https://github.com/nayoung238/for-me/blob/main/spring/docs/dispatcher_servlet.md)
- [HandlerMapping, HandlerAdapter](https://github.com/nayoung238/for-me/blob/main/spring/docs/handler_mapping_and_adapter.md)
- [httpServletRequest](https://github.com/nayoung238/for-me/blob/main/spring/docs/http_servlet_request.md)
- [httpServletResponse](https://github.com/nayoung238/for-me/blob/main/spring/docs/http_servlet_response.md)
- [http request: query](https://github.com/nayoung238/for-me/blob/main/spring/docs/http_request_query.md)
- [http request: JSON](https://github.com/nayoung238/for-me/blob/main/spring/docs/http_request_json.md)

<br>

- [view resolver](https://github.com/nayoung238/for-me/blob/main/spring/docs/view_resolver.md)
- [Redirect 와 Forward 차이](https://github.com/nayoung238/for-me/blob/main/spring/docs/redirect_forward.md)
- [쿠키와 세션](https://github.com/nayoung238/for-me/blob/main/spring/docs/cookie_and_session.md)

<br>

- [PK mapping](https://github.com/nayoung238/for-me/blob/main/spring/docs/pk_mapping.md)
- [AOP - aspect](https://github.com/nayoung238/for-me/blob/main/spring/docs/aop_aspect.md)
- [ProxyFactory](https://github.com/nayoung238/for-me/blob/main/spring/docs/proxy_factory.md)
- [BeanPostProcessor](https://github.com/nayoung238/for-me/blob/main/spring/docs/bean_post_processor.md)
- [BeanPostProcessor - Pointcut](https://github.com/nayoung238/for-me/blob/main/spring/docs/bean_post_processor_pointcut.md)

### JPA

- [JPA vs Spring Data JPA vs Hibernate](https://medium.com/@nayoung238/jpa-vs-spring-data-jpa-vs-hibernate-5657f21f2e18)
- [JPA Persistence Context 1차 캐시의 식별 방법 (feat.OSIV)](https://medium.com/@nayoung238/jpa-persistence-context%EC%97%90%EC%84%9C-entity-%EC%8B%9D%EB%B3%84-%EB%B0%A9%EB%B2%95-feat-osiv-d6b85bb53713)
r>

### Spring Cloud

- [Random Port](https://github.com/nayoung238/for-me/blob/main/spring/docs/random_port.md)
- [Custom Filter Using AbstractGatewayFilterFactory](https://github.com/nayoung238/for-me/blob/main/spring/docs/custom_filter.md)
- [Global Filter Using AbstractGatewayFilterFactory](https://github.com/nayoung238/for-me/blob/main/spring/docs/global_filter.md)

<br>

## Design Pattern

- [Singleton pattern](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/singleton_pattern.md)
- [MVC pattern](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/spring_mvc.md)
- [PRG pattern](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/post_redirect_get_pattern.md)
- [Template method pattern](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/template_method_pattern.md)
- [Strategy pattern](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/strategy_pattern.md)
- [Template callback pattern](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/template_callback_pattern.md)
- [Proxy & Decorator pattern](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/proxy_and_decorator_pattern.md)
- [JDK Dynamic proxy](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/JDK_dynamic_proxy.md)
- [CGLIB](https://github.com/nayoung238/for-me/blob/main/design-pattern/docs/CGLIB.md)

<br>

## Docker

- [Docker](https://github.com/nayoung238/for-me/blob/main/docker/docs/what_is_docker.md)
- [Container](https://github.com/nayoung238/for-me/blob/main/docker/docs/what_is_container.md)
- [Container 생성](https://github.com/nayoung238/for-me/blob/main/docker/docs/create_container.md)
- [Docker's OS: Linux](https://github.com/nayoung238/for-me/blob/main/docker/docs/docker_os_linux.md)
- [Docker's life cycle](https://github.com/nayoung238/for-me/blob/main/docker/docs/docker_life_cycle.md)
- [Docker Image 생성](https://github.com/nayoung238/for-me/blob/main/docker/docs/create_docker_image.md)
