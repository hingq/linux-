#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define BUFFER_SIZE 128

// 全局变量，用于共享内存
char *shared_memory;
int shared_memory_fd;

// 读取源文件的线程函数
void *read_file(void *arg) {
    FILE *source_file = fopen("output.txt", "r");
    if (!source_file) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    size_t bytes_read;
    while ((bytes_read = fread(shared_memory, sizeof(char), BUFFER_SIZE, source_file)) > 0) {
        usleep(100);  // 模拟实际操作时的延迟
    }

    fclose(source_file);
    return NULL;
}

// 将共享内存中的内容写入目标文件的线程函数
void *write_file(void *arg) {
    FILE *target_file = fopen("target.txt", "w");
    if (!target_file) {
        perror("Error opening target file");
        exit(EXIT_FAILURE);
    }

    while (1) {
        fwrite(shared_memory, sizeof(char), BUFFER_SIZE, target_file);
        fflush(target_file);
        usleep(100);  // 模拟实际操作时的延迟
    }

    fclose(target_file);
    return NULL;
}

int main() {
    // 打开共享内存
    shared_memory_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, S_IRWXU);
    if (shared_memory_fd == -1) {
        perror("Error opening shared memory");
        exit(EXIT_FAILURE);
    }

    // 调整共享内存大小
    if (ftruncate(shared_memory_fd, BUFFER_SIZE) == -1) {
        perror("Error setting shared memory size");
        exit(EXIT_FAILURE);
    }

    // 映射共享内存
    shared_memory = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("Error mapping shared memory");
        exit(EXIT_FAILURE);
    }

    // 创建读取源文件的线程
    pthread_t read_thread;
    if (pthread_create(&read_thread, NULL, read_file, NULL) != 0) {
        perror("Error creating read thread");
        exit(EXIT_FAILURE);
    }

    // 创建将共享内存中的内容写入目标文件的线程
    pthread_t write_thread;
    if (pthread_create(&write_thread, NULL, write_file, NULL) != 0) {
        perror("Error creating write thread");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    // 解除映射并关闭共享内存
    munmap(shared_memory, BUFFER_SIZE);
    close(shared_memory_fd);

    return 0;
}
