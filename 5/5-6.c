#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void alarm_handler(int signum) {
 
}

unsigned int sleep(unsigned int seconds) {
    // 注册 SIGALRM 信号处理函数
    signal(SIGALRM, alarm_handler);
    // 设置定时器，一定时间间隔后发生信号
    alarm(seconds);
   // 调用 pause 函数使进程挂起，等待 SIGALRM 信号
    pause();
    // 定时器超时后，会唤醒进程，此时返回 0
    return 0;
}

int main() {
    printf("sleep for 3s \n")
    sleep(3);
    printf("Woke up\n");
    return 0;
}
