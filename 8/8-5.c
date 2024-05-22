#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 256

char buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
pthread_cond_t cond;

void *input_thread(void *arg) {
    while (1) {
        char input[BUFFER_SIZE];

        printf("Enter your input: ");
        fgets(input, BUFFER_SIZE, stdin);

        // 加锁
        pthread_mutex_lock(&mutex);

        // 将输入放入缓冲区
        sprintf(buffer, "%s", input);

        // 通知输出线程可以输出
        pthread_cond_signal(&cond);

        // 解锁
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void *output_thread(void *arg) {
    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);

        // 等待输入线程的通知
        pthread_cond_wait(&cond, &mutex);

        // 输出缓冲区的内容
        printf("Received input: %s", buffer);

        // 解锁
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t input_tid, output_tid;

    // 初始化互斥锁和条件变量
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // 创建线程
    pthread_create(&input_tid, NULL, input_thread, NULL);
    pthread_create(&output_tid, NULL, output_thread, NULL);

    // 等待线程结束
    pthread_join(input_tid, NULL);
    pthread_join(output_tid, NULL);

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
