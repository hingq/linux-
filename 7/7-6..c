#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_LOOPS 50
#define CHILD_LOOP_COUNT 10
#define MAIN_LOOP_COUNT 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// 条件变量
int flag = 0;

void *child_thread_function(void *arg) {
    for (int i = 0; i < NUM_LOOPS; ++i) {
        pthread_mutex_lock(&mutex);
        while (flag != 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        for (int j = 0; j < CHILD_LOOP_COUNT; ++j) {
            printf("Child thread loop %d\n", j + 1);
            usleep(100000); // 100ms
        }
        flag = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t child_thread;
    pthread_create(&child_thread, NULL, child_thread_function, NULL);

    for (int i = 0; i < NUM_LOOPS; ++i) {
        pthread_mutex_lock(&mutex);
        while (flag != 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        for (int j = 0; j < MAIN_LOOP_COUNT; ++j) {
            printf("Main thread loop %d\n", j + 1);
            usleep(100000); // 100ms
        }
        flag = 0;
        pthread_cond_signal(&cond);                        
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(child_thread, NULL);

    return 0;
}
