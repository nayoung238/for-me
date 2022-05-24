# Computer science

1. [Operating system](#operating-system)
2. [Network](#network)
   1. [Socket](#socket-programming)
3. [Server](#server)
   1. [Web](#web)
   2. [SQL](#spring-sql)
   3. [Proxy](#spring-proxy)
   4. [Pattern](#pattern)
4. [Language](#language)
   1. [Java](#java)
<br>

# Operating system

- [For interview](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/README.md)
- [Operating system](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/introduction_to_os.md)
- [process status](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/process_state.md)
- [virtual address space](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/virtual_address_space.md)
  - velog: https://velog.io/@evelyn82ny/virtual-address-space
<br>
  
## Scheduling

- [MLFQ](https://github.com/evelyn82ny/Computer-science/blob/master/Operating_system/theory/MLFQ.md)
<br>

## Concurrency & Synchronization

- [Thread](https://velog.io/@evelyn82ny/concurrency-thread)
- [Lock](https://velog.io/@evelyn82ny/concurrency-lock)
- [Semaphore](https://velog.io/@evelyn82ny/synchronization-semaphore)
<br>

### reference

- [operating systems: three easy pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)
<br>

# Network

- [OSI 7계층](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/OSI-7-layer.md)
- [unicast, broadcast, multicast, anycast](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/unicast_BUM_traffic.md)
- [MAC address](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/MAC-address.md)
- [IP address](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/IP-address.md)
<br>

- [4-way handshake & half close](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/4-way-handshake.md)
- [TCP 와 UDP](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/TCP-VS-UDP.md)
- [TCP / IP protocol](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/TCP-IP-protocol.md)
<br>

- [blocking vs non-blocking](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/blocking-vs-non-blocking.md)
- [Nagle algorithm](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/nagle-algorithm.md)
<br>

## Socket programming

- [socket, bind, listen, accept, connect](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/socket.md)
- [file descriptor, open, close, read, write](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/file-descriptor.md)
- [TCP의 데이터 경계](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/boundary-of-tcp-transmission-data.md)
- [127.0.0.1](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/localhost.md)
<br>

- [IPv4 기반의 주소표현 sockaddr_in 구조체](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/sockaddr.md)
- [IP 주소 할당하기](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/allocate-ip.md)
<br>

- [TCP 함수호출 과정](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/TCP-system-call.md)
- [UDP 는 연결이라는 개념이 존재하지 않는다.](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/UDP-system-call.md)
<br>

- [Iterative server & Echo client](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/iterative-server-and-echo-client.md)
- [DNS - gethostbyname()으로 서버 정보 요청하기](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/dns.md)
<br>

- [자식 프로세스의 종료를 체크하는 wait, signal](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/zombie-check.md)
- [multiprocess](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/multiprocess.md)
- [multiplexing](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/multiplexing.md)
- [pipe](https://github.com/evelyn82ny/Computer-science/blob/master/Network/theory/pipe.md)

<br>

### reference

- [TCP에 우아한 종료란 없다?](https://sunyzero.tistory.com/269)
- [ssize_t](https://lacti.github.io/2011/01/08/different-between-size-t-ssize-t/)

<br>

# Server

## web

- [쿠키와 세션](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/cookie_and_session.md)
- [request mapping](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/request_mapping.md)
- [view resolver](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/view_resolver.md)
- [Redirect 와 Forward 차이](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/redirect_forward.md)
<br>

- [httpServletRequest](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/httpServletRequest.md)
- [httpServletResponse](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/httpServletResponse.md)
- [http request: query](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/http_request_query.md)
- [http request: JSON](https://github.com/evelyn82ny/Computer-science/blob/master/Server/Web/http_request_json.md)
<br>

## Spring: SQL

- [PK mapping](https://velog.io/@evelyn82ny/primary-key-mapping)
<br>

## Spring: Proxy

- [ProxyFactory](https://github.com/evelyn82ny/Computer-science/blob/master/Server/proxy/ProxyFactory.md)
- [BeanPostProcessor 기본 원리](https://github.com/evelyn82ny/Computer-science/blob/master/Server/proxy/BeanPostProcessor_basic.md)
- [BeanPostProcessor - Pointcut 적용 원리](https://github.com/evelyn82ny/Computer-science/blob/master/Server/proxy/BeanPostProcessor_pointcut.md)
- [@Aspect](https://velog.io/@evelyn82ny/Spring-AOP-Aspect)
<br>

## Pattern

- [Singleton pattern](https://github.com/evelyn82ny/Computer-science/blob/master/Server/pattern/singleton_pattern.md)
- [MVC pattern](https://github.com/evelyn82ny/Computer-science/blob/master/Server/pattern/spring_mvc.md)
- [Template method pattern](https://velog.io/@evelyn82ny/template-method-pattern)
- [Strategy pattern](https://velog.io/@evelyn82ny/strategy-pattern)
- [Template callback pattern](https://velog.io/@evelyn82ny/template-callback-pattern)
- [Proxy & Decorator pattern](https://velog.io/@evelyn82ny/proxy-pattern-decorator-pattern)

<br>

# Language

## Java

- [Inheritance](https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/Inheritance.md)
- [Abstract class](https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/AbstractClass.md)
- [Data type](https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/DataType.md)
- [CallByValue](https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/CallByValue.md)
<br>

- [Garbage Collection](https://d2.naver.com/helloworld/1329)