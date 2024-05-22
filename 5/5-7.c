#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

void sigusr1_handler(int signum) {

    printf("hahahhahahahhahahh \n");
}

int main() {
    // 注册 SIGUSR1 信号处理函数
    signal(SIGUSR1, sigusr1_handler);

    // 提示用户发送信号
    printf("Send  signal\n");

    // 发送 SIGUSR1 信号给自己
    raise(SIGUSR1);

    return 0;
}
