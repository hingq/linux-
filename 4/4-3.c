#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int pipe1[2]; // 管道1：兄弟进程1向兄弟进程2发送信息
    int pipe2[2]; // 管道2：兄弟进程2向兄弟进程1发送信息
    pid_t pid1, pid2;
    char message1[BUFFER_SIZE], message2[BUFFER_SIZE];

    // 创建管道1
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    // 创建管道2
    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    // 创建兄弟进程1
    pid1 = fork();

    if (pid1 < 0) {
        perror("fork1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) { // 子进程（兄弟进程1）
        // 关闭管道1的读端和管道2的写端
        close(pipe1[0]);
        close(pipe2[1]);

        while (1) {
            // 从标准输入读取信息
            fflush(stdout);
            printf("Brother 1: ");
            fgets(message1, BUFFER_SIZE, stdin);
            // 将信息写入管道1
            write(pipe1[1], message1, strlen(message1) + 1);
            // 从管道2读取信息
            read(pipe2[0], message2, BUFFER_SIZE);
            printf("Brother 1 received: %s", message2);
        }
    }

    // 创建兄弟进程2
    pid2 = fork();

    if (pid2 < 0) {
        perror("fork2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) { // 子进程（兄弟进程2）
        // 关闭管道1的写端和管道2的读端
        close(pipe1[1]);
        close(pipe2[0]);

        while (1) {
            fflush(stdout);
            // 从管道1读取信息
            read(pipe1[0], message1, BUFFER_SIZE);
            printf("Brother 2 received: %s", message1);

            // 从标准输入读取信息
            printf("Brother 2: ");
            fgets(message2, BUFFER_SIZE, stdin);
            // 将信息写入管道2
            write(pipe2[1], message2, strlen(message2) + 1);
        }
    }

    // 关闭父进程中不使用的管道端口
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // 等待子进程退出
    wait(NULL);
    wait(NULL);

    return 0;
}
