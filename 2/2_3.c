#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    // 打开文件
    int fd = open("test.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 设置文件记录锁
    struct flock lock;
    lock.l_type = F_WRLCK;  // 写锁  F_RDLCK read
    lock.l_whence = SEEK_SET; //决定l_start位置
    lock.l_start = 0;
    lock.l_len = 0;  // 锁定整个文件

    // 加锁
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    // 执行读写操作 
    getchar();
    // 释放锁
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // 关闭文件
    close(fd);

    return 0;
}
