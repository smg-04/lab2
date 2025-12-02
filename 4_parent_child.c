#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int turn = 0;

void *child(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (turn != 1)
            pthread_cond_wait(&cond, &mutex);

        printf("hello child\n");
        sleep(1);

        turn = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, child, NULL);

    while (1) {
        pthread_mutex_lock(&mutex);
        while (turn != 0)
            pthread_cond_wait(&cond, &mutex);

        printf("hello parent\n");
        sleep(1);

        turn = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(tid, NULL);
    return 0;
}
