#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 5; i++) {
        printf("Thread %d running... %d\n", id, i);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    int id1 = 1, id2 = 2;

    pthread_create(&tid1, NULL, thread_func, &id1);
    pthread_create(&tid2, NULL, thread_func, &id2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("Main thread finished.\n");
    return 0;
}
