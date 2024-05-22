#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    pid_t pid;

    if (argc != 2) {
        printf("Usage: %s <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid = atoi(argv[1]); // 将参数转换为进程ID

    // 发送 SIGTERM 信号给指定的进程
    if (kill(pid, SIGTERM) == -1) {
        perror("Error sending SIGTERM signal");
        exit(EXIT_FAILURE);
    } else {
        printf("SIGTERM signal sent to process with PID %d\n", pid);
    }

    // 等待一段时间，以便目标进程有时间处理信号
    sleep(1);

    // 检查目标进程是否仍在运行
    if (kill(pid, 0) == 0) {
        printf(" %d is still running.\n", pid);
    } else {
        printf(" %d has been terminated.\n", pid);
    }

    return 0;
}
