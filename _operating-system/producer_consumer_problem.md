# 생산자 / 소비자 문제

위 condition_variable.c 에서 부모 Thread가 ```wait()``` 를 호출해 sleep 상태가 되면 자식 Thread가 ```signal()``` 을 호출해 깨웠다. 비슷한 유형으로 **Producer(생산자) / Consumer(소비자) 문제**가 있다.

생산자는 데이터를 생산해 **Buffer라는 공유 자원**에 넣어 두고, 소비자는 생산된 데이터를 가져온다. 여러 생산자와 소비자가 존재할 수 있으며 여러 생산자와 소비자가 Buffer라는 공유 자원에 접근해야 하므로 **상호 배제를 위한 Lock 설정**이 필요하다. 또한, 생산한 데이터가 없는데 소비자가 계속해서 접근하는 경우 CPU가 낭비되므로 적절한 **순서를 설정하기 위해 Condition variable을 사용**한다. 

이렇게 상호 배제와 순서 관계를 고려하는 것을 **동기화(synchronization)** 라고 한다.

![](/_img/Operating-system/producer_consumer.png)

보통 Lock을 걸고 해제하는 코드는 같은 Method에 존재한다. 하지만 ```pthread_cond_signal(&cond);``` 와 ```pthread_cond_wait(&cond, &mutex)``` 는 서로 다른 메서드에서 짝을 이룬다. 이렇게 다른 메서드에서 짝을 이루는 경우 생산자/소비자 문제일 가능성이 높다고 한다.

<br>

# 1명의 생산자와 다수 소비자

- https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/code/one_producer_and_multi_consumer.c

![](/_img/Operating-system/cond_relation.png)

one_producer_and_multi_consumer.c 코드에서도 생산자와 소비자의 순서를 제어하기 위해 pthread_cond_t 타입인 조건 변수 cond를 사용했다.

```Cond_wait()``` 과 ```Cond_signal()``` 라이브러리가 같은 Method에 존재하지만 같은 Method끼리는 영향을 미치지 않고 위 그림과 같이 ```producer``` 와 ```consumer``` method 사이에서 영향을 주고 받는다.
<br>

## Producer

```c
void fill(int value) {
    buffer[fill_ptr] = value; // Data 생산
    fill_ptr = (fill_ptr + 1) % max;
    buffer_full++;
}
```
생산자는 fill method를 호출해 Data를 생산한다. 

데이터가 없는데 소비자가 계속해서 접근하는 것을 방지하기 위해 buffer_full 변수를 사용한다. 데이터를 생산했으므로 소비자가 접근되는 상태임을 알려주기 위해 buffer_full 변수 값을 올려준다.

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
    for(int i = 0; i < loops; i++) {
        Mutex_lock(&mutex);
        while(buffer_full == max)
            Cond_wait(&cond, &mutex);
        fill(i);
        Cond_signal(&cond);
        Mutex_unlock(&mutex);
    }
    
    for(int i = 0; i < consumers; i++) {
        Mutex_lock(&mutex);
        while(buffer_full == max) {
            Cond_wait(&cond, &mutex);
        }
        fill(-1);
        Cond_signal(&cond);
        Mutex_unlock(&mutex);
    }
    return NULL;
}
```
생산자는 2개의 for문을 수행한다. 첫번째 for문에선 데이터를 생산하고 두번째 for문은 데이터 생산이 중단됨을 소비자에게 알리기 위한 for문이다.

![](/_img/Operating-system/cond_relation_producer_consumer.png)

생산자는 fill method로 자원을 생산한 다음 ```Cond_signal()```을 호출한다. consumer method를 보면 **자원이 없는 경우(buffer_full == 0)** ```Cond_wait()```을 호출해 대기 상태가 된다. 즉, 생산자는 자원을 생산하고 생산된 자원을 소비자가 사용할 수 있게 ```Cond_signal()```을 호출해 대기 중인 소비자를 깨운다.

두번째 for문에서 소비자 수만큼 ```-1``` 값의 데이터를 생산한다. 소비자가 ```-1``` 값을 읽어오면 더 이상 데이터가 생산되지 않음을 파악하고 종료한다.
<br>

## Consumer

```c
int get(void *arg) {
    int tmp = buffer[get_ptr];
    get_ptr = (get_ptr + 1) % max;
    buffer_full--;
    return tmp;
}
```
소비자는 get method를 통해 생산된 Data에 접근한다. Data를 사용했기 때문에 buffer_full 변수 값을 줄인다.

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *arg) {
    int tmp = 0;
    while(tmp != -1) {
        Mutex_lock(&mutex);
        while(buffer_full == 0) {
            Cond_wait(&cond, &mutex);
        }
        tmp = get(NULL);
        Cond_signal(&cond);
        Mutex_unlock(&mutex);
    }
    return NULL;
}
```
소비자는 처음부터 자원이 있거나 Cond 변수가 Signal을 받아 자원에 접근할 수 있는 상태가 되면 get method를 통해 자원에 접근한다. 그 후 ```Cond_signal()```로 대기 중인 생산자를 깨운다. 그리고 자신이 얻은 자원의 값이 -1 일 경우 더이상 자원을 얻지 않는다.

