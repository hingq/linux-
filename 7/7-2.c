#include <stdio.h>
#include <pthread.h>

#define ROWS 5
#define COLS 8

int matrix[ROWS][COLS] = {
    {1, 2, 3, 4, 5, 6, 7, 8},
    {9, 10, 11, 12, 13, 14, 15, 16},
    {17, 18, 19, 20, 21, 22, 23, 24},
    {25, 26, 27, 28, 29, 30, 31, 32},
    {33, 34, 35, 36, 37, 38, 39, 40}
};

int sum = 0; // 保存所有元素的和
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 互斥锁，用于保护sum的访问

// 计算一行的元素和
void *row_function(void *arg) {
    int row = *(int *)arg;
    int row_sum = 0;
    for (int j = 0; j < COLS; ++j) {
        row_sum += matrix[row][j];
    }

    // 使用互斥锁保护sum的访问
    pthread_mutex_lock(&mutex);
    sum += row_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[ROWS];
    int row_numbers[ROWS] = {0, 1, 2, 3, 4}; // 每个线程负责的行号

    // 创建5个线程，每个线程负责计算一行元素的和
    for (int i = 0; i < ROWS; ++i) {
        pthread_create(&threads[i], NULL, row_function, &row_numbers[i]);
    }

    // 等待所有线程结束
    for (int i = 0; i < ROWS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // 输出总和
    printf("Sum : %d\n", sum);

    return 0;
}
