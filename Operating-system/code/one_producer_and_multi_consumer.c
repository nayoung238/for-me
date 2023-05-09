#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

int *buffer;
int buffer_size;
int number_of_productions;
int consumers = 1;

int buffer_full = 0;
int fill_ptr = 0;
int get_ptr = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void fill(int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % buffer_size;
    buffer_full++;
    printf("PUT -> [0x%lx]: %d\n", (unsigned long)pthread_self(), value);
}
int get(void *arg) {
    int value = buffer[get_ptr];
    printf("GET -> [0x%lx]: %d\n", (unsigned long)pthread_self(), value);
    
    get_ptr = (get_ptr + 1) % buffer_size;
    buffer_full--;
    return value;
}
void *producer(void *arg) {
    for(int i = 0; i < number_of_productions; i++) {
        assert(pthread_mutex_lock(&mutex) == 0);
        while(buffer_full == buffer_size) {
            assert(pthread_cond_wait(&cond, &mutex) == 0);
        }
        fill(i);
        assert(pthread_mutex_unlock(&mutex) == 0);
        assert(pthread_cond_signal(&cond) == 0);
    }
    
    // 소비자가 -1 값을 읽으면 소비자 작업 종료
    for(int i = 0; i < consumers; i++) {
        assert(pthread_mutex_lock(&mutex) == 0);
        while(buffer_full == buffer_size) {
            assert(pthread_cond_wait(&cond, &mutex) == 0);
        }
        fill(-1);
        assert(pthread_mutex_unlock(&mutex) == 0);
        assert(pthread_cond_signal(&cond) == 0);
    }
    return NULL;
}
void *consumer(void *arg) {
    int value = 0;
    while(value != -1) {
        assert(pthread_mutex_lock(&mutex) == 0);
        while(buffer_full == 0) {
            assert(pthread_cond_wait(&cond, &mutex) == 0);
        }
        value = get(NULL);
        assert(pthread_mutex_unlock(&mutex) == 0);
        assert(pthread_cond_signal(&cond) == 0);
    }
    return NULL;
}
int main(int argc, const char *argv[]) {
    if(argc != 4) {
        fprintf(stderr, "usage: %s <buffer size> <number of productions> <consumers>\n", argv[0]);
        exit(1);
    }
    
    buffer_size = atoi(argv[1]);
    number_of_productions = atoi(argv[2]);
    consumers = atoi(argv[3]);
    
    buffer = (int*)malloc(buffer_size * sizeof(int));
    assert(buffer != NULL);
    
    for(int i = 0; i < buffer_size; i++) {
        buffer[i] = 0;
    }
    
    pthread_t tp;
    pthread_t tc[consumers];
    
    assert(pthread_create(&tp, NULL, producer, NULL) == 0);
    for(int i = 0; i < consumers; i++) {
        assert(pthread_create(&tc[i], NULL, consumer, NULL) == 0);
    }
    
    assert(pthread_join(tp, NULL) == 0);
    for(int i = 0; i < consumers; i++) {
        assert(pthread_join(tc[i], NULL) == 0);
    }
    return 0;
}