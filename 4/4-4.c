#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd1[2]; // 父进程写，子进程读
    int fd2[2]; // 子进程写，父进程读
    pid_t pid;
    char buffer[100];
    int nbytes;

    // 创建第一个管道
    if (pipe(fd1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建第二个管道
    if (pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // 子进程
        close(fd1[1]); // 子进程关闭第一个管道的写端
        close(fd2[0]); // 子进程关闭第二个管道的读端

        // 从父进程读取信息，并打印到屏幕上
        nbytes = read(fd1[0], buffer, sizeof(buffer));
        printf("Child process received message from parent: %.*s\n", nbytes, buffer);

        // 向父进程发送信息
        char *message = "Message from child process";
        write(fd2[1], message, sizeof(message));

        exit(EXIT_SUCCESS);
    } else { // 父进程
        close(fd1[0]); // 父进程关闭第一个管道的读端
        close(fd2[1]); // 父进程关闭第二个管道的写端

        // 向子进程发送信息
        char *message = "Message from parent process";
        write(fd1[1], message, sizeof(message));

        // 从子进程读取信息，并打印到屏幕上
        nbytes = read(fd2[0], buffer, sizeof(buffer));
        printf("Parent process received message from child: %.*s\n", nbytes, buffer);

        // 等待子进程退出
        wait(NULL);
    }

    return 0;
}
