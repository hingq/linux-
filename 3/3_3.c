#include<stdio.h>    
#include<string.h>                                                                                             
#include<stdlib.h>    
#include<unistd.h>    
#include<sys/wait.h>  

int main() {
    pid_t pid = fork(); // 创建子进程
    // 孤儿进程
    int stat;
    if (pid < 0) {
        // fork失败
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        printf("子进程ID：%d\n", getpid());
        sleep(10);
    } else {
        // 父进程
        wait(&stat);
        printf("父进程ID：%d\n", getpid());
        sleep(5); 
        printf("父进程结束\n");
    }

pid_t pid1 = fork(); // 创建子进程
    // 僵尸进程
    if (pid1 < 0) {
        // fork失败
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // 子进程
        printf("子进程ID：%d\n", getpid());
        sleep(5); 
        exit(EXIT_SUCCESS); // 子进程退出
    } else {
        // 父进程
        printf("父进程ID：%d\n", getpid());
        sleep(10); 
       system("ps -o pid,ppid,state,tty,command");
    }

    return 0;
}
