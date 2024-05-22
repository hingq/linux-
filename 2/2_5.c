#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
    char *dir_name = "my_directory";

    // 创建目录
    if (mkdir(dir_name, 0111) == -1) {
        perror("mkdir");
        return 1;
    }
    printf("created successfully with permissions --x--x--x\n");
    return 0;
}
