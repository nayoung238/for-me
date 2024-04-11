## Semaphore

동기화(synchronization)란 상호 배제와 순서 관계를 고려한다.

자식 스레드를 먼저 실행한 다음, 부모 스레드가 실행하는 코드를 구현하려고 한다. 하지만 스레드가 처리되는 순서를 예측할 수 없기 때문에 조건이 필요하다. 

```c
int done = 0;

void *child(void *arg) {
    printf("child\n");
    done = 1;
    return NULL;
}

void *parent(void *arg) {
    while(done == 0);
    printf("parent\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t pth_parent, pth_child;
    pthread_create(&pth_parent, NULL, parent, NULL);
    pthread_create(&pth_child, NULL, child, NULL);
    
    pthread_join(&pth_parent, NULL);
    pthread_join(&pth_child, NULL);
}
```

특정 스레드가 먼저 실행한다고 가정한다.

### 자식 Thread가 먼저 실행

자식 스레드가 실행되고 ```done = 1``` 로 update한다. 그 다음 실행되는 부모 스레드는 done 변수가 1이므로 while문을 바로 탈출해 출력한다.

### 부모 Thread가 먼저 실행

부모 스레드가 먼저 실행되는 경우 ```done == 0``` 이므로 while문을 계속 돈다. 자식 스레드가 실행되고 ```done = 1``` 로 Update해야 while문을 빠져나올 수 있다. 그러므로 **자식 스레드가 먼저 수행하고 종료하는 것을 보장**한다.

done변수를 이용해 자식 Thread가 먼저 출력하는 것을 보장할 수 있지만 부모 스레드가 while문을 계속 도는 경우 **CPU가 낭비**된다. CPU를 사용하지 않는다면 해당 스레드를 sleep(일시 정지)상태로 변환해 CPU 낭비를 줄일 수 있다.

<br>

## Semaphore

임계영역에 단 1개의 스레드만 접근할 수 있고, 임계영역에 접근하기 위해 대기중인 여러 스레드의 순서를 정하는 방법 중 하나가 **Semaphore**이다.

- ```semWait(s)``` : 임계영역에 **접근**하겠다라는 신호
- ```semSignal(s)``` : 임계영역에서 나와 **다른 스레드가 사용**하라는 신호

임계영역에 접근 및 사용하기 위해 ```semWait()``` 과 ```semSignal()``` 을 사용한다. Semaphore Pseudo code는 다음과 같다.

```c
struct semaphore {
    int count;
    queueType queue;
};

void semWait(semaphore s) {
    s.count--;
    if(s.count < 0) {  // 남은 임계영역이 없는 상태
    	요청한 스레드를 s.queue에 push 및 Block 상태로 변경
    }
    임계영역 접근
}

void setSignal(semaphore s) {
    s.count++;
    if(s.count <= 0) { // 대기중인 스레드 존재
    	queue에 있는 스레드 중 1개를 선택
        선택된 스레드의 상태를 수행가능 상태로 변경하고 ready queue에 push
    }
}
```

### semWait()

임계영역에 접근하려면 ```semWait()```을 호출해 공용 변수 count를 감소시킨다.

- ```count < 0```: 남은 임계영역이 없어 대기해야함
- ```count >= 0```: 임계영역에 접근 가능

### setSignal()

임계영역에서 나와 ```setSignal()```을 호출해 공용 변수 count를 증가한다.

- ```count <= 0```: 대기큐에 대기중인 스레드가 존재하며 그 중 1개의 스레드를 임계영역에 접근하도록 설정
- ```count > 0```: 대기중인 스레드 없음

공용 변수 count로 대기중인 스레드가 있는지, 내가 임계영역에 접근할 수 있는지 판단한다. **count가 0 또는 1의 값만 갖는다면 Binary-semaphore, 자원의 개수 값을 갖는다면 Counting-semaphore**라고 한다.

<br>

## Binary semaphore (Mutex)

Binary semaphore는 0 또는 1 값으로 상태를 파악하며 이를 **Mutex**라고도 한다. Binary-semaphore Pseudo code는 다음과 같다.

```c
struct binary_semaphore {
    enum {zero, one} value;
    queueType queue;
};

void semWaitB(binary_semaphore s) {
    if(s.value == 1) {  // 임계영역 접근 가능
        s.value = 0;
    }
    else {
    	요청한 스레드를 s.queue에 push 및 Block 상태로 변경
    }
}

void semSignalB(binary_semaphore s) {
    if(s.queue is empty()) {
        s.value = 1; // 대기하는 스레드가 없으므로 요청 즉시 바로 접근할 수 있게 1로 설정
    }
    else {  // 대기중인 스레드 존재
    	s.queue에 있는 스레드 중 1개를 제거
        선택된 스레드 상태를 수행가능 상태로 변경하고 ready queue에 push
    }
}
```

### setWaitB()

0 또는 1 값을 갖는 count 값을 확인한다.

- ```value == 1```: 즉시 임계영역에 접근 가능하며 다른 스레드의 접근을 막기 위해 0으로 설정
- ```value == 0```: 현재 접근 가능한 임계영역이 없기 때문에 대기
 
