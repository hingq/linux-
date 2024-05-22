#include <stdio.h>
#include <pthread.h>

void *thread_function(void *arg) {
    // 新线程的工作函数
    return NULL;
}

int main() {
    pthread_t thread_id;
    pthread_attr_t attr;

    // 初始化线程属性
    pthread_attr_init(&attr);

    // 获取新线程的默认属性
    pthread_create(&thread_id, &attr, thread_function, NULL);

   
    size_t stack_size;
    // 大小
    pthread_attr_getstacksize(&attr, &stack_size);
    printf("Stack Size: %zu bytes\n", stack_size);

    void *stack_addr;
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    printf("Stack Address: %p\n", stack_addr);


    // 等待新线程结束
    pthread_join(thread_id, NULL);

    return 0;
}
