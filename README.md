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

- ⌨️ Project: [MSA with Spring Cloud](#project-msa-with-spring-cloud)

<br>

## Operating system

- [OS란 (추상화 & 가상화)](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/what_is_OS.md)
- [Process](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/process.md)
- [process state](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/process_state.md)
- [Kernel mode & Interrupt](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/interrupt.md)

### Memory

- [Memory Structure](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/memory_structure.md)
- [virtual address space](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/virtual_address_space.md)
- [Memory management](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/memory_management.md)
- [Page Replacement Policy](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/page_replacement_policy.md)
- [Paging](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/paging.md)
- [Page table 구조](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/page_table_structure.md)
- [Segmentation](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/segmentation.md)
- [참고: iOS와 Android 차이](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/difference_between_iOS_and_Android.md)
  
### Scheduling

- [FIFO, SJF, RR, HRRN](https://github.com/evelyn82ny/for-developere/blob/master/Operating-system/scheduling.md)
- [MLFQ](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/MLFQ.md)

### Concurrency & Synchronization

- [Thread](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/thread.md)
- [ULT & KLT](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/ULT_and_KLT.md)
- [Lock](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/lock.md)
- [Semaphore](https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/semaphore.md) (생산자 & 소비자 문제 구현)

> Reference
> - [operating systems: three easy pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)

<br>

## Network

- [OSI 7계층](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/OSI-7-layer.md)
- [Encapsulation & Decapsulation](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/encapsulation.md)
- [unicast, broadcast, multicast, anycast](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/unicast_BUM_traffic.md)
- [MAC address](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/MAC-address.md)
- [IP address](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/IP-address.md)
- [TCP / IP protocol](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/TCP-IP-protocol.md)
- [IPv4 기반의 주소표현 sockaddr_in 구조체](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/sockaddr.md)
- [IP 주소 할당하기](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/allocate-ip.md)
<br>

- [127.0.0.1](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/localhost.md)
- [4-way handshake & half close](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/4-way-handshake.md)
- [TCP 와 UDP](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/TCP-VS-UDP.md)
- [TCP / IP protocol](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/TCP-IP-protocol.md)
- [TCP의 데이터 경계](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/boundary-of-tcp-transmission-data.md)
- [TCP 함수호출 과정](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/TCP-system-call.md)
- [UDP 는 연결이라는 개념이 존재하지 않는다.](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/UDP-system-call.md)
- Test [TCP와 UDP 기반의 server,client 구현](https://velog.io/@evelyn82ny/TCP-and-UDP-based-on-server-and-client)
<br>

- [socket, bind, listen, accept, connect](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/socket.md)
- [file descriptor, open, close, read, write](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/file-descriptor.md)
<br>

- [Iterative server & Echo client](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/iterative-server-and-echo-client.md)
- [DNS - gethostbyname()으로 서버 정보 요청하기](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/dns.md)
<br>

- [blocking vs non-blocking](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/blocking-vs-non-blocking.md)
- [Nagle algorithm](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/nagle-algorithm.md)
<br>

- [자식 프로세스의 종료를 체크하는 wait, signal](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/zombie-check.md)
- [multiprocess](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/multiprocess.md)
- [multiplexing](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/multiplexing.md)
- [pipe](https://github.com/evelyn82ny/for-developer/blob/master/Network/theory/pipe.md)

> Reference
> - [TCP에 우아한 종료란 없다?](https://sunyzero.tistory.com/269)
> - [ssize_t](https://lacti.github.io/2011/01/08/different-between-size-t-ssize-t/)

<br>

## Database

- [Join](https://github.com/evelyn82ny/for-developer/blob/master/Database/Join.md)
- [Transaction Isolation Levels](https://github.com/evelyn82ny/for-developer/blob/master/Database/transation_isolation_levels.md)
- [Pessimistic Lock, Optimistic Lock](https://github.com/evelyn82ny/for-developer/blob/master/Database/optimistic_lock_pessimistic_lock.md)
- Ref [MVCC](https://mangkyu.tistory.com/53)
- [Clustered Index & Nonclustered Index](https://github.com/evelyn82ny/for-developer/blob/master/Database/clustered_index_and_nonclustered_index.md)
- Test [Record Lock](https://github.com/evelyn82ny/for-developer/blob/master/Database/record_lock.md)
- Test [Next-key lock vs Insert-intention](https://github.com/evelyn82ny/for-developer/blob/master/Database/next_key_lock_and_insert_intention_test.md)
- Test [Next-key lock 테스트 1](https://github.com/evelyn82ny/for-developer/blob/master/Database/next_key_lock_test.md) : 이미 존재하는 레코드 수정, 중복 ID로 INSERT 시도, 존재하지 않는 ID에 UPDATE 시도
- Test [Next-key lock 테스트 2](https://github.com/evelyn82ny/for-developer/blob/master/Database/next_key_lock_between_test.md) : Between
- Test [Next-key lock 테스트 3](https://github.com/evelyn82ny/for-developer/blob/master/Database/composite_key_test.md) : Composite Key
<br>

- [cursor-based-pagination](https://velog.io/@evelyn82ny/cursor-based-pagination)

<br>

## Language

### Java

- [Inheritance](https://github.com/evelyn82ny/for-developer/blob/master/Language/Java/Inheritance.md)
- [Abstract class](https://github.com/evelyn82ny/for-developer/blob/master/Language/Java/AbstractClass.md)
- [Data type](https://github.com/evelyn82ny/for-developer/blob/master/Language/Java/DataType.md)
- [CallByValue](https://github.com/evelyn82ny/for-developer/blob/master/Language/Java/CallByValue.md)
- [SOLID](https://github.com/evelyn82ny/for-developer/blob/master/Language/Java/SOLID.md)
- Ref [Garbage Collection](https://d2.naver.com/helloworld/1329)

<br>

## Server

- [HandlerMapping, HandlerAdapter](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/handler_mapping_and_adapter.md)
- [HTTP method](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/http_method.md)
- [httpServletRequest](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/httpServletRequest.md)
- [httpServletResponse](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/httpServletResponse.md)
- [http request: query](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/http_request_query.md)
- [http request: JSON](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/http_request_json.md)
<br>

- [view resolver](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/view_resolver.md)
- [Redirect 와 Forward 차이](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/redirect_forward.md)
- [쿠키와 세션](https://github.com/evelyn82ny/for-developer/blob/master/Server/Web/cookie_and_session.md)

<br>

## Spring

- [PK mapping](https://github.com/evelyn82ny/for-developer/blob/master/Spring/PK_mapping.md)
- Test [데이터가 DB에 반영되는 시점 (feat. Persistence Context, Transaction, OSIV)](https://velog.io/@evelyn82ny/%EB%8D%B0%EC%9D%B4%ED%84%B0%EA%B0%80-DB%EC%97%90-%EB%B0%98%EC%98%81%EB%90%98%EB%8A%94-%EC%8B%9C%EC%A0%90-feat-Persistence-Context-Transaction)
- [@RestControllerAdvice](https://velog.io/@evelyn82ny/Exception-handling-using-RestControllerAdvice)
- Ref [Filter, Interceptor, AOP](https://popo015.tistory.com/116?category=902728)

### Proxy

- [ProxyFactory](https://github.com/evelyn82ny/for-developer/blob/master/Spring/proxy/ProxyFactory.md)
- [BeanPostProcessor 기본 원리](https://github.com/evelyn82ny/for-developer/blob/master/Spring/proxy/BeanPostProcessor_basic.md)
- [BeanPostProcessor - Pointcut 적용 원리](https://github.com/evelyn82ny/for-developer/blob/master/Spring/proxy/BeanPostProcessor_pointcut.md)
- [@Aspect](https://velog.io/@evelyn82ny/Spring-AOP-Aspect)

### JPA

- [JPA와 Spring Data JPA의 차이 (feat. Hibernate)](https://velog.io/@evelyn82ny/JPA-vs-Spring-Data-JPA)
- [JPA Persistence Context 1차 캐시의 식별 방법 (feat.OSIV)](https://velog.io/@evelyn82ny/how-to-identify-a-first-level-cache)
- Test [Pessimistic Lock 적용하기](https://velog.io/@evelyn82ny/when-to-use-a-pessimistic-lock-feat-2PL)

<br>

## Spring Cloud

- [Spring CLoud Docs](https://spring.io/projects/spring-cloud)
- [Random Port](https://github.com/evelyn82ny/for-developer/blob/master/Spring-Cloud/random_port.md)
- Filter
  - [Filter using Java Code (RouteLocator)](https://github.com/evelyn82ny/for-developer/blob/master/Spring-Cloud/filter_using_java_code.md)
  - [Filter using Property (application.yml)](https://github.com/evelyn82ny/for-developer/blob/master/Spring-Cloud/filter_using_property.md)
- [Custom Filter Using AbstractGatewayFilterFactory](https://github.com/evelyn82ny/for-developer/blob/master/Spring-Cloud/custom_filter.md)
- [Global Filter Using AbstractGatewayFilterFactory](https://github.com/evelyn82ny/for-developer/blob/master/Spring-Cloud/global_filter.md)
- [Spring Cloud Gateway (Load Balancer)](https://github.com/evelyn82ny/for-developer/blob/master/Spring-Cloud/spring-cloud-gateway.md)
<br>

### Project: MSA with Spring Cloud

- Repo: [api-gateway-service](https://github.com/evelyn82ny/api-gateway-service)
- Repo: [service-discovery](https://github.com/evelyn82ny/service-discovery)
- Repo: [item-service](https://github.com/evelyn82ny/MSA-item-service)
- Repo: [account-service](https://github.com/evelyn82ny/MSA-account-service)
- Repo: [order-service](https://github.com/evelyn82ny/MSA-order-service)

<br>

## Design Pattern

- [Singleton pattern](https://github.com/evelyn82ny/for-developer/blob/master/Server/pattern/singleton_pattern.md)
- [MVC pattern](https://github.com/evelyn82ny/for-developer/blob/master/Server/pattern/spring_mvc.md)
- Ref [MVC Docs](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#mvc-servlet)
- [PRG pattern](https://github.com/evelyn82ny/for-developer/blob/master/Design-pattern/PRG_pattern.md)
- [Template method pattern](https://github.com/evelyn82ny/for-developer/blob/master/Design-pattern/template_method_pattern.md)
- [Strategy pattern](https://github.com/evelyn82ny/for-developer/blob/master/Design-pattern/strategy_pattern.md)
- [Template callback pattern](https://github.com/evelyn82ny/for-developer/blob/master/Design-pattern/template_callback_pattern.md)
- [Proxy & Decorator pattern](https://github.com/evelyn82ny/for-developer/blob/master/Design-pattern/proxy_and_decorator_pattern.md)
- [JDK Dynamic proxy](https://github.com/evelyn82ny/for-developer/blob/master/Design-pattern/JDK_dynamic_proxy.md)
- [CGLIB](https://github.com/evelyn82ny/for-developer/blob/master/Design-pattern/CGLIB.md)

<br>

## Docker

- [Docker가 뭔데?](https://github.com/evelyn82ny/for-developer/blob/master/Docker/what_is_docker.md)
- [Container는 뭔데?](https://github.com/evelyn82ny/for-developer/blob/master/Docker/what_is_container.md)
- [Container 생성](https://github.com/evelyn82ny/for-developer/blob/master/Docker/create_container.md)
- [Docker's OS: Linux](https://github.com/evelyn82ny/for-developer/blob/master/Docker/docker_os_linux.md)
- [Docker's life cycle](https://github.com/evelyn82ny/for-developer/blob/master/Docker/docker_life_cycle.md)
- [Docker Image 생성](https://github.com/evelyn82ny/for-developer/blob/master/Docker/create_docker_image.md)
