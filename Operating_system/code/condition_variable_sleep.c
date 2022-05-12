#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

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

    pthread_join(pth_child, NULL);
    pthread_join(pth_parent, NULL);
    return 0;
}