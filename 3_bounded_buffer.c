#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int id = *(int *)arg;
    int item = 0;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &mutex);

        buffer[in] = item;
        printf("Producer %d produced %d at %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);

        item++;
        usleep(500000);
    }
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0)
            pthread_cond_wait(&not_empty, &mutex);

        int item = buffer[out];
        printf("Consumer %d consumed %d at %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        usleep(700000);
    }
}

int main() {
    pthread_t p1, p2, c1, c2;
    int id1 = 1, id2 = 2;

    pthread_create(&p1, NULL, producer, &id1);
    pthread_create(&p2, NULL, producer, &id2);
    pthread_create(&c1, NULL, consumer, &id1);
    pthread_create(&c2, NULL, consumer, &id2);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);

    return 0;
}
