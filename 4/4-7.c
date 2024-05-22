#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *pipe_fp;
    char buffer[BUFSIZ + 1];

    // 打开管道并执行命令
    // popen 会建立管道连到子进程的标准输出设备或标准输入设备，然后返回一个文件指针
    if ((pipe_fp = popen("cat file_example | grep root", "r")) == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    // 读取管道输出
    while (fgets(buffer, BUFSIZ, pipe_fp) != NULL) {
        printf("%s", buffer);
    }

    // 关闭管道
    if (pclose(pipe_fp) == -1) {
        perror("pclose");
        exit(EXIT_FAILURE);
    }

    return 0;
}
