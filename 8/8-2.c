#include <stdio.h>
#include <pthread.h>

#define ROWS 4
#define COLS 10

int matrix[ROWS][COLS];

int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *sum_row(void *arg)
{
    int row = *((int *)arg);
    // 强制转换，后取值
    int row_sum = 0;

    for (int i = 0; i < COLS; i++)
    {
        row_sum += matrix[row][i];
    }

    pthread_mutex_lock(&mutex);
    sum += row_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[ROWS];
    int rows[ROWS];

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            matrix[i][j] = i * COLS + j + 1;
        }
    }

    for (int i = 0; i < ROWS; i++)
    {
        rows[i] = i;
        pthread_create(&threads[i], NULL, sum_row, &rows[i]);
    }

    for (int i = 0; i < ROWS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Sum : %d\n", sum);

    return 0;
}
