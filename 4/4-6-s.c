#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_FILE "myfifo"

int main() {
    char read_buffer[80];
    char write_buffer[80];
    int fd;

    // 创建命名管道
    mkfifo(FIFO_FILE, 0666);

    // 打开命名管道以供读取
    fd = open(FIFO_FILE, O_RDONLY);
    while (1) {
        
        // 退出聊天
        if (strncmp(write_buffer, "exit", 4) == 0) {
            break;
        }
        // 从命名管道读取另一进程发送的消息
        read(fd, read_buffer, sizeof(read_buffer));
       if(strlen(read_buffer)>0){
         printf("Other: %s", read_buffer);
       }
        // 清空缓冲区
        memset(read_buffer, 0, sizeof(read_buffer));
        memset(write_buffer, 0, sizeof(write_buffer));
    }

    // 关闭命名管道
    close(fd);
    unlink(FIFO_FILE); // 删除命名管道

    return 0;
}
