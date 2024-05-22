#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd;
    
    // 打开文件 ls.txt，如果不存在则创建，文件的权限为可读可写
    fd = open("ls.txt", O_WRONLY | O_CREAT | O_APPEND);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    // 将标准输出重定向到文件 ls.txt
    if (dup2(fd, 1) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    } 
    close(fd);  
    // 执行 ls -l 命令 第一个参数为path
    execlp("ls", "ls", "-l", NULL);
    // 如果execlp执行失败，则打印错误信息
    perror("execlp");
    exit(EXIT_FAILURE);
}
