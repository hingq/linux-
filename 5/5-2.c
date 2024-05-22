#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void handler1() {
    printf(" exiting\n");
    exit(0);
}
void handler2() {
    printf(" ignoring\n");
}
void handler3() {
    printf("custom\n");
}
int main() {
    signal(SIGINT, handler1);
    // 进入无限循环，等待信号
    while(1) {
        sleep(1);
    }
    return 0;
}
