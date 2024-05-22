#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int time; 

void alarm_handler(int signum) {
    time--; 
    printf("%d \n", time);
    alarm(1); 
}

int main() {
    time=10

    signal(SIGALRM, alarm_handler);
    // 设置定时器，每秒触发一次
    alarm(1);
    // 等待定时器结束
    while (time > 0) {
        // 等待信号触发
        pause();
    }

    printf("Time up\n");

    return 0;
}
