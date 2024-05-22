#include <stdio.h>
#include <pthread.h>

void *thread_function(void *arg) {
    // 输出线程号
    printf("Thread ID: %ld\n", (long)pthread_self());
    return NULL;
}

int main() {
    pthread_t thread_id[4]; // 保存新线程的线程号

    // 创建4个新线程
    for (int i = 0; i < 4; ++i) {
        // 指针，线程属性，线程函数首地址，函数参数
        pthread_create(&thread_id[i], NULL, thread_function, NULL);
    }

    // 等待所有线程结束
    for (int i = 0; i < 4; ++i) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
