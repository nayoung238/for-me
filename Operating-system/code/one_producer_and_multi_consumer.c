#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int *buffer;
int loops;
int max;
int consumers = 1;

int buffer_full = 0;
int fill_ptr = 0;
int get_ptr = 0;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void fill(int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % max;
    buffer_full++;
    printf("PUT -> [0x%lx]: %d\n", (unsigned long)pthread_self(), value);
}
int get(void *arg) {
    int temp = buffer[get_ptr];
    printf("GET -> [0x%lx]: %d\n", (unsigned long)pthread_self(), temp);
    get_ptr = (get_ptr + 1) % max;
    buffer_full--;
    return temp;
}
void *producer(void *arg) {
    for(int i = 0; i < loops; i++) {
        assert(pthread_mutex_lock(&mutex) == 0);
        while(buffer_full == max) {
            assert(pthread_cond_wait(&cond, &mutex) == 0);
        }
        fill(i);
        assert(pthread_cond_signal(&cond) == 0);
        assert(pthread_mutex_unlock(&mutex) == 0);
    }
    for(int i = 0; i < loops; i++) {
        assert(pthread_mutex_lock(&mutex) == 0);
        while(buffer_full == max) {
            assert(pthread_cond_wait(&cond, &mutex) == 0);
        }
        fill(-1);
        assert(pthread_cond_signal(&cond) == 0);
        assert(pthread_mutex_unlock(&mutex) == 0);
    }
    return NULL;
}
void *consumer(void *arg) {
    int tmp = 0;
    while(tmp != -1) {
        assert(pthread_mutex_lock(&mutex) == 0);
        while(buffer_full == 0) {
            assert(pthread_cond_wait(&cond, &mutex) == 0);
        }
        tmp = get(NULL);
        assert(pthread_cond_signal(&cond) == 0);
        assert(pthread_mutex_unlock(&mutex) == 0);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        fprintf(stderr, "usage: %s <buffersize> <loops> <consumers>\n", argv[0]);
        exit(1);
    }
    
    max = atoi(argv[1]);
    loops = atoi(argv[2]);
    consumers = atoi(argv[3]);
    
    buffer = (int*)malloc(max * sizeof(int));
    assert(buffer != NULL);
    
    int i;
    for(int i = 0; i < max; i++) {
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