![](/_img/Operating-system/cond_relation_consumer_producer.png)

producer method를 보면 **자원을 생산할 자리가 없을 경우(buffer_full == max)** ```Cond_wait()```을 호출해 대기 상태가 된다.

소비자는 get method로 자원을 사용하고 buffer_full 변수 값을 감소시킨다. 그 후 ```Cond_signal()```로 대기 중인 생산자를 깨운다. 대기 중인 생산자의 Cond 변수에 Signal을 받으면 while문을 빠져나와 다시 자원을 생산한다.
<br>

## 결과

5개의 데이터를 생산하고 소비자가 3명일 경우 결과는 다음과 같다.

```bash
nayoung@nayoungui-MacBookPro concurrency % gcc one_producer_and_multi_consumer.c -o pc -lpthread
nayoung@nayoungui-MacBookPro concurrency % ./pc 5 5 3
PUT -> [0x16afbb000]: 0
PUT -> [0x16afbb000]: 1
PUT -> [0x16afbb000]: 2
GET -> [0x16b0d3000]: 0
GET -> [0x16b15f000]: 1
GET -> [0x16b0d3000]: 2
PUT -> [0x16afbb000]: 3
PUT -> [0x16afbb000]: 4
PUT -> [0x16afbb000]: -1
PUT -> [0x16afbb000]: -1
PUT -> [0x16afbb000]: -1
GET -> [0x16b15f000]: 3
GET -> [0x16b15f000]: 4
GET -> [0x16b15f000]: -1
GET -> [0x16b047000]: -1
GET -> [0x16b0d3000]: -1
```
소비자가 가져온 데이터를 출력할 때 thread ID를 같이 출력하도록 작성했다. 자세히보면 ID가 다름을 알 수 있다.

![](/_img/Operating-system/one_producer_multi_consumers_result.png)

소비자는 데이터가 있을 경우에만 데이터에 접근하며 3개의 Thread가 예측할 수 없는 순서로 진행됨을 알 수 있다.

<br>

# 다수 생산자와 다수 소비자

- https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/code/multi_producer_and_multi_consumer.c

이번엔 N명의 생산자가 M(max)개의 자원을 만들어내는 상황을 구현했다.

```c
void fill(int value) {
    if(fill_ptr < max) {  // 자원 생성
        buffer[fill_ptr] = value;
        fill_ptr = fill_ptr + 1;
    }
    else {  // 더이상 자원을 만들지 않음을 알려주기 위함
        buffer[exit_ptr] = value;
        exit_ptr = exit_ptr + 1;
    }
    buffer_full++;
    printf("PUT -> [0x%lx]: %d\n", (unsigned long)pthread_self(), value);
}
```

```cpp
void *producer(void *arg) {
    while(fill_ptr < max) {
        Mutex_lock(&mutex);
        while(buffer_full == max) {
            Cond_wait(&cond, &mutex);
        }
        if(fill_ptr < max) {
        	fill(fill_ptr);
        }
        Cond_signal(&cond);
        Mutex_unlock(&mutex);
    }
    while(exit_ptr < consumers) {
        Mutex_lock(&mutex);
        while(buffer_full == max) {
            Cond_wait(&cond, &mutex);
        }
        fill(-1);
        Cond_signal(&cond);
        Mutex_unlock(&mutex);
    }
    return NULL;
}
```

### if(fill_ptr < max) fill(fill_ptr);

생산자는 현재까지 만들어진 자원(fill_ptr)이 M(max)보다 작다면 자원을 만든다.

### while(exit_ptr < consumers) fill(-1);

생산자가 M(max)개의 자원을 만들었다면 더이상 자원을 만들지 않음을 소비자에게 알려주기 위해 소비자 수만큼 -1 값을 갖는 자원을 만든다. 소비자는 -1 값을 읽으면 종료한다.
<br>

## 결과

```text
nayoung@nayoungui-MacBookPro concurrency % gcc multi_producer_and_multi_consumer.c -o multi -lpthread
nayoung@nayoungui-MacBookPro concurrency % ./multi 5 2 3
PUT -> [0x16cedf000]: 0
GET -> [0x16cff7000]: 0
PUT -> [0x16cf6b000]: 1
GET -> [0x16d083000]: 1
PUT -> [0x16cf6b000]: 2
PUT -> [0x16cf6b000]: 3
PUT -> [0x16cf6b000]: 4
PUT -> [0x16cf6b000]: -1
PUT -> [0x16cf6b000]: -1
GET -> [0x16cff7000]: 2
GET -> [0x16cff7000]: 3
GET -> [0x16d10f000]: 4
GET -> [0x16d10f000]: -1
GET -> [0x16cff7000]: -1
PUT -> [0x16cf6b000]: -1
GET -> [0x16d083000]: -1
```

![](/_img/Operating-system/multi_producers_and_multi_consumers_result.png)

다수 생산자가 만든 자원을 다수 소비자가 정상적으로 얻는 것을 알 수 있다.