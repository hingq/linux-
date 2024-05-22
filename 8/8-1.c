#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10

char buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// 互斥锁 临界资源访问
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// 互斥变量 阻塞进程等待条件

void *producer(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (count < BUFFER_SIZE) {
            char ch = getchar();
            buffer[count++] = ch;
            if (count == BUFFER_SIZE) {
                pthread_cond_signal(&cond); 
            }
        }
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (count < BUFFER_SIZE) {
            pthread_cond_wait(&cond, &mutex); // 等待缓冲区满
        }
        printf("the string is:\t");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%c ", buffer[i]);
        }
        printf("\n");
        count = 0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    return 0;
}
