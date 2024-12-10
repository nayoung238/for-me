# for-developer

- 🖥 [Operating system](#operating-system)
- ⚡️ [Network](#network)
- 📚 [Database](#database)
- ⌨️ [Language](#language)
- 💌 [Server](#server)
- 🌱 [Spring](#spring)
- 🌱 [Spring Cloud](#spring-cloud)
- 🛠 [Design Pattern](#design-pattern)
- 🐳 [Docker](#docker)

<br>

## Operating system

- [OS란 (추상화 & 가상화)](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/operating_system.md)
- [Process](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/process.md)
- [process state](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/process_state.md)
- [Kernel mode & Interrupt](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/interrupt.md)

### Memory

- [Memory Structure](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/memory_structure.md)
- [virtual address space](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/virtual_address_space.md)
- [Memory management](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/memory_management.md)
- [Page Replacement Policy](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/page_replacement_policy.md)
- [Paging](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/paging.md)
- [Page table 구조](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/page_table_structure.md)
- [Segmentation](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/segmentation.md)
- Ref [iOS와 Android 차이](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/difference_between_iOS_and_Android.md)

### Scheduling

- [FIFO, SJF, RR, HRRN](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/scheduling.md)
- [MLFQ](https://github.com/nayoung8142/for-developer/blob/master/Operating-system/multi_level_feedback_queue.md)

### Concurrency & Synchronization

- [Thread](https://github.com/nayoung8142/for-developer/blob/master/operating-system/docs/thread.md)
- [ULT & KLT](https://github.com/nayoung8142/for-developer/blob/master/operating-system/docs/ult_and_klt.md)
- [Lock](https://github.com/nayoung8142/for-developer/blob/master/operating-system/docs/lock.md)
- Test [Semaphore](https://github.com/nayoung8142/for-developer/blob/master/operating-system/docs/semaphore.md)
- Test [생산자 & 소비자 문제 구현](https://github.com/nayoung8142/for-developer/blob/master/operating-system/docs/producer_consumer_problem.md)
<br>

- Ref [operating systems: three easy pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)

<br>

## Network

- [OSI 7계층](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/OSI-7-layer.md)
- [Encapsulation & Decapsulation](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/encapsulation.md)
- [unicast, broadcast, multicast, anycast](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/unicast_BUM_traffic.md)
- [MAC address](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/MAC-address.md)
- [IP address](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/IP-address.md)
- [TCP / IP protocol](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/TCP-IP-protocol.md)
- [IPv4 기반의 주소표현 sockaddr_in 구조체](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/sockaddr.md)
- [IP 주소 할당하기](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/allocate-ip.md)
<br>

- [4-way handshake & half close](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/4-way-handshake.md)
- [TCP 와 UDP](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/TCP-VS-UDP.md)
- [TCP / IP protocol](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/TCP-IP-protocol.md)
- Test [TCP의 데이터 경계](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/boundary-of-tcp-transmission-data.md)
- [TCP 함수호출 과정](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/TCP-system-call.md)
- Test [UDP 는 연결이라는 개념이 존재하지 않는다.](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/UDP-system-call.md)
<br>

- [socket, bind, listen, accept, connect](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/socket.md)
- [file descriptor, open, close, read, write](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/file-descriptor.md)
<br>

- Test [Iterative server & Echo client](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/iterative-server-and-echo-client.md)
- Test [DNS - gethostbyname()으로 서버 정보 요청하기](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/dns.md)
<br>

- [blocking vs non-blocking](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/blocking-vs-non-blocking.md)
- [Nagle algorithm](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/nagle-algorithm.md)
<br>

- [자식 프로세스의 종료를 체크하는 wait, signal](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/zombie-check.md)
- Test [multiprocess](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/multiprocess.md)
- Test [multiplexing](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/multiplexing.md)
- Test [pipe](https://github.com/nayoung8142/for-developer/blob/master/Network/theory/pipe.md)
<br>

- Ref [TCP에 우아한 종료란 없다?](https://sunyzero.tistory.com/269)
- Ref [ssize_t](https://lacti.github.io/2011/01/08/different-between-size-t-ssize-t/)

<br>

## Database

- [Clustered Index & Nonclustered Index](https://github.com/imzero238/for-interview/blob/master/database/clustered_index_and_nonclustered_index.md)
- [Non-unique Index & Next Key Lock](https://medium.com/@im_zero/non-unique-index%EC%99%80-next-key-lock-b314379d9b0f)
- [MySQL InnoDB 락 테스트(트랜잭션 격리 수준별 비교)](https://medium.com/@im_zero/innodb-%EB%9D%BD-%ED%85%8C%EC%8A%A4%ED%8A%B8-ab712d19b74f)
- [cursor-based-pagination](https://medium.com/@im_zero/infinite-scroll-encountered-two-children-with-the-same-key-%EC%97%90%EB%9F%AC-48c62db262e1)

<br>

## Language

### Java

- [Inheritance](https://github.com/nayoung8142/for-developer/blob/master/Language/Java/Inheritance.md)
- [Abstract class](https://github.com/nayoung8142/for-developer/blob/master/Language/Java/AbstractClass.md)
- [Data type](https://github.com/nayoung8142/for-developer/blob/master/Language/Java/DataType.md)
- [CallByValue](https://github.com/nayoung8142/for-developer/blob/master/Language/Java/CallByValue.md)
- [SOLID](https://github.com/nayoung8142/for-developer/blob/master/Language/Java/SOLID.md)
- Ref [Garbage Collection](https://d2.naver.com/helloworld/1329)

<br>

## Server

- [HandlerMapping, HandlerAdapter](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/handler_mapping_and_adapter.md)
- [HTTP method](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/http_method.md)
- [httpServletRequest](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/httpServletRequest.md)
- [httpServletResponse](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/httpServletResponse.md)
- [http request: query](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/http_request_query.md)
- [http request: JSON](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/http_request_json.md)
<br>

- [view resolver](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/view_resolver.md)
- [Redirect 와 Forward 차이](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/redirect_forward.md)
- [쿠키와 세션](https://github.com/nayoung8142/for-developer/blob/master/Server/Web/cookie_and_session.md)

<br>

## Spring

- [PK mapping](https://github.com/nayoung8142/for-developer/blob/master/Spring/PK_mapping.md)
- Ref [Filter, Interceptor, AOP](https://popo015.tistory.com/116?category=902728)

### Proxy

- [ProxyFactory](https://github.com/nayoung8142/for-developer/blob/master/Spring/proxy/ProxyFactory.md)
- [BeanPostProcessor 기본 원리](https://github.com/nayoung8142/for-developer/blob/master/Spring/proxy/BeanPostProcessor_basic.md)
- [BeanPostProcessor - Pointcut 적용 원리](https://github.com/nayoung8142/for-developer/blob/master/Spring/proxy/BeanPostProcessor_pointcut.md)

### JPA

- [JPA와 Spring Data JPA의 차이 (feat. Hibernate)](https://velog.io/@evelyn82ny/JPA-vs-Spring-Data-JPA)
- [JPA Persistence Context 1차 캐시의 식별 방법 (feat.OSIV)](https://velog.io/@evelyn82ny/how-to-identify-a-first-level-cache)

<br>

## Spring Cloud

- [Spring CLoud Docs](https://spring.io/projects/spring-cloud)
- [Random Port](https://github.com/nayoung8142/for-developer/blob/master/Spring-Cloud/random_port.md)
- Filter
  - [Filter using Java Code (RouteLocator)](https://github.com/nayoung8142/for-developer/blob/master/Spring-Cloud/filter_using_java_code.md)
  - [Filter using Property (application.yml)](https://github.com/nayoung8142/for-developer/blob/master/Spring-Cloud/filter_using_property.md)
- [Custom Filter Using AbstractGatewayFilterFactory](https://github.com/nayoung8142/for-developer/blob/master/Spring-Cloud/custom_filter.md)
- [Global Filter Using AbstractGatewayFilterFactory](https://github.com/nayoung8142/for-developer/blob/master/Spring-Cloud/global_filter.md)
- [Spring Cloud Gateway (Load Balancer)](https://github.com/nayoung8142/for-developer/blob/master/Spring-Cloud/spring-cloud-gateway.md)
<br>

## Design Pattern

- [Singleton pattern](https://github.com/nayoung8142/for-developer/blob/master/Server/pattern/singleton_pattern.md)
- [MVC pattern](https://github.com/nayoung8142/for-developer/blob/master/Server/pattern/spring_mvc.md)
- Ref [MVC Docs](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-servlet)
- [PRG pattern](https://github.com/nayoung8142/for-developer/blob/master/design-pattern/post_redirect_get_pattern.md)
- [Template method pattern](https://github.com/nayoung8142/for-developer/blob/master/design-pattern/template_method_pattern.md)
- [Strategy pattern](https://github.com/nayoung8142/for-developer/blob/master/design-pattern/strategy_pattern.md)
- [Template callback pattern](https://github.com/nayoung8142/for-developer/blob/master/design-pattern/template_callback_pattern.md)
- [Proxy & Decorator pattern](https://github.com/nayoung8142/for-developer/blob/master/design-pattern/proxy_and_decorator_pattern.md)
- [JDK Dynamic proxy](https://github.com/nayoung8142/for-developer/blob/master/design-pattern/JDK_dynamic_proxy.md)
- [CGLIB](https://github.com/nayoung8142/for-developer/blob/master/design-pattern/CGLIB.md)

<br>

## Docker

- [Docker란](https://github.com/nayoung8142/for-developer/blob/master/Docker/what_is_docker.md)
- [Container란](https://github.com/nayoung8142/for-developer/blob/master/Docker/what_is_container.md)
- [Container 생성](https://github.com/nayoung8142/for-developer/blob/master/Docker/create_container.md)
- [Docker's OS: Linux](https://github.com/nayoung8142/for-developer/blob/master/Docker/docker_os_linux.md)
- [Docker's life cycle](https://github.com/nayoung8142/for-developer/blob/master/Docker/docker_life_cycle.md)
- [Docker Image 생성](https://github.com/nayoung8142/for-developer/blob/master/Docker/create_docker_image.md)
