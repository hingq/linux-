#include <cstdio>

int main() {
    FILE *fp1 = fopen("/etc/passwd", "r");
    // 打开文件
    char buffer[1024] = {0};
    fread(buffer, sizeof(char), sizeof(buffer), fp1);
    fclose(fp1);

    //FILE *fp2 = fopen("password.txt", "w+");
    // fopen 无法设置权限

    // 将文件原本内容全部丢弃 o_trunc
    FILE *fp2=open("password.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR  | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    fwrite(buffer, sizeof(buffer), 1, fp2);
    // 写入 
    fclose(fp2);

    return 0;
}