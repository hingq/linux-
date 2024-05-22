#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

int global_var = 0;
pthread_mutex_t mutex;

void *thread_function(void *arg) {
    int *thread_id = (int *)arg;
    int i;

    pthread_mutex_lock(&mutex);
    for (i = 0; i < 10; ++i) {
        global_var += 1;
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    void *thread_result;
    int i;

    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL);

    // 创建线程
    for (i = 0; i < NUM_THREADS; ++i) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, thread_function, (void *)&thread_args[i]);
    }

    // 等待线程结束并获取结果
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], &thread_result);
    }

    // 销毁互斥锁
    pthread_mutex_destroy(&mutex);
    printf("Total sum: %d\n", global_var);

    return 0;
}
