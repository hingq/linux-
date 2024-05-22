#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_FILE "myfifo"

int main() {
    char write_buffer[80];
    int fd;

    // 创建命名管道
    mkfifo(FIFO_FILE, 0666);

    // 打开命名管道以供读取
    fd = open(FIFO_FILE, O_RDWR);
    printf("Chat started... (type 'exit' to quit)\n");

    while (1) {
        fflush(stdin);
        printf("You: ");
        // 从标准输入读取用户输入的消息
        fgets(write_buffer, sizeof(write_buffer), stdin);
        
        // 将用户输入的消息写入命名管道
        write(fd, write_buffer, strlen(write_buffer) + 1);

        // 退出聊天
        if (strncmp(write_buffer, "exit", 4) == 0) {
            break;
        }
        memset(write_buffer, 0, sizeof(write_buffer));
    }

    // 关闭命名管道
    close(fd);
    unlink(FIFO_FILE); // 删除命名管道

    return 0;
}
