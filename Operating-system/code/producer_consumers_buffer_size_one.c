#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "common_threads.h"

int buffer = 0;
int loops;
int consumers = 1;

int buffer_full = 0;
int fill_ptr = 0;
int get_ptr = 0;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void fill(int value) {
    buffer = value;
    buffer_full = 1;
    printf("PUT -> [0x%lx]: %d\n", (unsigned long)pthread_self(), value);
}
int get(void *arg) {
    int tmp = buffer;
    printf("GET -> [0x%lx]: %d\n", (unsigned long)pthread_self(), tmp);
    buffer_full = 0;
    return tmp;
}
void *producer(void *arg) {
    for(int i = 0; i < loops; i++) {
        Mutex_lock(&mutex);
        while(buffer_full == 1) {
            Cond_wait(&cond, &mutex);
        }
        fill(i);
        Cond_signal(&cond);
        Mutex_unlock(&mutex);
    }

    for(int i = 0; i < consumers; i++) {
        Mutex_lock(&mutex);
        while(buffer_full == 1) {
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
    if(argc != 3) {
        fprintf(stderr, "usage: %s <buffersize> <loops> <consumers>\n", argv[0]);
        exit(1);
    }

    loops = atoi(argv[1]);
    consumers = atoi(argv[2]);

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