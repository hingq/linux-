#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define M 3
#define K 4
#define N 4
typedef struct
{
    int i; // 矩阵C的行号
    int j; // 矩阵C的列号
} thread_args;

int a[M][N] = {
    {1, 2, 3, 4},
    {1, 2, 3, 4},
    {1, 2, 3, 4}};
int B[K][N] = {
    {1, 2, 3, 4},
    {1, 2, 3, 4},
    {1, 2, 3, 4},
    {1, 2, 3, 4}};
int c[M][N];
void *row_function(void *arg)
{
    thread_args *args = (thread_args *)arg;
    int sum = 0;
    int row = args->i;
    int col = args->j;

    for (int i = 0; i < N; i++)
    {
        /* code */
        sum += a[row][i] + B[i][col];
    }
    c[row][col] = sum;
}
int main()
{
    pthread_t thread[M][N];
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            thread_args *args = malloc(sizeof(thread_args));
            args->i = i;
            args->j = j;
            pthread_create(&thread[i][j], NULL, row_function, (void*)args);
        }
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            pthread_join(thread[i][j], NULL);
        }
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", c[i][j]);
        }
        printf("\n");
    }

    return 0;
}