### setSignalB()

queue의 상태를 확인한다.

- ```queue is empty()```: 대기하는 스레드가 없으므로 즉시 임계영역에 접근 가능하다는 신호를 주기 위해 value를 1로 설정
- ```queue is not empty()```: 대기하는 스레드 중 1개를 선택해 임계영역에 접근
<br>

0 또는 1의 설정이므로 Counting semaphore(범용 세마포어)보다 구현이 쉽다. Binary semaphore와 Counting semaphore는 둘다 Queue를 사용해 대기 상태의 스레드를 관리한다. 그렇기 때문에 기아 상태가 발생하지 않고 많은 OS에서 사용중인 방식이다. 

하지만 대기 중인 스레드 중 어떤 스레드를 선택해야 하는가에 대한 기준이 필요하다. **FIFO 방식을 사용하면 String semaphore(강성 세마포어)** 이고 딱히 기준이 없다면 Weak semaphore(약성 세마포어)라고 한다. Weak semaphore는 잘 사용하지 않는다.

<br>

## Mutual exclusion 상호 배제

Semaphore로 상호 배제하는 과정은 다음과 같다.

![](/_img/Operating-system/semaphore.jpg)

접근 가능한 임계영역이 1개라면 세마포어 값을 1로 설정한다. 즉, n개의 임계영역이 있다면 세마포어 값을 n으로 설정한다.

A가 임계영역에 접근하기 위해 ```setWait()```를 호출해 세마포어 값을 감소시켰다. 감소시킨 값이 0보다 크거나 같다면 이전의 값이 양수였음을 알 수 있다. 즉, 접근가능한 임계영역이 존재한다는 뜻이므로 A는 바로 임계영역에 접근한다.

그 후 B가 임계영역에 접근하기 위해 ```setWait()```를 호출해 세마포어 값을 감소시켰는데 음수가 되었다. 즉, 접근가능한 임계영역이 없다는 뜻이므로 대기큐로 push 한다. C도 마찬가지다.

현재 대기큐에는 B와 C가 있고 세마포어 값은 -2이다. 세마포어 값이 음수라는 것은 해당 값의 **절댓값만큼의 프로세스가 대기하는 중**이라는 의미이다.  

A가 임계영역에서 나와 ```setSignal()```을 호출해 세마포어 값을 증가시킨다. 만약 증가시킨 값이 0보다 작거나 같다면 이전 값이 음수라는 것을 알 수 있다. 즉, 대기 중인 스레드가 있다는 뜻이므로 FIFO 방식으로 사용한다면 제일 먼저 들어온 스레드를 꺼내 **실행가능 상태로 변경**한다. 임계영역에서는 위 사진과 같이 스레드가 **직렬화**된다.

<br>

# Condition variable

- condition_variable.c 전체 코드: https://github.com/evelyn82ny/for-developer/blob/0ee6445c377bfd79ac709f24a11b7744728efb39/Operating-system/code/condition_variable.c

CPU를 사용하지 않는 스레드를 Sleep 상태로 만들기 위해 Condition variable(조건 변수)를 사용한다. 조건 변수를 통해 스레드가 특정 조건을 만족하지 못하면 Queue에 대기시켜 Sleep 상태로 만든다. 

```c
int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // lock
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;     // 조건 변수

void *child(void *arg) {
    printf("child\n");
    pthread_mutex_lock(&mutex);
    done = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void *parent(void *arg) {
    pthread_mutex_lock(&mutex);
    while(done == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("parent\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}
```
pthread_cond_t 타입의 cond 변수가 조건 변수이다.

- parent method에서는 **cond 조건 변수에 대해** ```wait()``` 을 호출
- child method에서는 **cond 조건 변수에 대해** ```signal()``` 을 호출
<br>

## pthread_cond_signal()

child method에서 사용하는 ```pthread_cond_signal(pthread_cond_t *c)``` 라이브러리는 pthread_cond_t 타입인 **조건 변수**만 매개 변수로 갖는다.

```signal()```은 **대기 중인 Thread를 깨우는 역할**을 한다. 만약 부모 Thread가 먼저 실행되었다면 ```done == 0``` 이므로 ```wait()```을 호출하고 Sleep 상태가 된다. 그 후 자식 Thread가 ```done = 1``` 로 설정한 다음 ```Signal()``` 을 호출해 **sleep 상태인 부모 Thread를 깨운다**.
<br>

## pthread_cond_wait()

parent method에서 사용하는 ```pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m)``` 라이브러리는 pthread_cond_t 타입인 조건 변수와 pthread_mutex_t 타입인 Lock을 매개변수로 받는다.

![](/_img/Operating-system/condition_variable_code_review.png)

