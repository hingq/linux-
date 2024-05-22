#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork(); // 创建子进程

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // 调用exec族函数打开浏览器
        execlp("firefox", "firefox", "http://www.google.com.hk", NULL);
        //error
        perror("execl");
        exit(EXIT_FAILURE);
    }
    else {
        // 父进程
        printf("父进程ID：%d\n", getpid());
        int status;
        waitpid(pid, &status, 0); // 等待子进程结束
        if (WIFEXITED(status)) {
            printf("子进程退出值：%d\n", WEXITSTATUS(status));
        }
        else {
            printf("子进程异常结束\n");
        }
    }
    return 0;
}
