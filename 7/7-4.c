#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_THREADS 2
#define NUM_CHILDREN 3
void *function()
{
    printf("thread:%ld\n", (long)pthread_self());
    pthread_exit(NULL);
}
int main()
{

    pid_t p[3];
    pthread_t t[2];

    for (int i = 0; i < 3; i++)
    {
        p[i] = fork();
        if (p[i] < 0)
        {
            perror("fork");
            return 1;
        }
        else if (p[i] == 0)
        {
            for (int j = 0; j < 2; j++)
            {
                pthread_create(&t[j], NULL, function, NULL);
            }
            for (int j = 0; j < NUM_THREADS; ++j)
            {
                pthread_join(t[j], NULL);
            }
            printf("porcess id:%d,thread id:%ld\n ", getpid(), (long)pthread_self());
            return 0;
        }
    }
    // 等待所有子进程结束
    for (int i = 0; i < NUM_CHILDREN; ++i)
    {
        waitpid(t[i], NULL, 0);
    }
    return 0;
}