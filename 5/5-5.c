#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void sigint_handler(int signum) {
    printf("Received in children\n");
    system("sl", "sl", NULL); // 在子进程中执行sl命令
}

int main() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程代码
        signal(SIGINT, sigint_handler);
        // 子进程持续运行
        while (1) {
            sleep(1);
        }
    } else {
        // 父进程代码
        // 向子进程发送SIGINT信号
        if (kill(pid, SIGINT) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }
        // 等待子进程结束
        wait(NULL);
        printf("Parent process exiting.\n");
    }

    return 0;
}
