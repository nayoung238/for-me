#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "common_threads.h"

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
    int tmp = buffer[get_ptr];
    printf("GET -> [0x%lx]: %d\n", (unsigned long)pthread_self(), tmp);
    get_ptr = (get_ptr + 1) % max;
    buffer_full--;
    return tmp;
}
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
int main(int argc, char *argv[]) {

    if(argc != 4) {
        fprintf(stderr, "usage: %s <buffersize> <loops> <consumers>\n", argv[0]);
        exit(1);
    }

    max = atoi(argv[1]);
    loops = atoi(argv[2]);
    consumers = atoi(argv[3]);

    buffer = (int *)malloc(max * sizeof(int));
    assert(buffer != NULL);
    int i;
    for(i = 0; i < max; i++) {
        buffer[i] = 0;
    }

    pthread_t tp; // thread_producer
    pthread_t tc[consumers]; // thread_consumer

    Pthread_create(&tp, NULL, producer, NULL);
    for(int i = 0; i < consumers; i++) {
        Pthread_create(&tc[i], NULL, consumer, NULL);
    }

    Pthread_join(tp, NULL);
    for(int i = 0; i < consumers; i++) {
        Pthread_join(tc[i], NULL);
    }
    return 0;
}