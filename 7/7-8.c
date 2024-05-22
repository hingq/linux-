
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NUM 3
#define NUM_PRINTS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[MAX_NUM];
int current_thread = 0;

void *thread_function(void *(arg))
{

    int thread_id = *((int *)arg);

    for (int i = 0; i < NUM_PRINTS; ++i)
    {
        pthread_mutex_lock(&mutex);
        while (current_thread != thread_id)
        {
            pthread_cond_wait(&cond[thread_id], &mutex);
        }
        printf("%c", 'A' + thread_id);
        fflush(stdout);
        current_thread = (current_thread + 1) % MAX_NUM;
        // 取余
        pthread_cond_signal(&cond[current_thread]);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
int main()
{
    pthread_t thread_id[MAX_NUM];
    int t[MAX_NUM];
    for (int i = 0; i < MAX_NUM; i++)
    {
        pthread_cond_init(&cond[i], NULL);
        t[i] = i;
        pthread_create(&thread_id[i], NULL, thread_function, (void *)&t[i]);
    }
    pthread_cond_signal(&cond[0]);
    for (int i = 0; i < MAX_NUM; ++i)
    {
        pthread_join(thread_id[i], NULL);
        pthread_cond_destroy(&cond[i]);
    }
    printf("\n");
    return 0;
}
