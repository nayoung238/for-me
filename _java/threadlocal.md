## ThreadLocal

- 전체 코드: https://github.com/evelyn82ny/ThreadLocal

ThreadLocal은 동시성 문제를 해결하기 위한 클래스로 각 스레드에게 독립적인 공간을 제공한다. 각 스레드는 자신에게 할당된 내부 저장소에서 ```get()```, ```set()```으로 값을 조회 및 저장할 수 있다. 

## concurrency issues

Spring bean을 singleton으로 보장한다는 것은 객체의 Instance가 Application에 딱 1개만 존재함을 보장하는 것을 의미한다. 메모리를 아낄 수 있다는 장점도 있지만 1개만 존재하는 field 에 여러 스레드가 동시에 접근하면 **동시성(concurrency)** 문제가 발생한다는 단점도 있다.

같은 상품에 대해 여러 관리자가 가격을 수정하는 과정에서 동시성 문제가 발생하는 과정은 다음과 같다.

- commit : https://github.com/evelyn82ny/ThreadLocal/commit/8c758340762dfbb49a30e1742ef9e4bdd46ddc51

```java
@Slf4j
public class ItemService {

    private Long price;

    public void changePrice(Long price)  {
        log.info("저장: 기존 가격 = {} -> 변경 가격= {}", this.price, price);
        this.price = price;

        sleep(1000);
        log.info("조회: 현재 가격 = {}", this.price);
    }
}
```
Singleton인 ItemService에 여러 관리자가 접근한다. changePrice method 로 가격을 수정하고 **1초 후** 수정된 가격을 조회하는 과정에서 발생되는 동시성 문제는 다음과 같다.

### 동시성 문제 발생

같은 상품에 대해 userA 는 1000원으로, userB 는 2000원으로 가격을 수정하려고 한다. userA 의 가격 수정을 thread_A 가 처리하고, userB 의 가격 수정을 thread_B 가 처리하도록 설정했다.

```java
void concurrencyTest() {
	Runnable userA = () -> itemService.changePrice(1000L);
    Runnable userB = () -> itemService.changePrice(2000L);

    Thread threadA = new Thread(userA);
    threadA.setName("thread_A");

	Thread threadB = new Thread(userB);
    threadB.setName("thread_B");

    threadA.start();
    sleep(100);  
    threadB.start();

    sleep(3000);
}
```
두 스레드 사이에 ```sleep(100);``` 으로 설정했기 때문에 thread_A가 시작되고 0.1초 후에 thread_B가 시작된다. 즉, changePrice method는 가격을 수정하고 **1초 후**에 조회하므로 **thread_A 가 수정 및 조회 과정을 모두 처리하기 전에 thread_B 가 실행**한다.

```text
[thread_A] INFO ... 저장: 기존 가격 = null -> 변경 가격 = 1000
[thread_B] INFO ... 저장: 기존 가격 = 1000 -> 변경 가격 = 2000
[thread_A] INFO ... 조회: 현재 가격 = 2000
[thread_B] INFO ... 조회: 현재 가격 = 2000
```
thread_A는 실행하자마자 null인 가격을 1000원으로 수정한다. thread_A가 실행하고 0.1초 후에 thread_B가 실행되므로 **threadA가 수정된 가격을 조회하기 전에 threadB의 가격 수정이 먼저 처리**된다. 그렇기 때문에 위 log 를 보면 thread_A의 저장 후 thread_B의 저장이 수행되었음을 알 수 있다.

thread_B가 price field에 접근했을 땐 thread_A에 의해 1000원으로 수정된 상태이므로 기존 가격이 1000원으로 출력된다.

thread_A는 price를 1000원으로 수정하고 1초 후 price를 조회했지만 1000원이 아닌 2000원이 출력된다. 1초 사이에 thread_B가 같은 price field에 접근해 2000원으로 수정했기 때문이다. 

![](/_img/Language/threadlocal/access_the_same_reference.png)

이렇게 여러 스레드가 같은 reference에 접근해 수정하는 경우 발생되는 문제를 **동시성** 문제라고 한다.
<br>

```java
threadA.start();
threadB.start();
```

위 ```sleep(100);``` 코드를 제거하면 다음과 같은 log 가 출력된다.

```text
[thread_A] INFO ... 저장: 기존 가격 = null -> 변경 가격 = 1000
[thread_B] INFO ... 저장: 기존 가격 = null -> 변경 가격 = 2000
[thread_A] INFO ... 조회: 현재 가격 = 2000
[thread_B] INFO ... 조회: 현재 가격 = 2000
```

두 스레드가 거의 동시에 같은 field에 접근하므로 둘다 기존 가격이 null 로 출력되고 조회 시 마지막에 수정된 가격인 2000 원으로 출력된다.
<br>


### 동시성 문제 발생하지 않음

```java
void concurrencyTest() {
	Runnable userA = () -> itemService.changePrice(1000L);
    Runnable userB = () -> itemService.changePrice(2000L);

    Thread threadA = new Thread(userA);
    threadA.setName("thread_A");

	Thread threadB = new Thread(userB);
    threadB.setName("thread_B");

    threadA.start();
    sleep(2000);  
    threadB.start();

    sleep(3000);
}
```
동시성 문제를 발생시키지 않기 위해 두 thread 사이에 ```sleep()``` 설정 시간을 2초로 설정했다. 즉, threadA가 처리된 다음 2초 후에 thtreadB가 처리된다. 

```text
[thread_A] INFO ... 저장: 기존 가격 = null -> 변경 가격 = 1000
[thread_A] INFO ... 조회: 현재 가격 = 1000
[thread_B] INFO ... 저장: 기존 가격 = 1000 -> 변경 가격 = 2000
[thread_B] INFO ... 조회: 현재 가격 = 2000
```
위 log 를 보면 thread_A가 수정 및 조회한 후 thread_B의 저장 및 조회가 실행되었다. 즉, 같은 field에 접근했지만 서로 영향을 주지 않았다.

