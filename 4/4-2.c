#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pid_t pid;

    // 创建管道
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // 子进程
        // 关闭管道的读端,确保子进程不会读取数据
        close(fd[0]);

        // 将标准输出重定向到管道的写端
        if (dup2(fd[1], 1) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        // 执行 cat 命令
        execlp("cat", "cat", "file", NULL);
        perror("execlp cat");
        exit(EXIT_FAILURE);
    } else { // 父进程
        // 关闭管道的写端
        close(fd[1]);
        // 等待子进程结束
        wait(NULL);
        // 将标准输入重定向到管道的读端
        if (dup2(fd[0], 0) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        // 执行 sort 命令
        execlp("sort", "sort", NULL);
        perror("execlp sort");
        exit(EXIT_FAILURE);
    }

    return 0;
}
