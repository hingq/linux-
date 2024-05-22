#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define BUFFER_SIZE 4096
#define PATH_MAX 255
// 函数声明
void copyFile(const char *sourcePath, const char *destinationPath);
void copyDirectory(const char *sourcePath, const char *destinationPath);

int main(int argc, char *argv[]) {

    // 复制文件或目录
    struct stat sourceStat;
    if (stat(argv[1], &sourceStat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    if (S_ISREG(sourceStat.st_mode)) {
        // 如果是普通文件，则复制文件
        copyFile(argv[1], argv[2]);
    } else if (S_ISDIR(sourceStat.st_mode)) {
        // 如果是目录，则复制目录
        copyDirectory(argv[1], argv[2]);
    } else {
        printf("Unsupported file type.\n");
        exit(EXIT_FAILURE);
    }

    printf("File or directory copied successfully!\n");
    return 0;
}

// 复制文件
void copyFile(const char *sourcePath, const char *destinationPath) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;
    
    // 打开源文件
    int sourceFd = open(sourcePath, O_RDONLY);
    if (sourceFd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 创建目标文件
    int destinationFd = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (destinationFd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 从源文件读取数据并写入目标文件
    while ((bytesRead = read(sourceFd, buffer, BUFFER_SIZE)) > 0) {
        bytesWritten = write(destinationFd, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // 检查读取是否出错
    if (bytesRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // 关闭文件描述符
    if (close(sourceFd) == -1 || close(destinationFd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

// 复制目录
void copyDirectory(const char *sourcePath, const char *destinationPath) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char sourceFile[PATH_MAX], destinationFile[PATH_MAX];

    // 创建目标目录
    if (mkdir(destinationPath, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }

    // 打开源目录
    dir = opendir(sourcePath);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // 遍历源目录中的文件和子目录
    while ((entry = readdir(dir)) != NULL) {
    //connect
        snprintf(sourceFile, PATH_MAX, "%s/%s", sourcePath, entry->d_name);
        snprintf(destinationFile, PATH_MAX, "%s/%s", destinationPath, entry->d_name);

        // 获取文件信息
        if (lstat(sourceFile, &statbuf) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        // 如果是目录，则递归复制目录
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                copyDirectory(sourceFile, destinationFile);
            }
        } else {
            // 否则复制文件
            copyFile(sourceFile, destinationFile);
        }
    }

    // 关闭目录
    if (closedir(dir) == -1) {
        perror("closedir");
        exit(EXIT_FAILURE);
    }
}