2개의 thread만 조절하면 되니깐 ```sleep()```으로 동시성 문제를 해결할 수 있었다. 하지만 실제 서비스에서 사용되는 수많은 thread에 대해 일일이 ```sleep()```을 설정할 수 없다. 이 문제를 해결하기 위해 **ThreadLocal**을 사용한다.
<br>


## Apply ThreadLocal

![](/_img/Language/threadlocal/ThreadLocal.png)

ThreadLocal은 **Thread 단위로 local 변수를 할당**한다. 즉, 스레드마다 별도의 내부 저장소를 제공받기 때문에 같은 instance의 thread local field에 접근해도 동시성 문제가 발생되지 않는다.

```java
public class ItemService {

    // private Long price;
    private ThreadLocal<Long> price = new ThreadLocal<>();
}
```

price field에 대해 스레드마다 별도의 내부 저장소를 제공하기 위해 위와 같이 Long 타입에서 ```ThreadLocal<Long>``` 으로 수정했다. 각 thread는 해당 field에 값을 저장, 조회 그리고 삭제를 하기위해 다음과 같은 method를 사용한다.

- 저장: ThreadLocal.set(value);
- 조회: ThreadLocal.get();
- 삭제: ThreadLocal.remove();

스레드마다 내부 저장소가 별도로 있기 때문에 index 접근이 아니고 ```set(value)```, ```get()``` 만 작성하면 된다.

- commit : https://github.com/evelyn82ny/ThreadLocal/commit/ff25ff30755071067b083b4a5bbdca72a305bd91

```java
public class ItemService {

    private String name;
    // private Long price;
    private ThreadLocal<Long> price = new ThreadLocal<>();
    
    public void changePrice(Long price)  {
    	// this.price -> this.price.get() 으로 변경
        log.info("저장: 기존 가격 = {} -> 변경 가격 = {}", this.price.get(), price);
        
        // this.price = price 이 아닌 set() 사용
        this.price.set(price);

        sleep(1000);

        log.info("조회: 현재 가격 = {}", this.price.get());
    }
}
```

```java
threadA.start();
sleep(2000);
threadB.start();
```

두 스레드 사이에 ```sleep(2000);``` 을 설정했다.

```text
[thread_A] INFO ... 저장: 기존 가격 = null -> 변경 가격 = 1000
[thread_A] INFO ... 조회: 현재 가격 = 1000
[thread_B] INFO ... 저장: 기존 가격 = null -> 변경 가격 = 2000
[thread_B] INFO ... 조회: 현재 가격 = 2000
```
```sleep(2000);``` 으로 thread_A가 저장 및 조회까지 한 상태에서 thread_B의 저장이 수행되었음을 위 log를 통해 확인할 수 있다. 

이론상 같은 field에 접근했지만 ThreadLocal을 적용했기 때문에 thread_A가 수행을 끝내고 thread_B가 price field에 접근해도 price의 초기 값인 null 로 출력된다. 이는 **스레드마다 내부 저장소가 있기 때문**이다.

```java
threadA.start();
threadB.start();
```

```text
[thread_A] INFO ... 저장: 기존 가격 = null -> 변경 가격 = 1000
[thread_B] INFO ... 저장: 기존 가격 = null -> 변경 가격 = 2000
[thread_A] INFO ... 조회: 현재 가격 = 1000
[thread_B] INFO ... 조회: 현재 가격 = 2000
```

두 스레드가 동시에 price field를 수정해도 각 스레드가 수정한 가격으로 조회되는 것을 통해 스레드 간 아무런 영항을 주지 않음을 알 수 있다.
<br>

## Thread Pool

Application 개발 시 스레드가 필요하면 생성해 사용하면 된다. 스레드를 생성할 때마다 OS는 스택 영역을 확보하고 더이상 사용되지 않을 경우 메모리 영역을 회수하는 작업에서 큰 비용이 발생한다.

![](/_img/Language/threadlocal/thread_pool.png)

> 출처: https://velog.io/@haero_kim/Java-Thread-Pool-%EA%B0%9C%EB%85%90%EA%B3%BC-%EB%8F%99%EC%9E%91%EC%9B%90%EB%A6%AC

<br>

이를 해결하고자 Thread Pool에 여러 스레드를 미리 만들어두고 작업을 할당한다. 작업이 끝나면 해당 스레드가 제거되지 않고 **재사용**하기 때문에 비용 발생을 막아 성능 저하를 방지한다.

### ThreadLocal.remove()

Thread Pool 에 있는 스레드는 재사용된다. 작업이 끝나 반환된 스레드에는 데이터가 아직 살아있다. **반환된다고 자동으로 지워지지 않는다**.

그렇기 때문에 다른 작업이 해당 스레드에게 할당되면 원치않는 데이터에 접근 가능하다. 중요한 데이터일 경우 유출되는 심각한 문제가 발생하게 된다. 그렇기 때문에 작업이 끝나면 ```remove()```를 호출해 스레드 내의 로컬 데이터를 제거해야 한다.

- commit: https://github.com/evelyn82ny/ThreadLocal/commit/9953d7409ceb61fbda214ba9b75348c8d39e0ca5

```java
log.info("조회: 현재 가격 = {}", this.price.get());

this.price.remove();
log.info("thread 삭제: 가격 = {}", this.price.get());
```

```text
[thread_A] INFO ... 조회: 현재 가격 = 1000
[thread_A] INFO ... thread 삭제: 가격 = null
```