조건 변수를 사용하는 이유는 스레드가 특정 조건을 만족하지 못하면 **Sleep 상태로 만들기 위함**이다. 만약 Parent method가 먼저 실행되어 Lock을 걸고 특정 조건을 만족하지 못해 Sleep 상태가 된다면 **이미 lock이 걸려있는 상태이므로 다른 스레드는 임계 영역에 들어갈 수 없다**. 즉, 자식 Thread는 child method에서 공유 자원인 done에 접근하지 못해 ```done == 0``` 으로 유지되고 **부모 Thread는 계속 sleep 상태**가 된다. 이를 방지하기 위해 ```pthread_cond_wait()```을 사용한다. 

```pthread_cond_wait()```을 호출하면 Cond 변수로 Signal이 전달될 때까지 기다린다. 기다리는 과정에서 CPU를 사용하지 않기 때문에 내부에서 ```pthread_unlock_mutex()```를 호출해 **mutex 잠금을 되돌려줘 CPU의 낭비를 막는다**. 

그 후 Cond 변수가 Signal을 받는다면 해당 Thread는 실행 가능상태로 변하고 내부에서 ```pthread_lock_mutex()```를 호출해 Mutex 잠금을 다시 얻는데 이 모든 과정은 **원자적으로 수행**된다.
<br>

```text
nayoung@nayoungui-MacBookPro thread % gcc condition_variable.c -o cv -lpthread
nayoung@nayoungui-MacBookPro thread % ./cv
child
parent
```

위 코드를 실행하면 위와 같이 원하는 순서대로 출력할 수 있고 CPU를 사용하지 않을 때 Sleep 상태로 만들어 CPU 낭비를 줄일 수 있다.

<br>

## 전역 변수 done의 필요성

위 condition_variable.c 에서 자식과 부모 Thread의 순서를 조절하기 위해 cond 변수에 대한 ```pthread_cond_signal()```과 ```pthread_cond_wait()``` 라이브러리를 사용했고 **done 변수**까지 사용했다. 

- 부모 Thread는 전역 변수 done이 1이 될 때까지 ```pthread_cond_wait()``` 호출하고 대기
- 자식 Thread는 전역 변수 done이 1로 바꾼 뒤 ```pthread_cond_signal()``` 호출해 대기 중인 Thread를 깨움

다시 생각해보니 **done 전역 변수**없이 부모 Thread는 Lock을 걸고 Cond 조건 변수에 대해 ```pthread_cond_wait()```를 호출하고, 자식 Thread도 Lock을 걸고 Cond 조건 변수에 대해 ```pthread_cond_signal()```를 호출해도 자식, 부모 순서로 Thread 수행이 마무리 될 것 같아 기존 코드를 수정해 돌려봤다.

- condition_variable_sleep.c 코드: https://github.com/evelyn82ny/for-developer/blob/master/Operating-system/code/condition_variable_sleep.c

```c
void *child(void *arg) {
    printf("child\n");
    pthread_mutex_lock(&mutex);
    //done = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void *parent(void *arg) {
    pthread_mutex_lock(&mutex);
    //while(done == 0)
        pthread_cond_wait(&cond, &mutex);
    printf("parent\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}
int main(int argc, const char * argv[]) {
    pthread_t pth_parent, pth_child;

    pthread_create(&pth_child, NULL, child, NULL);
    sleep(2);
    pthread_create(&pth_parent, NULL, parent, NULL);
    // 아래 생략
}
```
전역 변수 done이 없는 상태로 수정했으며 자식 Thread가 먼저 수행되는 것을 보장하기 위해 자식 Thread를 생성하고 ```sleep()```를 사용해 2초 후에 부모 Thread를 생성했다. 결과는 아래와 같다.

```text
nayoung@nayoungui-MacBookPro concurrency % gcc condition_variable_sleep.c -o cv -lpthread
nayoung@nayoungui-MacBookPro concurrency % ./cv
child
^C
```
강제 종료해야 프로그램이 끝난다. 그 이유는 자식 Thread가 먼저 수행되고 조건 변수 Cond에 대해 ```pthread_cond_signal()```을 호출하지만 대기 중인 Thread가 없기 때문에 아무런 처리없이 수행을 마무리 한다.

그 후 부모 Thread가 Lock을 걸고 조건 변수 Cond에 대해 ```pthread_cond_wait()```을 호출한다. 하지만 **더 이상 조건 변수 Cond에 대해** ```pthread_cond_signal()```**을 호출해줄 Thread가 없기 때문에 부모 Thread는 계속 Sleep 상태를 유지**하기 때문에 ```^C```를 입력해 프로그램을 종료시켜야 한다. 
<br>

전역 변수 done이 존재했다면 자식 Thread가 먼저 실행되어도 문제되지 않는다. 자식 Thread가 먼저 실행되어 ```done = 1```로 설정 후 Cond에 대해 ```pthread_cond_signal()```을 호출한다. 이 때 부모 Thread가 Cond에 대해 ```pthread_cond_wait()```을 호출하지 않았다면 아무런 처리없이 수행을 마무리 한다.

하지만 이미 ```done == 1```로 설정되었기 때문에 부모 Thread는 while문을 바로 탈출해 무한 Sleep 상태가 되는 것이 피할 수 있게 되어 위와 같은 문제가 발생하지 않는다.